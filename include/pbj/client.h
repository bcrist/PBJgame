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
#include "pbj/scene/entity.h"
#include "pbj/input_controller.h"
#include "pbj/net/net_platform.h"
#include "pbj/net/net_transport.h"

using pbj::net::Transport;
using pbj::Engine;
using pbj::gfx::Batcher;
using pbj::gfx::BuiltIns;
using pbj::Window;
using pbj::InputController;

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
		
		void draw();

		void onContextResized(I32, I32);

		//Networking members
		Transport* _transport;
		F32 _dt;
		ClientState _state;

		//Enginey stuff
		bool _running;
		Engine& _engine;
		Batcher& _batcher;
		const BuiltIns& _builtIns;
		Window& _window;
		//This variable is necessary only for demo purposes.  When actually
		//making the game it can go away
		F32 _sendMsgTimer;
	};

	
} // namespace pbj

#endif