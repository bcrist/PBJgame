////////////////////////////////////////////////////////////////////////////////
/// \file	Z:\Documents\PBJgame\include\pbj\net\net_node_mesh.h
///
/// \brief	Declares the net node mesh class.
////////////////////////////////////////////////////////////////////////////////
#ifndef NET_NODE_MESH_H_
#define NET_NODE_MESH_H_

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
	////////////////////////////////////////////////////////////////////////////
	/// \class	NetMesh
	///
	/// \brief	Net mesh.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	////////////////////////////////////////////////////////////////////////////
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
		I32 getNumberConnected() const;
		void reserve(I32, const Address&);

	protected:
		void receivePackets();
		void processPacket(const Address&, U8*, I32);
		void sendPackets(F32);
		void checkForTimeouts(F32);

	private:

		////////////////////////////////////////////////////////////////////////
		/// \struct	NodeState
		///
		/// \brief	Node state.
		///
		/// \author	Peter Bartosch
		/// \date	2013-08-05
		////////////////////////////////////////////////////////////////////////
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

		////////////////////////////////////////////////////////////////////////
		/// \typedef	map<I32,NodeState*> IdToNode
		///
		/// \brief	Defines an alias representing the identifier to node.
		////////////////////////////////////////////////////////////////////////
		typedef map<I32,NodeState*> IdToNode;

		////////////////////////////////////////////////////////////////////////
		/// \typedef	map<Address,NodeState*> AddrToNode
		///
		/// \brief	Defines an alias representing the address to node.
		////////////////////////////////////////////////////////////////////////
		typedef map<Address,NodeState*> AddrToNode;
		IdToNode _idToNode;
		AddrToNode _addrToNode;
		bool _running;
		F32 _sendAccumulator;


	};
} //namespace net
} //namespace pbj
#endif