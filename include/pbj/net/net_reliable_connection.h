////////////////////////////////////////////////////////////////////////////////
/// \file	Z:\Documents\PBJgame\include\pbj\net\net_reliable_connection.h
///
/// \brief	Declares the net reliable connection class.
////////////////////////////////////////////////////////////////////////////////
#ifndef NET_RELIABLE_CONNECTION_H_
#define NET_RELIABLE_CONNECTION_H_

#include "pbj/_pbj.h"
#include "pbj/net/net_sockets.h"
#include "pbj/net/net_connection.h"
#include "pbj/net/net_reliability.h"

namespace pbj
{
namespace net
{
	////////////////////////////////////////////////////////////////////////////
	/// \class	ReliableConnection
	///
	/// \brief	Reliable connection.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	////////////////////////////////////////////////////////////////////////////
	class ReliableConnection : public Connection
	{
	public:
		ReliableConnection(U32, F32, U32);
		~ReliableConnection();

		// overriden functions from "Connection"
		virtual bool sendPacket(const U8* const, I32);
		virtual I32 receivePacket(U8*, I32);
		virtual void update(F32);
		virtual I32 getHeaderSize() const;
		virtual ReliabilitySystem& getReliabilitySystem();

	protected:
		void writeHeader(U8*, U32, U32, U32);
		void readHeader(const U8*, U32&, U32&, U32&);
		virtual void onStop();
		virtual void onDisconnect();

	private:
		void clearData();

		ReliabilitySystem _reliabilitySystem;
	};
} //namespace net
} //namespace pbj
#endif