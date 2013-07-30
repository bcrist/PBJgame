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
				if(sequence_more_recent(it->seq,p.seq,max))
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

ReliabilitySystem::ReliabilitySystem(F32 rttMax=1.0f, U32 max=0xFFFFFFFF)
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
	_sentQueue.clear();
	_receivedQueue.clear();
	_ackedQueue.clear();
}