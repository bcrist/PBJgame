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
		ReliableConnection(U32, F32, U32)
		~ReliableConnection()

		// overriden functions from "Connection"
		bool SendPacket(const U8* const, I32);
		I32 ReceivePacket(U8*, I32);
		void Update(F32);
		I32 GetHeaderSize() const;
		ReliabilitySystem& GetReliabilitySystem();

	protected:
		void WriteHeader(U8*, U32, U32, U32);
		void ReadHeader(const U8*, U32&, U32&, U32&);
		virtual void OnStop();
		virtual void OnDisconnect();

	private:
		void clearData();

		ReliabilitySystem _reliabilitySystem;
	};
} //namespace net
} //namespace pbj
#endif