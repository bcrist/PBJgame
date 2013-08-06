#ifndef TRANSPORT_LAN_H_
#define TRANSPORT_LAN_H_

#include "pbj/net/net_transport.h"

namespace pbj
{
namespace net
{
	class TransportLAN : public Transport
	{
		static bool init();
		static void shutdown();
		static bool getHostName(U8*, int);
		static void unitTest();
		
		struct Config
		{
			U16 meshPort;
			U16 serverPort;
			U16 clientPort;
			U16 beaconPort;
			U16 listenerPort;
			U32 protoId;
			F32 meshSendRate;
			F32 timeout;
			I32 maxNodes;

			Config()
			{
				meshPort = 30000;
				clientPort = 30001;
				serverPort = 30002;
				beaconPort = 40000;
				listenerPort = 40001;
				protoId = 0x47464750;
				meshSendRate = 0.25f;
				timeout = 10.0f;
				maxNodes = 4;
			}
		};

		struct LobbyEntry
		{
			U8 name[65];
			U8 address[65];
		};

		TransportLAN();
		~TransportLAN();

		void configure(Config&);
		const Config& getConfig() const;
		bool startServer(const U8* const);
		bool connectClient(const U8* const);
		bool isConnected() const;
		bool connectFailed() const;
		bool enterLobby();
		I32 getLobbyEntryCount();
		bool getLobbyEntryAtIndex(I32, LobbyEntry&);
		void stop();

		virtual bool isNoneConnected(int);
		virtual int getLocalNodeID() const;
		virtual int getMaxNodes() const;
		virtual bool sendPacket(int, const U8* const, int);
		virtual int receivePacket(int&, U8*, int);
		virtual class ReliabilitySystem& getReliability(int);
		virtual void update(F32);
		virtual TransportType getType() const;

	private:
		Config _config;
		class Mesh* _mesh;
		class Node* _node;
		class Beacon* _beacon;
		class Listener* _listener;
		F32 _beaconAccumulator;
		bool _connectingByName;
		char _connectName[65];
		F32 _connectAccumulator;
		bool _connectFailed;
	};
}
}
#endif