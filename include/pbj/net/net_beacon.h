#ifndef NET_LAN_BEACON_H_
#define NET_LAN_BEACON_H_

#include "pbj/net/net_sockets.h"

//sends broadcast udp over the lan to advertise existence of the server
namespace pbj
{
namespace net
{
	class Beacon
	{
	public:
		Beacon(const U8* const, U32, U32, U32);
		~Beacon();

		bool start(I32);
		void stop();
		void update(F32);

	private:
		U8 _name[64+1];
		U32 _protoId;
		U16 _listenerPort;
		U16 _serverPort;
		bool _running;
		Socket _socket;
	};
}
}
#endif