#ifndef NET_LAN_CONNECTION_H_
#define NET_LAN_CONNECTION_H_

#include "pbj/net/net_sockets.h"
#include "pbj/net/net_reliability.h"

namespace pbj
{
namespace net
{
	//virtual connection over udp
	//from sample marked for learning purposes only
	class Connection
	{
	public:
		enum Mode
		{
			None,
			Client,
			Server
		};

		Connection(U32, F32);
		virtual ~Connection();

		bool start(I32);
		void stop();

		bool isRunning() const;
		void listen();
		void connect(const Address&);
		bool isConnecting() const;
		bool connectFailed() const;
		bool isConnected() const;
		bool isListening() const;
		Connection::Mode getMode() const;
		
		virtual void update(F32);
		virtual bool sendPacket(const U8*, I32);
		virtual I32 receivePacket(U8*, I32);
		I32 getHeaderSize() const;

	protected:
		virtual void onStart();
		virtual void onStop();
		virtual void onConnect();
		virtual void onDisconnect();

	private:
		enum State
		{
			Disconnected,
			Listening,
			Connecting,
			ConnectFail,
			Connected
		};
		void clearData();

	};
}
}
#endif