///////////////////////////////////////////////////////////////////////////////
/// \file   server_app_entry.cpp
/// \author Benjamin Crist and Peter Bartosch
///
/// \brief  Application entry point.
/// \details Parses command line parameters, initializes game engine, loads
///         configuration data, and starts server.

// Auto-link with libraries
#ifdef _WIN32
#pragma comment (lib, "opengl32.lib")
#ifdef DEBUG
#pragma comment (lib, "glew32sd.lib")
#pragma comment (lib, "glfw3sd.lib")
#pragma comment (lib, "assimpsd.lib")
#else
#pragma comment (lib, "glew32s.lib")
#pragma comment (lib, "glfw3s.lib")
#pragma comment (lib, "assimps.lib")
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

void runServer(Transport*);
Transport* initServer();

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
   std::string cerr_log("pbjed.log");

   if (argc != 1)
   {
      std::cout << "PBJgame " << PBJ_VERSION_MAJOR << '.' << PBJ_VERSION_MINOR << " (" << __DATE__ " " __TIME__ << ')' << std::endl
                << PBJ_COPYRIGHT << std::endl;
      return 1;
   }
   
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
   runServer(initServer());
   // TODO: start server
   return 0;
};

Transport* initServer()
{
	if(!Transport::init())
	{
		PBJ_LOG(pbj::VError) << "Failed to initialize transport layer" << PBJ_LOG_END;
		return 0;
	}

	Transport* ret = 0;
	ret = Transport::create();
	if(ret == 0)
	{
		PBJ_LOG(pbj::VError) << "Failed to create transport object" << PBJ_LOG_END;
		delete ret;
		return 0;
	}

	 Transport::Config cfg = ret->getConfig();
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

	U8 hostname[64+1] = "hostname";
	ret->getHostName(hostname, sizeof(hostname));
	ret->startServer(hostname);

	return ret;
}

void runServer(Transport* transport)
{ 
	if(transport==0)
		return;
	
	F32 sendTimer = 0.0f;
	const F32 dt = 1.0f/30.0f;
	const I32 maxSize = 512;
	while(true)
	{
		//first receive any information we might need
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
				std::cerr<<"Server: received message from "<<nodeId<<":\n\t"<<msg<<std::endl;
			}
		}


		sendTimer+=dt;
		if(sendTimer >= 1.0f)
		{
			I32 n = transport->getNumberConnected();
			for(I32 i=1;i<n;++i) //I don't like this
			{
				U8 packet[512];
				net::writeInteger(packet, transport->getConfig().protoId);
				packet[4] = 0x02; //type = 2, which is nothing for the time being
				packet[5] = (U8)i;
				strcpy_s((char*)(packet+6),19+1,"This is just a test");
				if(transport->sendPacket(i,packet,512))
					std::cerr<<"Sent packet to node: "<<i<<std::endl;
			}
			sendTimer-=1.0f;
		}
		transport->update(dt);
		pbj::net::waitSeconds(dt);
	}
	Transport::destroy(transport);
	Transport::shutdown();
}
#endif
