#ifndef NET_NODE_H_
#define NET_NODE_H_

#include <stack>
#include <map>
#include <vector>
#include "pbj/_pbj.h"
#include "pbj/net/net_address.h"
#include "pbj/net/net_sockets.h"

using std::stack;
using std::map;
using std::vector;

namespace pbj
{
namespace net
{
	class Node
	{
	public:
		Node(U32);
		Node(U32, F32);
		Node(U32, F32, F32);
		Node(U32, F32, F32, I32);
		~Node();
		bool start(I32);
		void stop();
		void join(const Address&);
		bool isJoining() const;
		bool joinFailed() const;
		bool isConnected() const;
		I32 getLocalNodeId() const;
		void update(F32);
		bool isNodeConnected(I32);
		Address getNodeAddress(I32);
		I32 getMaxNodes() const;
		bool sendPacket(I32, const U8* const, I32);
		I32 receivePacket(I32&, U8*, I32);

	protected:
		void receivePackets();
		void processPacket(const Address&, U8*, I32);
		void sendPackets(F32);
		void checkForTimeout(F32);
		void clearData();

	private:
		struct NodeState
		{
			bool connected;
			Address address;
			NodeState()
			{
				connected = false;
				address = Address();
			}
		};

		struct BufferedPacket
		{
			I32 nodeId;
			vector<U8> data;
		};
		
		enum State
		{
			Disconnected,
			Joining,
			Joined,
			JoinFail
		};

		typedef stack<BufferedPacket*> PacketBuffer;
		typedef map<I32, NodeState*> IdToNode;
		typedef map<Address, NodeState*> AddrToNode;

		PacketBuffer _receivedPackets;
		U32 _protoId;
		F32 _sendRate;
		F32 _timeout;
		I32 _maxPacketSize;
		Socket _socket;
		vector<NodeState> _nodes;
		AddrToNode _addrToNode;
		bool _running;
		F32 _sendAccumulator;
		F32 _timeoutAccumulator;
		State _state;
		Address _meshAddress;
		I32 _localNodeId;
	};
} //namespace net
} //namespace pbj
#endif