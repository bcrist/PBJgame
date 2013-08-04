/*
	Simple Network Library from "Networking for Game Programmers"
	http://www.gaffer.org/networking-for-game-programmers
	Author: Glenn Fiedler <gaffer@gaffer.org>
*/

#ifndef NET_TRANSPORT_H_
#define NET_TRANSPORT_H_

#include <assert.h>
#include <iostream>
#include <string>
#include <vector>
#include "pbj/_pbj.h"
#include "pbj/net/net_address.h"
#include "pbj/net/net_sockets.h"
#include "pbj/net/net_beacon.h"
#include "pbj/net/net_connection.h"
#include "pbj/net/net_node_mesh.h"
#include "pbj/net/net_node.h"
#include "pbj/net/net_reliability.h"
#include "pbj/net/net_listener.h"

namespace pbj
{
namespace net
{
	class Transport
	{
	public:
		static bool init();
		static void shutdown();
		static Transport* create();
		static void destroy(Transport*);
		static bool getHostName(U8*, int);

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
				maxNodes = 16;
			}
		};

		struct LobbyEntry
		{
			U8 name[65];
			U8 address[65];
		};

		~Transport();

		void configure(Config&);
		const Config& getConfig() const;
		bool startServer(const U8* const);
		bool connectClient(const U8* const);
		bool connectClient(const Address&);
		bool isConnected() const;
		bool connectFailed() const;
		bool enterLobby();
		bool leaveLobby();
		I32 getLobbyEntryCount();
		bool getLobbyEntryAtIndex(I32, LobbyEntry&);
		I32 getNumberConnected() const;
		void stop();

		bool isNoneConnected(I32);
		I32 getLocalNodeID() const;
		I32 getMaxNodes() const;
		bool sendPacket(I32, const U8* const, I32);
		I32 receivePacket(I32&, U8*, I32);
		ReliabilitySystem& getReliability(I32);
		void update(F32);

	private:
		static int transportCount;

		Transport();

		Config _config;
		class NetMesh* _mesh;
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
