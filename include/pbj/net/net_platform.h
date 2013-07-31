/*
	Simple Network Library from "Networking for Game Programmers"
	http://www.gaffer.org/networking-for-game-programmers
	Author: Glenn Fiedler <gaffer@gaffer.org>
*/

#ifndef NET_PLATFORM_H_
#define NET_PLATFORM_H_

#include <WinSock2.h>
#pragma comment(lib, "wsock32.lib")

#include <Windows.h>
#include <assert.h>
#include <stdio.h>
#include <Windows.h>
#include "pbj/_pbj.h"

namespace pbj
{
namespace net
{
	/// \b wait for n seconds
	inline void waitSeconds(float n)
	{
		Sleep( (int) ( n * 1000.0f ) );
	}

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
} //namespace net
} //namespace pbj
#endif