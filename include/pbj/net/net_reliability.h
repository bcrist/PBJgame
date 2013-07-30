#ifndef NET_RELIABILITY_H_
#define NET_RELIABILITY_H_

#include <assert.h>
#include <vector>
#include <map>
#include <stack>
#include <list>
#include <algorithm>
#include <functional>
#include "pbj/_pbj.h"

using std::vector;

namespace pbj
{
namespace net
{
	inline bool sequence_more_recent(U32 s1, U32 s2, U32 max)
	{
		return (s1>s2) && (s1-s2<=max/2) || (s2>s1) && (s2-s1>max/2);
	}

	struct PacketData
	{
		U32 seq;
		F32 time;
		I32 size;
	};

	class PacketQueue : public std::list<PacketData>
	{
	public:
		bool exists(U32);
		void insert_sorted(const PacketData&, U32);
		void verify_sorted(U32);
	};

	class ReliabilitySystem
	{
	public:
		static bool moreRecentSeq(U32, U32, U32);
		static I32 bitIndexForSeq(U32, U32, U32);
		static U32 generateAckBits(U32, const PacketQueue&, U32);
		static void sProccessAck(U32, U32, PacketQueue&, PacketQueue&,
							std::vector<U32>&, U32&, F32&, U32);

		ReliabilitySystem(F32, U32);
		~ReliabilitySystem();

		void reset();
		void packetSent(I32);
		void packetReceived(U32, I32);
		U32 generateAckBits();
		void processAck(U32, U32);
		void update(F32);
		void validate();
		U32 getLocalSequence() const;
		U32 getRemoteSequence() const;
		U32 getmaxSequence() const;
		void getAcks(U32**, I32&);
		U32 getSentPackets() const;
		U32 getReceivedPackets() const;
		U32 getLostPackets() const;
		U32 getAckedPackets() const;
		F32 getSentBandwidth() const;
		F32 getAckedBandwidth() const;
		F32 getRoundTripTimwe() const;
		I32 getHeaderSize() const;

	protected:
		void advanceQueueTime(F32 dt);
		void updateQueues();
		void updateStats();

	private:
		U32 _maxSeq;
		U32 _localSeq;
		U32 _remoteSeq;
		U32 _sentPackets;
		U32 _receivedPackets;
		U32 _lostPackets;
		U32 _ackedPackets;
		F32 _sentBandwidth;
		F32 _ackedBandwidth;
		F32 _rtt;
		F32 _rttMax;
		vector<U32> _acks;
		PacketQueue _sentQueue;
		PacketQueue _receivedQueue;
		PacketQueue _ackedQueue;
	};
}
}
#endif