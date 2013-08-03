#ifndef NET_SOCKETS_H_
#include "pbj/net/net_sockets.h"
#endif

using namespace pbj;
using namespace pbj::net;

Socket::Socket()
{
	_options = Options::NonBlocking;
	_socket = 0;
}

Socket::Socket(I32 options)
{
	_options = options;
	_socket = 0;
}

Socket::~Socket()
{
	close();
}

bool Socket::open(U16 port)
{
	assert(!isOpen());

	//create the socket
	_socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(_socket <= 0)
	{
		PBJ_LOG(pbj::VError) << "Failed to create socket" << PBJ_LOG_END;
		_socket = 0;
		return false;
	}

	//bind to port
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	if(bind(_socket, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0)
	{
		PBJ_LOG(pbj::VError) << "Failed to bind socket" << PBJ_LOG_END;
		close();
		return false;
	}

	//set non-blocking io, if the option is set
	if(_options & NonBlocking)
	{
		DWORD nonBlocking = 1;
		if(ioctlsocket(_socket, FIONBIO, &nonBlocking) != 0)
		{
			PBJ_LOG(pbj::VError) << "Failed to set non-blocking socket" << PBJ_LOG_END;
			close();
			return false;
		}
	}

	//set broadcast socket, if option is set
	if(_options & Broadcast)
	{
		I32 enable = 1;
		if(setsockopt(_socket, SOL_SOCKET, SO_BROADCAST, (const char*)&enable, sizeof(enable)) < 0)
		{
			PBJ_LOG(pbj::VError) << "Failed set socket to broadcast" << PBJ_LOG_END;
			close();
			return false;
		}
	}

	return true;
}

void Socket::close()
{
	if(_socket != 0)
	{
		closesocket(_socket);
		_socket = 0;
	}
}

bool Socket::isOpen() const { return _socket != 0; }

bool Socket::send(const Address& dest, const void* data, I32 size)
{
	assert(data);
	assert(size>0);

	if(socket==0)
		return false;

	assert(dest.getAddress() != 0);
	assert(dest.getPort() != 0);

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(dest.getAddress());
	address.sin_port = htons(dest.getPort());

	I32 sentBytes = sendto(_socket, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in));
	return sentBytes == size;
}

int Socket::receive(Address& sender, void* data, int size)
{
	assert(data);
	assert(size>0);

	if(_socket==0)
		return false;

	sockaddr_in from;
	I32 fromLength = sizeof(from);
	I32 receivedBytes = recvfrom(_socket, (char*)data, size, 0, (sockaddr*)&from, &fromLength);
	if(receivedBytes <= 0)
		return 0;
	/* Only use this for checking socket reception of data
	std::cerr<<"Socket: Recieved more than 0 bytes, printing bytes received"<<std::endl<<"\t";
	for(int i=0;i<receivedBytes;++i)
	{
		U8 c = ((U8*)data)[i];
		std::cerr<<hex(c)<<" ";
		if(i % 16 == 15)
			std::cerr<<std::endl<<"\t";
	}
	std::cerr<<std::endl;*/

	U32 address = ntohl(from.sin_addr.s_addr);
	U16 port = (U16)ntohs(from.sin_port);

	sender = Address(address, port);
	
	return receivedBytes;

}