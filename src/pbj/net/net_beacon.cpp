///////////////////////////////////////////////////////////////////////////////
/// \file	Z:\Documents\PBJgame\src\pbj\net\net_beacon.cpp
/// \author	Peter Bartosch
/// \date	2013-08-05
/// \brief	Implements the net beacon class.
///////////////////////////////////////////////////////////////////////////////

#ifndef NET_LAN_BEACON_CPP
#include "pbj/net/net_beacon.h"
#endif

using namespace pbj;
using namespace net;

///////////////////////////////////////////////////////////////////////////////
/// \fn	Beacon::Beacon(const U8* const name, U32 protoId, U16 listenerPort,
/// 	U16 serverPort) : _socket(Socket::Broadcast | Socket::NonBlocking)
///
/// \brief	Constructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	name			The name.
/// \param	protoId			Identifier for the protocol.
/// \param	listenerPort	The listener port.
/// \param	serverPort  	The server port.
///////////////////////////////////////////////////////////////////////////////
Beacon::Beacon(const U8* const name, U32 protoId, U16 listenerPort, U16 serverPort)
	: _socket(Socket::Broadcast | Socket::NonBlocking)
{
	strncpy_s((char*)_name, 65, (const char*)name, 64);
	_name[64] = '\0';
	_protoId = protoId;
	_listenerPort = listenerPort;
	_serverPort = serverPort;
	_running = false;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn	Beacon::~Beacon()
///
/// \brief	Destructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///////////////////////////////////////////////////////////////////////////////
Beacon::~Beacon()
{
	if(_running)
		stop();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn	bool Beacon::start(U16 port)
///
/// \brief	Starts the Beacon on the given port.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	port	The port on which the Beacon will transmit.
///
/// \return	true if it succeeds, false if it fails.
///////////////////////////////////////////////////////////////////////////////
bool Beacon::start(U16 port)
{
	assert(!_running);
	PBJ_LOG(pbj::VInfo) << "Starting beacon on port " << port << PBJ_LOG_END;
	if(!_socket.open(port))
		return false;
	_running = true;
	return true;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn	void Beacon::stop()
///
/// \brief	Stops this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///////////////////////////////////////////////////////////////////////////////
void Beacon::stop()
{
	assert(_running);
	PBJ_LOG(pbj::VInfo) << "Stopping beacon" << PBJ_LOG_END;
	_socket.close();
	_running = false;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn	void Beacon::update(F32 dt)
///
/// \brief	Updates using the given dt.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	dt	The change in time (delta time).
///////////////////////////////////////////////////////////////////////////////
void Beacon::update(F32 dt)
{
	//std::cerr<<"Beacon: starting update with dt: "<<dt<<std::endl;
	assert(_running);
	U8 packet[12+1+64];

	writeInteger(packet, 0);

	writeInteger(packet+4, _protoId);

	//std::cerr<<"\tWriting server port:"<<_serverPort<<std::endl;
	writeShort(packet+8, _serverPort);
	//add padding?
	writeShort(packet+10,0);

	size_t len = strlen((const char*)_name);
	assert(len<63);
	packet[12] = (U8)len;
	memcpy(packet+13, _name, len);
	/*std::cerr<<"\tAttempting to send packet:\n\t";
	for(U32 i=0;i<12+len;++i)
	{
		std::cerr<<hex(packet[i])<<" ";
		if(i%16==15)
			std::cerr<<"\n\t";
	}
	std::cerr<<std::endl;*/

	if(!_socket.send(Address(255,255,255,255,_listenerPort), packet, 12+1+packet[12]))
		PBJ_LOG(pbj::VError) << "Failed to send broadcast packet" << PBJ_LOG_END;
	
	//std::cerr<<"\tSent packet"<<std::endl;
	Address sender;
	while(_socket.receive(sender, packet, 256));
	//std::cerr<<"Beacon: leaving update"<<std::endl;
}