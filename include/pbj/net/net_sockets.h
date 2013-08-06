////////////////////////////////////////////////////////////////////////////////
/// \file	Z:\Documents\PBJgame\include\pbj\net\net_sockets.h
///
/// \brief	Declares the net sockets class.
////////////////////////////////////////////////////////////////////////////////
#ifndef NET_SOCKETS_H_
#define NET_SOCKETS_H_

#include <assert.h>
#include <iostream>
#include "pbj/net/net_platform.h"
#include "pbj/net/net_address.h"

namespace pbj
{
namespace net
{
	////////////////////////////////////////////////////////////////////////////
	/// \class	Socket
	///
	/// \brief	Socket.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	////////////////////////////////////////////////////////////////////////////
	class Socket
	{
	public:

		////////////////////////////////////////////////////////////////////////
		/// \enum	Options
		///
		/// \brief	Values that represent Options.
		////////////////////////////////////////////////////////////////////////
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
		U32 localIP();
	private:
		I32 _socket;
		U16 _port;
		I32 _options;
	};
} //namespace net
} //namespace pbj
#endif