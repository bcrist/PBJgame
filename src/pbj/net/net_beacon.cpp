#ifndef NET_LAN_BEACON_CPP
#include "pbj/net/net_beacon.h"
#endif

using namespace pbj;
using namespace net;

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

Beacon::~Beacon()
{
	if(_running)
		stop();
}

bool Beacon::start(U16 port)
{
	assert(!_running);
	PBJ_LOG(pbj::VInfo) << "Starting beacon on port " << port << PBJ_LOG_END;
	if(!_socket.open(port))
		return false;
	_running = true;
	return true;
}

void Beacon::stop()
{
	assert(_running);
	PBJ_LOG(pbj::VInfo) << "Stopping beacon" << PBJ_LOG_END;
	_socket.close();
	_running = false;
}

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