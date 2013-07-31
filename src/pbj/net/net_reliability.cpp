#ifndef NET_RELIABILITY_H_
#include "pbj/net/net_reliability.h"
#endif

using namespace pbj;
using namespace pbj::net;

bool PacketQueue::exists(U32 sequence)
{
	for(iterator it=begin();it!=end();++it)
		if(it->seq == sequence)
			return true;
	return false;
}

void PacketQueue::insert_sorted(const PacketData& p, U32 maxSeq)
{
	if(empty())
	{
		push_back(p);
	}
	else
	{
		if(!sequence_more_recent(p.seq, front().seq, maxSeq))
		{
			push_front(p);
		}
		else if(sequence_more_recent(p.seq, back().seq, maxSeq))
		{
			push_back(p);
		}
		else
		{
			for(iterator it=begin();it!=end();it++)
			{
				assert(it->seq!=p.seq);
				if(sequence_more_recent(it->seq,p.seq, maxSeq))
				{
					insert(it,p);
					break;
				}
			}
		}
	}
}

void PacketQueue::verify_sorted(U32 max)
{
	iterator prev = end();
	for (iterator it=begin();it!=end();it++)
	{
		assert(it->seq <= max);
		if (prev != end())
		{
			assert(sequence_more_recent(it->seq, prev->seq, max));
			prev = it;
		}
	}
}

bool ReliabilitySystem::moreRecentSeq(U32 s1, U32 s2, U32 max)
{
	return (s1>s2) && (s1-s2<=max/2) || (s2>s1) && (s2-s1>max/2);
}

I32 ReliabilitySystem::bitIndexForSeq(U32 seq, U32 ack, U32 max)
{
	assert(seq!=ack);
	assert(!moreRecentSeq(seq, ack, max));
	if(seq > ack)
	{
		assert(ack<33);
		assert(max>=seq);
		return ack + max - seq;
	}
	else
	{
		assert(ack>=1);
		assert(seq<=ack-1);
		return ack - 1 - seq;
	}
}

U32 ReliabilitySystem::generateAckBits(U32 ack, const PacketQueue& receivedQueue, U32 max)
{
	U32 ackBits = 0;
	for(PacketQueue::const_iterator it = receivedQueue.begin();
		it != receivedQueue.end();
		it++ )
	{
		if(it->seq==ack || sequence_more_recent(it->seq, ack, max))
			break;
		I32 bitIdx = bitIndexForSeq(it->seq, ack, max);
		if(bitIdx <= 31 )
			ackBits |= 1 << bitIdx;
	}
	return ackBits;
}

void ReliabilitySystem::sProccessAck(U32 ack, U32 ackBits, PacketQueue& pending,
										PacketQueue& acked, vector<U32>& acks,
										U32& ackedPackets, F32& rtt, U32 max)
{
	if(pending.empty())
		return;
	PacketQueue::iterator it = pending.begin();
	while(it!=pending.end())
	{
		bool isAcked = false;

		if(it->seq == ack)
		{
			isAcked = true;
		}
		else if(!sequence_more_recent(it->seq, ack, max))
		{
			I32 bitIdx = bitIndexForSeq(it->seq, ack, max);
			if(bitIdx<=31)
				isAcked = (ackBits>>bitIdx) & 1;
		}

		if(isAcked)
		{
			rtt += (it->time - rtt) * 0.1f;
			acked.insert_sorted(*it, max);
			acks.push_back(it->seq);
			ackedPackets++;
			it = pending.erase(it);
		}
		else
			++it;
	}
}

ReliabilitySystem::ReliabilitySystem()
{
	_rttMax = 1.0f;
	_maxSeq = 0xFFFFFFFF;
	reset();
}

ReliabilitySystem::ReliabilitySystem(F32 rttMax, U32 max)
{
	_rttMax = rttMax;
	_maxSeq = max;
	reset();
}

ReliabilitySystem::~ReliabilitySystem() {}

void ReliabilitySystem::reset()
{
	_localSeq = 0;
	_remoteSeq = 0;
	_sentPackets = 0;
	_receivedPackets = 0;
	_lostPackets = 0;
	_ackedPackets = 0;
	_sentBandwidth = 0.0f;
	_ackedBandwidth = 0.0f;
	_rtt = 0.0f;
	_rttMax = 1.0f;
	_acks.clear();
	_pendingAckQueue.clear();
	_sentQueue.clear();
	_receivedQueue.clear();
	_ackedQueue.clear();
}

void ReliabilitySystem::packetSent(I32 size)
{
	if(_sentQueue.exists(_localSeq))
	{
		printf( "local sequence %d exists\n", _localSeq);
		for(PacketQueue::iterator it=_sentQueue.begin();
				it != _sentQueue.end();
				++it)
			printf( " + %d\n", it->seq );
	}
	assert(!_sentQueue.exists(_localSeq));
	assert(!_pendingAckQueue.exists(_localSeq));
	PacketData data;
	data.seq = _localSeq;
	data.time = 0.0f;
	data.size = size;
	_sentQueue.push_back(data);
	_pendingAckQueue.push_back(data);
	_sentPackets++;
	_localSeq++;
	if(_localSeq > _maxSeq)
		_localSeq = 0;
}

void ReliabilitySystem::packetReceived(U32 sequence, I32 size)
{
	_receivedPackets++;
	if( _receivedQueue.exists(sequence))
		return;
	PacketData data;
	data.seq = sequence;
	data.time = 0.0f;
	data.size = size;
	_receivedQueue.push_back(data);
	if(sequence_more_recent(sequence, _remoteSeq, _maxSeq))
		_remoteSeq = sequence;
}

U32 ReliabilitySystem::generateAckBits()
{
	return generateAckBits(getRemoteSequence(), _receivedQueue, _maxSeq);
}

void ReliabilitySystem::processAck(U32 ack, U32 ackBits)
{
	sProccessAck(ack, ackBits, _pendingAckQueue, _ackedQueue, _acks, _ackedPackets, _rtt, _maxSeq);
}

void ReliabilitySystem::update(F32 dt)
{
	_acks.clear();
	advanceQueueTime(dt);
	updateQueues();
	updateStats();
}

U32 ReliabilitySystem::getLocalSequence() const
{
	return _localSeq;
}

U32 ReliabilitySystem::getRemoteSequence() const
{
	return _remoteSeq;
}

U32 ReliabilitySystem::getmaxSequence() const
{
	return _maxSeq;
}

void ReliabilitySystem::getAcks(U32** acks, I32& count)
{
	*acks = &_acks[0];
	count = _acks.size();
}

U32 ReliabilitySystem::getSentPackets() const
{
	return _sentPackets;
}

U32 ReliabilitySystem::getReceivedPackets() const
{
	return _receivedPackets;
}

U32 ReliabilitySystem::getLostPackets() const
{
	return _lostPackets;
}

U32 ReliabilitySystem::getAckedPackets() const
{
	return _ackedPackets;
}

F32 ReliabilitySystem::getSentBandwidth() const
{
	return _sentBandwidth;
}

F32 ReliabilitySystem::getAckedBandwidth() const
{
	return _ackedBandwidth;
}

F32 ReliabilitySystem::getRoundTripTimwe() const
{
	return _rtt;
}

I32 ReliabilitySystem::getHeaderSize() const
{
	return 12; //MAGIC!
}

void ReliabilitySystem::advanceQueueTime(F32 dt)
{
	PacketQueue::iterator it;

	it = _pendingAckQueue.begin();
	while(it!=_pendingAckQueue.end())
	{
		it->time+=dt;
		++it;
	}

	it = _sentQueue.begin();
	while(it!=_sentQueue.end())
	{
		it->time+=dt;
		++it;
	}

	it = _receivedQueue.begin();
	while(it!=_receivedQueue.end())
	{
		it->time+=dt;
		++it;
	}

	it = _ackedQueue.begin();
	while(it!=_ackedQueue.end())
	{
		it->time+=dt;
		++it;
	}

}

void ReliabilitySystem::updateQueues()
{
	const F32 epsilon = 0.001f;

	while(_sentQueue.size() && _sentQueue.front().time > _rttMax + epsilon )
		_sentQueue.pop_front();

	if(_receivedQueue.size())
	{
		const U32 latest_sequence = _receivedQueue.back().seq;
		const U32 minimum_sequence = latest_sequence >= 34 ? (latest_sequence-34):_maxSeq-(34-latest_sequence);
		while(_receivedQueue.size() && !sequence_more_recent(_receivedQueue.front().seq, minimum_sequence, _maxSeq))
			_receivedQueue.pop_front();
	}

	while(_ackedQueue.size() && _ackedQueue.front().time > _rttMax*2 - epsilon )
		_ackedQueue.pop_front();

	while(_pendingAckQueue.size() && _pendingAckQueue.front().time > _rttMax + epsilon)
	{
		_pendingAckQueue.pop_front();
		_lostPackets++;
	}
}

void ReliabilitySystem::updateStats()
{
	F32 sent_bytes_per_second = 0.0f;
	for(PacketQueue::iterator it=_sentQueue.begin();
		it!=_sentQueue.end();
		++it)
		sent_bytes_per_second += it->size;
	F32 acked_packets_per_second = 0.0f;
	F32 acked_bytes_per_second = 0.0f;
	for(PacketQueue::iterator it=_ackedQueue.begin();
		it!=_ackedQueue.end();
		++it)
	{
		if(it->time >= _rttMax)
		{
			acked_packets_per_second++;
			acked_bytes_per_second += it->size;
		}
	}
	sent_bytes_per_second /= _rttMax;
	acked_bytes_per_second /= _rttMax;
	_sentBandwidth = sent_bytes_per_second * ( 8 / 1000.0f );
	_ackedBandwidth = acked_bytes_per_second * ( 8 / 1000.0f );
}