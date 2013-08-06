////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJgame\include\pbj\server.h
///
/// \brief	Declares the server class.
////////////////////////////////////////////////////////////////////////////////
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
	////////////////////////////////////////////////////////////////////////////
	/// \class	Server
	///
	/// \brief	Server.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	////////////////////////////////////////////////////////////////////////////
	class Server
	{
	public:
		/// \brief	Maximum size of a packet.
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