#ifndef NET_NODE_MESH_H_
#define NET_NODE_MESH_H

#include <assert.h>
#include <vector>
#include <map>
#include <stack>
#include "pbj/_pbj.h"
#include "pbj/net/net_sockets.h"
#include "pbj/net/net_reliability.h"

using std::vector;
using std::map;
using std::stack;

namespace pbj
{
namespace net
{
	class NetMesh
	{
	public:
		NetMesh(U32, I32, F32, F32);
		NetMesh(U32, I32, F32);
		NetMesh(U32, I32);
		NetMesh(U32);
		~NetMesh();

		bool start(I32);
		void stop();
		void update(F32);
		bool isNodeConnected(I32);
		Address getNodeAddress(I32);
		I32 getMaxNodes() const;
		void reserve(I32, const Address&);

	protected:
		void receivePackets();
		void processPacket(const Address&, U8*, I32);
		void sendPackets(F32);
		void checkForTimeouts(F32);

	private:
		struct NodeState
		{
			enum Mode { Disconnected, ConnectionAccept, Connected };
			Mode mode;
			F32 timeoutAccumulator;
			Address address;
			I32 nodeId;
			
			NodeState()
			{
				mode = Disconnected;
				address = Address();
				nodeId = -1;
				timeoutAccumulator = 0.0f;
			}
		};

		U32 _protoId;
		F32 _sendRate;
		F32 _timeout;
		
		Socket _socket;
		vector<NodeState> _nodes;
		typedef map<I32,NodeState*> IdToNode;
		typedef map<Address,NodeState*> AddrToNode;
		IdToNode _idToNode;
		AddrToNode _addrToNode;
		bool _running;
		F32 _sendAccumulator;


	};
}
}
#endif