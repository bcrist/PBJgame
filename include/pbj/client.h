////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJgame\include\pbj\client.h
///
/// \brief	Declares the client class.
////////////////////////////////////////////////////////////////////////////////
#ifndef CLIENT_H_
#define CLIENT_H_

#include <iostream>
#include <fstream>
#include <random>
#include <string>

#include "pbj/_pbj.h"
#include "pbj/engine.h"
#include "pbj/net/net_platform.h"
#include "pbj/net/net_transport.h"

using pbj::net::Transport;

namespace pbj
{
	////////////////////////////////////////////////////////////////////////////
	/// \class		Client
	///
	/// \brief		Client.
	///
	/// \author		Peter Bartosch
	/// \date		2013-08-05
	/// \details	The Client class manages and runs the game from the
	/// 			perspective of the player.
	////////////////////////////////////////////////////////////////////////////
	class Client
	{
	public:
		static bool init(U32);
		static bool init(const U8* const, U32);
		static I32 run();
		static void stop();
		
		~Client();
		
	private:

		////////////////////////////////////////////////////////////////////////
		/// \enum	ClientState
		///
		/// \brief	Values that represent ClientState.
		////////////////////////////////////////////////////////////////////////
		enum ClientState
		{
			Disconnected = 0x01,
			Searching = 0x02,
			Connecting = 0x04,
			Connected = 0x08
		};
		static Client* _instance;

		Client();
		
		bool update();
		
		bool displayLobby();
		bool doConnecting();
		bool doConnected();

		void joinServer(Transport::LobbyEntry);
		
		Transport* _transport;
		F32 _dt;
		ClientState _state;
		
		//This variable is necessary only for demo purposes.  When actually
		//making the game it can go away
		F32 _sendMsgTimer;
	};

	
} // namespace pbj

#endif