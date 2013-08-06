#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <fstream>
#include <random>

#include "pbj/_pbj.h"
#include "pbj/engine.h"
#include "pbj/net/net_platform.h"
#include "pbj/net/net_transport.h"

using pbj::net::Transport;

namespace pbj
{
	class Server
	{
	public:
		static const I32 maxPacketSize = 512;

		static bool init(I32, U32);
		static I32 run();
		static void stop();
		
		~Server();
		
	private:
		static Server* _instance;
		
		Server();
		
		bool update();
		
		Transport* _transport;
		F32 _dt;
		
		//for testing purposes only
		F32 _sendMsgTimer;
	};
} //namespace pbj
#endif