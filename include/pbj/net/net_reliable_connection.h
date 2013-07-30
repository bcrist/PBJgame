#ifndef RELIABLE_CONNECTION_H_
#define RELIABLE_CONNECTION_H_

#include "pbj/_pbj.h"
#include "pbj/net/net_sockets.h"
#include "pbj/net/net_connection.h"
#include "pbj/net/net_reliability.h"

namespace pbj
{
namespace net
{
	class ReliableConnection : public Connection
	{
	public:
		ReliableConnection(U32, F32, U32);
		~ReliableConnection();

		// overriden functions from "Connection"
		bool sendPacket(const U8* const, I32);
		I32 receivePacket(U8*, I32);
		void update(F32);
		I32 getHeaderSize() const;
		ReliabilitySystem& getReliabilitySystem();

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