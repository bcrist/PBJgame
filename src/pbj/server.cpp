////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJgame\src\pbj\server.cpp
///
/// \brief	Implements the server class.
////////////////////////////////////////////////////////////////////////////////
#ifndef SERVER_H_
#include "pbj/server.h"
#endif

using namespace pbj;

/// \brief	The server instance.
Server* Server::_instance = 0;

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Server::init(I32 maxClients, U32 fps)
///
/// \brief	Initialises this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	maxClients	The maximum clients.
/// \param	fps		  	The FPS.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
bool Server::init(I32 maxClients, U32 fps)
{
	assert(!_instance);

	if(!Transport::init())
	{
		PBJ_LOG(pbj::VError) << "Failed to initialize transport layer"
								<< PBJ_LOG_END;
		return false;
	}
	
	_instance = new Server();

	_instance->_transport = Transport::create();

	if(!_instance->_transport)
	{
		PBJ_LOG(pbj::VError) << "Could not create transport" << PBJ_LOG_END;
		return false;
	}
	Transport::Config cfg = _instance->_transport->getConfig();
	cfg.maxNodes = maxClients;
	_instance->_transport->configure(cfg);
	PBJ_LOG(pbj::VInfo) << "Transport configuration:" << PBJ_LOG_NL
						<< "Mesh Port:\t"<< cfg.meshPort << PBJ_LOG_NL
						<< "Client Port:\t"<< cfg.clientPort << PBJ_LOG_NL
						<< "Server Port:\t"<< cfg.serverPort << PBJ_LOG_NL
						<< "Beacon Port:\t"<< cfg.beaconPort << PBJ_LOG_NL
						<< "Listener Port:\t"<< cfg.listenerPort << PBJ_LOG_NL
						<< "Proto Id:\t"<< cfg.protoId << PBJ_LOG_NL
						<< "Mesh Send Rate:\t"<< cfg.meshSendRate << PBJ_LOG_NL
						<< "Timeout:\t"<< cfg.timeout << PBJ_LOG_NL
						<< "Max Nodes:\t"<< cfg.maxNodes << PBJ_LOG_END;

	///< An enum constant representing the instance > dt option
	_instance->_dt = 1.0f/fps;
	U8 hostname[64+1] = "hostname";
	_instance->_transport->getHostName(hostname, sizeof(hostname));
	_instance->_transport->startServer(hostname);

	// Initialize game engine
   //pbj::Engine engine;
   return true;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	I32 Server::run()
///
/// \brief	Gets the run.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	.
////////////////////////////////////////////////////////////////////////////////
I32 Server::run()
{
	do
	{
		net::waitSeconds(_instance->_dt);
	}
	while(_instance->update());
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Server::stop()
///
/// \brief	Stops this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
void Server::stop()
{
	if(_instance != 0)
	{
		if(_instance->_transport != 0)
		{
			Transport::destroy(_instance->_transport);
			///< An enum constant representing the instance > transport option
			_instance->_transport = nullptr;
		}
		delete _instance;
		_instance = nullptr;
	}
	Transport::shutdown();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Server::Server()
///
/// \brief	Default constructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
Server::Server() :
	_transport(nullptr),
	_dt(0.0f),
	_sendMsgTimer(0.0f)
{}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Server::~Server()
///
/// \brief	Destructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
Server::~Server()
{}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Server::update()
///
/// \brief	Updates this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
bool Server::update()
{
	_transport->update(_dt);

	//first receive any information we might need
	I32 nodeId;
	U8 data[maxPacketSize];
	I32 bytesReceived = _transport->receivePacket(nodeId, data, maxPacketSize);
	if(bytesReceived != 0)
	{ //we actually got something!
		U32 protoId;
		net::readInteger(data, protoId);
		if(_transport->getConfig().protoId == protoId)
		{ //even better, it's the right protocol!
			//because this is just a test, I will ignore other checks.
			U8* msg = new U8[bytesReceived-4];
			strncpy_s((char*)msg, bytesReceived-4, (char*)(data+6), bytesReceived-4);
			std::cerr<<"Server: received message from "<<nodeId<<":\n\t"<<msg<<std::endl;
		}
	}


	_sendMsgTimer+=_dt;
	if(_sendMsgTimer >= 1.0f)
	{
		I32 n = _transport->getNumberConnected();
		for(I32 i=1;i<n;++i) //I don't like this
		{
			U8 packet[512];
			net::writeInteger(packet, _transport->getConfig().protoId);
			packet[4] = 0x02; //type = 2, which is nothing for the time being
			packet[5] = (U8)i;
			strcpy_s((char*)(packet+6),19+1,"This is just a test");
			if(_transport->sendPacket(i,packet,512))
				std::cerr<<"Sent packet to node: "<<i<<std::endl;
		}
		_sendMsgTimer -= 1.0f;
	}
	return true;
}