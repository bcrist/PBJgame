// Copyright (c) 2013 PBJ Productions
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

///////////////////////////////////////////////////////////////////////////////
/// \file   app_entry.cpp
/// \author Benjamin Crist
///
/// \brief  Application entry point.
/// \details Parses command line parameters, initializes game engine, loads
///         configuration data, and starts game.

// Auto-link with libraries
#ifdef _WIN32
#pragma comment (lib, "opengl32.lib")
#ifdef DEBUG
#pragma comment (lib, "glew32sd.lib")
#pragma comment (lib, "glfw3sd.lib")
#else
#pragma comment (lib, "glew32s.lib")
#pragma comment (lib, "glfw3s.lib")
#endif // DEBUG
#endif // _WIN32

// Is this a unit testing build?
#ifdef PBJ_TEST
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#else

#include "pbj/engine.h"
#include "pbj/net/net_platform.h"
#include "pbj/net/net_transport.h"

#include <iostream>
#include <fstream>
#include <random>
#include <string>

#if defined(_WIN32) && !defined(DEBUG)
#include <windows.h>
int main(int argc, char* argv[]);

///////////////////////////////////////////////////////////////////////////////
/// \brief  Win32 API entry point; redirects to main().
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
   return main(__argc, __argv);
}

#endif // defined(_WIN32) && !defined(DEBUG)

using namespace pbj;
using pbj::net::Transport;

enum ClientState
{
	Searching = 0x01,
	Connecting = 0x02,
	Connected = 0x04
};

Transport* transport;
ClientState state;
F32 clientToServerTimer;

I32 initializeClient();
bool viewLobby();
void joinServer(Transport::LobbyEntry);
bool doConnecting();
bool doConnected(F32);

I32 initializeClient()
{
	if(!Transport::init())
	{
		PBJ_LOG(pbj::VError) << "Failed to initialize transport layer" << PBJ_LOG_END;
		return 1;
	}

	transport = Transport::create();

	if(!transport)
	{
		PBJ_LOG(pbj::VError) << "Could not create transport" << PBJ_LOG_END;
		return 1;
	}
	Transport::Config cfg = transport->getConfig();
	std::cerr<<"Transport configuration:"<<std::endl;
	std::cerr<<"\tMesh Port:\t"<<cfg.meshPort<<std::endl;
	std::cerr<<"\tClient Port:\t"<<cfg.clientPort<<std::endl;
	std::cerr<<"\tServer Port:\t"<<cfg.serverPort<<std::endl;
	std::cerr<<"\tBeacon Port:\t"<<cfg.beaconPort<<std::endl;
	std::cerr<<"\tListener Port:\t"<<cfg.listenerPort<<std::endl;
	std::cerr<<"\tProto Id:\t"<<cfg.protoId<<std::endl;
	std::cerr<<"\tMesh Send Rate:\t"<<cfg.meshSendRate<<std::endl;
	std::cerr<<"\tTimeout:\t"<<cfg.timeout<<std::endl;
	std::cerr<<"\tmax Nodes:\t"<<cfg.maxNodes<<std::endl;

	state = Searching;
	clientToServerTimer = 0.0f;
	return 0;
}

bool viewLobby()
{
	I32 entryCount = transport->getLobbyEntryCount();
	Transport::LobbyEntry entry;
	std::cerr<<"--------------------------------------------------------------------------------"<<std::endl
				<<"Available Servers"<<std::endl;
	for(I32 i=0;i<entryCount;++i)
	{
		if(transport->getLobbyEntryAtIndex(i, entry))
			std::cerr<<"\t"<<(i+1)<<". "<<entry.name<<" ("<<entry.address<<")"<<std::endl;
	}
	std::cerr<<"Enter 'R' to refresh, 'Q' to quit or the number of the server."<<std::endl;
	std::cerr<<"Choice:"<<std::endl;
	std::string choice;

	//I'm expect this to halt all execution of the loop in main while it waits for input
	std::getline(std::cin,choice);
	if(choice.c_str()[0]=='R' || choice.c_str()[0]=='r')
	{
		return false;
	}
	else if(choice.c_str()[0]=='Q' || choice.c_str()[0]=='q')
	{
		return true;
	}
	else
	{
		I32 iChoice = atoi(choice.c_str());
		if(iChoice<=entryCount)
		{
			transport->getLobbyEntryAtIndex(iChoice-1,entry);
			joinServer(entry);
		}
		//and why not just refresh if invalid input.
	}
	return false;
}

void joinServer(Transport::LobbyEntry entry)
{
	if(!transport->leaveLobby())
	{
		std::cerr<<"Leaving lobby failed!"<<std::endl;
		return;
	}
	transport->connectClient(entry.address);
	state = Connecting;
}

bool doConnecting()
{
	if(transport->connectFailed())
	{
		PBJ_LOG(pbj::VError) << "Connect failed" << PBJ_LOG_END;
		return true;
	}

	if(transport->isConnected())
		   state = Connected;
	return false;
}

bool doConnected(F32 dt)
{
	//first receive any information we might need
	const I32 maxSize = 512;
	I32 nodeId;
	U8 data[maxSize];
	I32 bytesReceived = transport->receivePacket(nodeId, data, maxSize);
	if(bytesReceived != 0)
	{ //we actually got something!
		U32 protoId;
		net::readInteger(data, protoId);
		if(transport->getConfig().protoId == protoId)
		{ //even better, it's the right protocol!
			//because this is just a test, I will ignore other checks.
			U8* msg = new U8[bytesReceived-4];
			strncpy_s((char*)msg, bytesReceived-4, (char*)(data+6), bytesReceived-4);
			std::cerr<<"Client: received message:\n\t"<<msg<<std::endl;
		}
	}

	//now send messages to the server
	clientToServerTimer+=dt;
	if(clientToServerTimer >= 1.5f)
	{
		U8 packet[512];
		net::writeInteger(packet, transport->getConfig().protoId);
		packet[4] = 0x03; //type = 3, which is nothing for the time being
		packet[5] = 0;
		strcpy_s((char*)(packet+6),38+1,"This is just a test (client to server)");
		if(transport->sendPacket(0,packet,512))
			std::cerr<<"Sent packet to server"<<std::endl;
		clientToServerTimer-=1.5f;
	}

	if(!transport->isConnected())
	{
		PBJ_LOG(pbj::VInfo) << "Disconnected" << PBJ_LOG_END;
		return true;
	}
	return false;
}
///////////////////////////////////////////////////////////////////////////////
/// \brief  Application entry point
/// \details Parses any command line arguments, then initializes game engine.
int main(int argc, char* argv[])
{
#ifdef BE_CRT_BUILD
   _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

#ifdef DEBUG
   int verbosity = pbj::VAll;
#else
   int verbosity = pbj::VErrorsAndWarnings;
#endif
   std::ofstream cerr_log_file;
   std::string cerr_log("pbjgame.log");

   /*if (argc != 1)
   {
      std::cout << "PBJgame " << PBJ_VERSION_MAJOR << '.' << PBJ_VERSION_MINOR << " (" << __DATE__ " " __TIME__ << ')' << std::endl
                << PBJ_COPYRIGHT << std::endl;
      return 1;
   }*/
   
   // Set the appropriate verbosity level
   be::setVerbosity(verbosity);

   // Redirect PBJ_LOG_STREAM to a log file if not in DEBUG mode
#ifndef DEBUG
   if (cerr_log.length() > 0)
   {
      PBJ_LOG(pbj::VNotice) << "Redirecting log to " << cerr_log << PBJ_LOG_END;
      cerr_log_file.open(cerr_log, std::ofstream::trunc);
      PBJ_LOG_STREAM.rdbuf(cerr_log_file.rdbuf());
   }
#endif

   // Initialize game engine
   //pbj::Engine engine;

   // TODO: start game
   if(initializeClient()==1)
	   return 1;
   assert(transport);

   //connect to server or look for one
   if(argc >= 2) //address passed to client, try to do direct connect
   {
	   transport->connectClient((U8*)argv[1]);
	   state = Connecting;
   }
   else
   {
	   transport->enterLobby();
   }

   //main loop
   const F32 dt = 1.0f/30.0f;
   bool breakLoop = false;
   std::cerr<<"Starting client loop"<<std::endl;
	while(!breakLoop)
	{
		switch(state)
		{
		case Searching:
			breakLoop = viewLobby();
			break;
		case Connecting:
			breakLoop = doConnecting();
			break;
		case Connected:
			breakLoop = doConnected(dt);
			break;
		default:
			break;
		}

		transport->update(dt);
		net::waitSeconds(dt);
	}

   //shutdown
   Transport::destroy(transport);
   Transport::shutdown();

   return 0;
};

#endif
