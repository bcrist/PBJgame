////////////////////////////////////////////////////////////////////////////////
/// \file	Z:\Documents\PBJgame\src\pbj\net\net_reliability.cpp
///
/// \brief	Implements the net reliability class.
////////////////////////////////////////////////////////////////////////////////
#ifndef NET_RELIABILITY_H_
#include "pbj/net/net_reliability.h"
#endif

using namespace pbj;
using namespace pbj::net;

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool PacketQueue::exists(U32 sequence)
///
/// \brief	Exists the given sequence.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	sequence	The sequence.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
bool PacketQueue::exists(U32 sequence)
{
	for(iterator it=begin();it!=end();++it)
		if(it->seq == sequence)
			return true;
	return false;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PacketQueue::insert_sorted(const PacketData& p, U32 maxSeq)
///
/// \brief	Inserts a sorted.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	p	  	The const PacketData&amp; to process.
/// \param	maxSeq	The maximum sequence.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PacketQueue::verify_sorted(U32 max)
///
/// \brief	Verify sorted.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	max	The maximum.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool ReliabilitySystem::moreRecentSeq(U32 s1, U32 s2, U32 max)
///
/// \brief	More recent sequence.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	s1 	The first U32.
/// \param	s2 	The second U32.
/// \param	max	The maximum.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
bool ReliabilitySystem::moreRecentSeq(U32 s1, U32 s2, U32 max)
{
	return (s1>s2) && (s1-s2<=max/2) || (s2>s1) && (s2-s1>max/2);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	I32 ReliabilitySystem::bitIndexForSeq(U32 seq, U32 ack, U32 max)
///
/// \brief	Bit index for sequence.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	seq	The sequence.
/// \param	ack	The acknowledge.
/// \param	max	The maximum.
///
/// \return	.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	U32 ReliabilitySystem::generateAckBits(U32 ack,
/// 	const PacketQueue& receivedQueue, U32 max)
///
/// \brief	Generates an acknowledge bits.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	ack			 	The acknowledge.
/// \param	receivedQueue	Queue of receiveds.
/// \param	max			 	The maximum.
///
/// \return	The acknowledge bits.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	void ReliabilitySystem::sProccessAck(U32 ack, U32 ackBits,
/// 	PacketQueue& pending, PacketQueue& acked, vector<U32>& acks,
/// 	U32& ackedPackets, F32& rtt, U32 max)
///
/// \brief	Proccess acknowledge.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	ack						The acknowledge.
/// \param	ackBits					The acknowledge bits.
/// \param [in,out]	pending			The pending.
/// \param [in,out]	acked			The acked.
/// \param [in,out]	acks			The acks.
/// \param [in,out]	ackedPackets	The acked packets.
/// \param [in,out]	rtt				The rtt.
/// \param	max						The maximum.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	ReliabilitySystem::ReliabilitySystem()
///
/// \brief	Default constructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
ReliabilitySystem::ReliabilitySystem()
{
	_rttMax = 1.0f;
	_maxSeq = 0xFFFFFFFF;
	reset();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	ReliabilitySystem::ReliabilitySystem(F32 rttMax, U32 max)
///
/// \brief	Constructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	rttMax	The rtt maximum.
/// \param	max   	The maximum.
////////////////////////////////////////////////////////////////////////////////
ReliabilitySystem::ReliabilitySystem(F32 rttMax, U32 max)
{
	_rttMax = rttMax;
	_maxSeq = max;
	reset();
}

ReliabilitySystem::~ReliabilitySystem() {}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void ReliabilitySystem::reset()
///
/// \brief	Resets this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	void ReliabilitySystem::packetSent(I32 size)
///
/// \brief	Packet sent.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	size	The size.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	void ReliabilitySystem::packetReceived(U32 sequence, I32 size)
///
/// \brief	Packet received.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	sequence	The sequence.
/// \param	size		The size.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	U32 ReliabilitySystem::generateAckBits()
///
/// \brief	Generates an acknowledge bits.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The acknowledge bits.
////////////////////////////////////////////////////////////////////////////////
U32 ReliabilitySystem::generateAckBits()
{
	return generateAckBits(getRemoteSequence(), _receivedQueue, _maxSeq);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void ReliabilitySystem::processAck(U32 ack, U32 ackBits)
///
/// \brief	Process the acknowledge.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	ack	   	The acknowledge.
/// \param	ackBits	The acknowledge bits.
////////////////////////////////////////////////////////////////////////////////
void ReliabilitySystem::processAck(U32 ack, U32 ackBits)
{
	sProccessAck(ack, ackBits, _pendingAckQueue, _ackedQueue, _acks, _ackedPackets, _rtt, _maxSeq);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void ReliabilitySystem::update(F32 dt)
///
/// \brief	Updates the given dt.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	dt	The dt.
////////////////////////////////////////////////////////////////////////////////
void ReliabilitySystem::update(F32 dt)
{
	_acks.clear();
	advanceQueueTime(dt);
	updateQueues();
	updateStats();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	U32 ReliabilitySystem::getLocalSequence() const
///
/// \brief	Gets local sequence.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The local sequence.
////////////////////////////////////////////////////////////////////////////////
U32 ReliabilitySystem::getLocalSequence() const
{
	return _localSeq;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	U32 ReliabilitySystem::getRemoteSequence() const
///
/// \brief	Gets remote sequence.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The remote sequence.
////////////////////////////////////////////////////////////////////////////////
U32 ReliabilitySystem::getRemoteSequence() const
{
	return _remoteSeq;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	U32 ReliabilitySystem::getmaxSequence() const
///
/// \brief	Gets the getmax sequence.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	.
////////////////////////////////////////////////////////////////////////////////
U32 ReliabilitySystem::getmaxSequence() const
{
	return _maxSeq;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void ReliabilitySystem::getAcks(U32** acks, I32& count)
///
/// \brief	Gets the acks.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param [in,out]	acks 	If non-null, the acks.
/// \param [in,out]	count	Number of.
////////////////////////////////////////////////////////////////////////////////
void ReliabilitySystem::getAcks(U32** acks, I32& count)
{
	*acks = &_acks[0];
	count = _acks.size();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	U32 ReliabilitySystem::getSentPackets() const
///
/// \brief	Gets sent packets.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The sent packets.
////////////////////////////////////////////////////////////////////////////////
U32 ReliabilitySystem::getSentPackets() const
{
	return _sentPackets;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	U32 ReliabilitySystem::getReceivedPackets() const
///
/// \brief	Gets received packets.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The received packets.
////////////////////////////////////////////////////////////////////////////////
U32 ReliabilitySystem::getReceivedPackets() const
{
	return _receivedPackets;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	U32 ReliabilitySystem::getLostPackets() const
///
/// \brief	Gets lost packets.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The lost packets.
////////////////////////////////////////////////////////////////////////////////
U32 ReliabilitySystem::getLostPackets() const
{
	return _lostPackets;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	U32 ReliabilitySystem::getAckedPackets() const
///
/// \brief	Gets acked packets.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The acked packets.
////////////////////////////////////////////////////////////////////////////////
U32 ReliabilitySystem::getAckedPackets() const
{
	return _ackedPackets;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	F32 ReliabilitySystem::getSentBandwidth() const
///
/// \brief	Gets sent bandwidth.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The sent bandwidth.
////////////////////////////////////////////////////////////////////////////////
F32 ReliabilitySystem::getSentBandwidth() const
{
	return _sentBandwidth;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	F32 ReliabilitySystem::getAckedBandwidth() const
///
/// \brief	Gets acked bandwidth.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The acked bandwidth.
////////////////////////////////////////////////////////////////////////////////
F32 ReliabilitySystem::getAckedBandwidth() const
{
	return _ackedBandwidth;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	F32 ReliabilitySystem::getRoundTripTimwe() const
///
/// \brief	Gets round trip timwe.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The round trip timwe.
////////////////////////////////////////////////////////////////////////////////
F32 ReliabilitySystem::getRoundTripTimwe() const
{
	return _rtt;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	I32 ReliabilitySystem::getHeaderSize() const
///
/// \brief	Gets header size.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The header size.
////////////////////////////////////////////////////////////////////////////////
I32 ReliabilitySystem::getHeaderSize() const
{
	return 12; //MAGIC!
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void ReliabilitySystem::advanceQueueTime(F32 dt)
///
/// \brief	Advance queue time.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	dt	The dt.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	void ReliabilitySystem::updateQueues()
///
/// \brief	Updates the queues.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	void ReliabilitySystem::updateStats()
///
/// \brief	Updates the stats.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
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