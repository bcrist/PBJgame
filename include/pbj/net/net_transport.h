/*
	Simple Network Library from "Networking for Game Programmers"
	http://www.gaffer.org/networking-for-game-programmers
	Author: Glenn Fiedler <gaffer@gaffer.org>
*/

#ifndef NET_TRANSPORT_H_
#define NET_TRANSPORT_H_

#include "pbj/_pbj.h"

namespace pbj
{
namespace net
{
	enum TransportType
	{
		None=0x01,
		LAN=0x02
	};

	class Transport
	{
	public:
		static bool init(TransportType);
		static void shutdown();
		static Transport* create();
		static void destroy(Transport*);

		virtual ~Transport() {};
		virtual bool isNoneConnected(int) = 0;
		virtual int getLocalNodeID() const = 0;
		virtual int getMaxNodes() const = 0;
		virtual bool sendPacket(int, const U8* const, int)=0;
		virtual int receivePacket(int&, U8*, int)=0;
		virtual class ReliabilitySystem& getReliability(int)=0;
		virtual void update(F32)=0;
		virtual TransportType getType() const=0;
	};
}
}
#endif
