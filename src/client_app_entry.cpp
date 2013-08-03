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
   if(!Transport::init())
   {
	   PBJ_LOG(pbj::VError) << "Failed to initialize transport layer" << PBJ_LOG_END;
	   return 1;
   }

   Transport* transport = Transport::create();

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
   //connect to server
   if(argc >= 2)
	   transport->connectClient((U8*)argv[1]);
   else
   {
	   U8 hostname[64+1];
	   Transport::getHostName(hostname, sizeof(hostname));
	   transport->connectClient(hostname);
   }

   //main loop
   const F32 dt = 1.0f/30.0f;
   bool connected = false;
   while(true)
   {
	   connected = (!connected && transport->isConnected());
	   if(connected && !transport->isConnected())
	   {
		   PBJ_LOG(pbj::VInfo) << "Disconnected" << PBJ_LOG_END;
		   break;
	   }

	   if(transport->connectFailed())
	   {
		   PBJ_LOG(pbj::VError) << "Connect failed" << PBJ_LOG_END;
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
