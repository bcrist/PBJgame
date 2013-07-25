#ifndef NET_SOCKETS_H_
#define NET_SOCKETS_H_

#include <WinSock2.h>
#pragma comment(lib, "wsock32.lib")

#include <assert.h>
#include <iostream>
#include <Windows.h>
#include "pbj/net/net_platform.h"
#include "pbj/net/net_address.h"

namespace pbj
{
namespace net
{
	inline bool initSockets()
	{
		WSADATA wsaData;
		return WSAStartup(MAKEWORD(2,2), &wsaData) == NO_ERROR;
	}

	inline void shutdownSockets()
	{
		WSACleanup();
	}

	// get host name helper
	inline bool getHostName(U8* hostname, int size)
	{
		gethostname((char*)hostname, size);
		hostname[size-1] = '\0';
		return true;		// todo: proper return value check
	}

	// helper functions for reading and writing integer values to packets

	inline void writeByte(U8* data, U8 value) { *data = value; }

	inline void readByte(const U8* data, U8& value) { value =*data; }

	inline void writeShort(U8* data, U16 value)
	{
		data[0] = (U8)((value>>8) & 0xFF);
		data[1] = (U8)(value & 0xFF);
	}

	inline void readShort(const U8* data, U16& value)
	{
		value = (((I32)data[0]<<8)  | ((I32)data[1]));
	}

	inline void writeInteger(U8* data, U32 value)
	{
		data[0] = (U8)(value>>24);
		data[1] = (U8)((value>>16) & 0xFF);
		data[2] = (U8)((value>>8) & 0xFF);
		data[3] = (U8)(value & 0xFF);
	}

	inline void readInteger(const U8* data, U32& value)
	{
		value = (((I32)data[0]<<24) | ((I32)data[1]<<16) | 
				  ((I32)data[2]<<8)  | ((I32)data[3]));		
	}

	class Socket
	{
	public:
		enum Options
		{
			NonBlocking=0x01,
			Broadcast = 0x02
		};

		Socket();
		Socket(I32);
		~Socket();

		bool open(U16);
		void close();
		bool isOpen() const;
		bool send(const Address&, const void*, I32);
		I32 receive(Address&, void*, I32);
	private:
		I32 _socket;
		I32 _options;
	};
} //namespace net
} //namespace pbj
#endif