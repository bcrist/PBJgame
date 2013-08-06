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
#include <iostream>
#include <Windows.h>
#include "pbj/_pbj.h"

namespace pbj
{
namespace net
{
	////////////////////////////////////////////////////////////////////////////
	/// \fn	inline void waitSeconds(float n)
	///
	/// \brief	Wait seconds.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	///
	/// \param	n	The float to process.
	////////////////////////////////////////////////////////////////////////////
	inline void waitSeconds(float n)
	{
		Sleep( (int) ( n * 1000.0f ) );
	}

	////////////////////////////////////////////////////////////////////////////
	/// \fn	inline bool initSockets()
	///
	/// \brief	Initialises the sockets.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	///
	/// \return	true if it succeeds, false if it fails.
	////////////////////////////////////////////////////////////////////////////
	inline bool initSockets()
	{
		WSADATA wsaData;
		return WSAStartup(MAKEWORD(2,2), &wsaData) == NO_ERROR;
	}

	////////////////////////////////////////////////////////////////////////////
	/// \fn	inline void shutdownSockets()
	///
	/// \brief	Shutdown sockets.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	////////////////////////////////////////////////////////////////////////////
	inline void shutdownSockets()
	{
		WSACleanup();
	}

	////////////////////////////////////////////////////////////////////////////
	/// \fn	inline bool getHostName(U8* hostname, int size)
	///
	/// \brief	Gets host name.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	///
	/// \param [in,out]	hostname	If non-null, the hostname.
	/// \param	size				The size.
	///
	/// \return	true if it succeeds, false if it fails.
	////////////////////////////////////////////////////////////////////////////
	inline bool getHostName(U8* hostname, int size)
	{
		gethostname((char*)hostname, size);
		hostname[size-1] = '\0';
		return true;		// todo: proper return value check
	}

	////////////////////////////////////////////////////////////////////////////
	/// \fn	inline void writeByte(U8* data, U8 value)
	///
	/// \brief	Writes a byte.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	///
	/// \param [in,out]	data	If non-null, the data.
	/// \param	value			The value.
	////////////////////////////////////////////////////////////////////////////
	inline void writeByte(U8* data, U8 value) { *data = value; }

	////////////////////////////////////////////////////////////////////////////
	/// \fn	inline void readByte(const U8* data, U8& value)
	///
	/// \brief	Reads a byte.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	///
	/// \param	data		 	The data.
	/// \param [in,out]	value	The value.
	////////////////////////////////////////////////////////////////////////////
	inline void readByte(const U8* data, U8& value) { value =*data; }

	////////////////////////////////////////////////////////////////////////////
	/// \fn	inline void writeShort(U8* data, U16 value)
	///
	/// \brief	Writes a short.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	///
	/// \param [in,out]	data	If non-null, the data.
	/// \param	value			The value.
	////////////////////////////////////////////////////////////////////////////
	inline void writeShort(U8* data, U16 value)
	{
		data[0] = (U8)((value>>8) & 0xFF);
		data[1] = (U8)(value & 0xFF);
	}

	////////////////////////////////////////////////////////////////////////////
	/// \fn	inline void readShort(const U8* data, U16& value)
	///
	/// \brief	Reads a short.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	///
	/// \param	data		 	The data.
	/// \param [in,out]	value	The value.
	////////////////////////////////////////////////////////////////////////////
	inline void readShort(const U8* data, U16& value)
	{
		value = (((U32)(data[0])<<8)  | ((U32)(data[1])));
	}

	////////////////////////////////////////////////////////////////////////////
	/// \fn	inline void writeInteger(U8* data, U32 value)
	///
	/// \brief	Writes an integer.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	///
	/// \param [in,out]	data	If non-null, the data.
	/// \param	value			The value.
	////////////////////////////////////////////////////////////////////////////
	inline void writeInteger(U8* data, U32 value)
	{
		data[0] = (U8)(value>>24);
		data[1] = (U8)((value>>16) & 0xFF);
		data[2] = (U8)((value>>8) & 0xFF);
		data[3] = (U8)(value & 0xFF);
	}

	////////////////////////////////////////////////////////////////////////////
	/// \fn	inline void readInteger(const U8* data, U32& value)
	///
	/// \brief	Reads an integer.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	///
	/// \param	data		 	The data.
	/// \param [in,out]	value	The value.
	////////////////////////////////////////////////////////////////////////////
	inline void readInteger(const U8* data, U32& value)
	{
		value = (((I32)data[0]<<24) | ((I32)data[1]<<16) | 
				  ((I32)data[2]<<8)  | ((I32)data[3]));		
	}

	////////////////////////////////////////////////////////////////////////////
	/// \struct	HexCharStruct
	///
	/// \brief	Hexadecimal character structure.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	/// \details This is for debug output on data received on the socket.  I
	/// 		 grabbed this from
	/// 		 stackoverflow.com/questions/673240/how-do-i-print-an-unsigned-char-as-hex-in-c-using-ostream
	////////////////////////////////////////////////////////////////////////////
	struct HexCharStruct
	{
		U8 c;
		HexCharStruct(U8 _c) : c(_c) {}
	};

	////////////////////////////////////////////////////////////////////////////
	/// \fn	inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
	///
	/// \brief	Stream insertion operator.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	///
	/// \param [in,out]	o	The std::ostream&amp; to process.
	/// \param	hs		 	The hs.
	///
	/// \return	The shifted result.
	////////////////////////////////////////////////////////////////////////////
	inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
	{
		return (o << std::hex << (int)hs.c);
	}

	////////////////////////////////////////////////////////////////////////////
	/// \fn	inline HexCharStruct hex(U8 _c)
	///
	/// \brief	Hexadecimals the given c.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	///
	/// \param	_c	The c.
	///
	/// \return	.
	////////////////////////////////////////////////////////////////////////////
	inline HexCharStruct hex(U8 _c)
	{
		return HexCharStruct(_c);
	}
} //namespace net
} //namespace pbj
#endif