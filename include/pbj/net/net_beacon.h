////////////////////////////////////////////////////////////////////////////////
/// \file	Z:\Documents\PBJgame\include\pbj\net\net_beacon.h
///
/// \brief	Declares the net beacon class.
////////////////////////////////////////////////////////////////////////////////
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
		Beacon(const U8* const, U32, U16, U16);
		~Beacon();

		bool start(U16);
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