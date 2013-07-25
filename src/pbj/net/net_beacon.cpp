#ifndef NET_LAN_BEACON_CPP
#include "pbj/net/net_beacon.h"
#endif

using namespace pbj;
using namespace net;

Beacon::Beacon(const char* name, U32 protoId, U32 listenerPort, U32 serverPort)
	: _socket(Socket::Broadcast | Socket::NonBlocking)
{
	std::strncpy((char*)_name, name, 64);
	_name[64] = '\0';
	_protoId = protoId;
	_listenerPort = listenerPort;
	_serverPort = serverPort;
	_running = false;
}

Beacon::~Beacon()
{
	if(_running)
		stop();
}

bool Beacon::start(I32 port)
{
	assert(!_running);
	PBJ_LOG(Verbosity::VInfo) << "Starting beacon on port " << port << PBJ_LOG_END;
	if(!_socket.open(port))
		return false;
	_running = true;
	return true;
}

void Beacon::stop()
{
	assert(_running);
	PBJ_LOG(Verbosity::VInfo) << "Stopping beacon" << PBJ_LOG_END;
	_socket.close();
	_running = false;
}

void Beacon::update(F32 dt)
{
	assert(_running);
	U8 packet[12+1+64];
	writeInteger(packet, 0);
	writeInteger(packet+4, _protoId);
	writeInteger(packet+8, _serverPort);
	packet[12] = (U8) strlen((const char*)_name);
	assert(packet[12]<63);
	memcpy(packet+13, _name, strlen((const char*)_name));
	if(!_socket.send(Address(255,255,255,255,_listenerPort), packet, 12+1+packet[12]))
		PBJ_LOG(Verbosity::VError) << "Failed to send broadcast packet" << PBJ_LOG_END;
	Address sender;
	while(_socket.receive(sender, packet, 256));
}