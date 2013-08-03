#ifndef NET_NODE_H_
#include "pbj/net/net_node.h"
#endif

using namespace pbj;
using namespace pbj::net;

Node::Node(U32 protocolId)
{
	_protoId = protocolId;
	_sendRate = 0.25f;
	_timeout = 10.0f;
	_maxPacketSize = 1024;
	_state = Disconnected;
	_running = false;
	clearData();
}

Node::Node(U32 protocolId, F32 sendRate)
{
	_protoId = protocolId;
	_sendRate = sendRate;
	_timeout = 10.0f;
	_maxPacketSize = 1024;
	_state = Disconnected;
	_running = false;
	clearData();
}

Node::Node(U32 protocolId, F32 sendRate, F32 timeout)
{
	_protoId = protocolId;
	_sendRate = sendRate;
	_timeout = timeout;
	_maxPacketSize = 1024;
	_state = Disconnected;
	_running = false;
	clearData();

}

Node::Node(U32 protocolId, F32 sendRate, F32 timeout, I32 maxPacketSize)
{
	_protoId = protocolId;
	_sendRate = sendRate;
	_timeout = timeout;
	_maxPacketSize = maxPacketSize;
	_state = Disconnected;
	_running = false;
	clearData();
}
Node::~Node()
{
	if(_running)
		stop();
}

bool Node::start(I32 port)
{
	assert(!_running);
	PBJ_LOG(pbj::VInfo) << "Start node on port " << port << PBJ_LOG_END;
	if(!_socket.open(port))
		return false;
	_running = true;
	return true;
}

void Node::stop()
{
	assert(_running);
	printf("stop node\n");
	clearData();
	_socket.close();
	_running = false;
}	

void Node::join(const Address& address)
{
	PBJ_LOG(pbj::VInfo) << "Node join "
						<< (U32) address.getA() << "."
						<< (U32) address.getB() << "."
						<< (U32) address.getC() << "."
						<< (U32) address.getD() << ":"
						<< (U32) address.getPort() << PBJ_LOG_END;
	clearData();
	_state = Joining;
	_meshAddress = address;
}

bool Node::isJoining() const
{
	return _state == Joining;
}

bool Node::joinFailed() const
{
	return _state == JoinFail;
}

bool Node::isConnected() const
{
	return _state == Joined;
}

I32 Node::getLocalNodeId() const
{
	return _localNodeId;
}

void Node::update(F32 dt)
{
	assert(_running);
	receivePackets();
	sendPackets(dt);
	checkForTimeout(dt);
}

bool Node::isNodeConnected(I32 nodeId)
{
	assert(nodeId >= 0);
	assert(nodeId < (int)_nodes.size());
	return _nodes[nodeId].connected;
}

Address Node::getNodeAddress(I32 nodeId)
{
	assert(nodeId >= 0);
	assert(nodeId < (int)_nodes.size());
	return _nodes[nodeId].address;
}

I32 Node::getMaxNodes() const
{
	assert(_nodes.size() <= 255);
	return _nodes.size();
}

bool Node::sendPacket(I32 nodeId, const U8* const data, I32 size)
{
	assert(_running);
	if (_nodes.size() == 0)
		return false;	// not connected yet
	assert(nodeId >= 0);
	assert(nodeId < (int)_nodes.size());
	if (nodeId < 0 || nodeId >= (int)_nodes.size())
		return false;
	if (!_nodes[nodeId].connected)
		return false;
	assert(size <= _maxPacketSize);
	if (size > _maxPacketSize)
		return false;
	return _socket.send(_nodes[nodeId].address, data, size);
}

I32 Node::receivePacket(I32& nodeId, U8* data, I32 size)
{
	assert(_running);
	if (!_receivedPackets.empty())
	{
		BufferedPacket* packet = _receivedPackets.top();
		assert(packet);
		if ((int)packet->data.size() <= size)
		{
			nodeId = packet->nodeId;
			memcpy(data, &packet->data[0], packet->data.size());
			delete packet;
			_receivedPackets.pop();
			return packet->data.size();
		}
		else
		{
			delete packet;
			_receivedPackets.pop();
		}
	}
	return 0;
}

void Node::receivePackets()
{
	U8* data = new U8[_maxPacketSize];
	while (true)
	{
		Address sender;
		int size = _socket.receive(sender, data, _maxPacketSize*sizeof(data[0]));
		if (!size)
			break;
		processPacket(sender, data, size);
	}
	delete[] data;
}

void Node::processPacket(const Address& sender, U8* data, int size)
{
	assert(sender != Address());
	assert(size > 0);
	assert(data);
	
	// is packet from the mesh?
	if (sender == _meshAddress)
	{
		// *** packet sent from the mesh ***
		// ignore packets that dont have the correct protocol id
		unsigned int firstIntegerInPacket = (unsigned(data[0]) << 24) | (unsigned(data[1]) << 16) |
											(unsigned(data[2]) << 8)  | unsigned(data[3]);
		if (firstIntegerInPacket != _protoId)
			return;
		// determine packet type
		enum PacketType { ConnectionAccepted, Update };
		PacketType packetType;
		std::cerr<<"Node: Packet type received (should be 0 or 1): "<<(I32)(data[4])<<std::endl;
		if (data[4] == 0)
			packetType = ConnectionAccepted;
		else if (data[4] == 1)
			packetType = Update;
		else
			return;
		// handle packet type
		switch (packetType)
		{
			case ConnectionAccepted:
			{
				if (size != 7)
					return;
				if (_state == Joining)
				{
					_localNodeId = data[5];
					_nodes.resize(data[6]);
					printf("node accepts join as node %d of %d\n",_localNodeId, (int)_nodes.size());
					_state = Joined;
				}
				_timeoutAccumulator = 0.0f;
			}
			break;
			case Update:
			{
				if (size != (int)(5 + _nodes.size() * 6))
					return;
				if (_state == Joined)
				{
					// process update packet
					unsigned char * ptr = &data[5];
					for (unsigned int i = 0; i < _nodes.size(); ++i)
					{
						unsigned char a = ptr[0];
						unsigned char b = ptr[1];
						unsigned char c = ptr[2];
						unsigned char d = ptr[3];
						unsigned short port = (unsigned short)ptr[4] << 8 | (unsigned short)ptr[5];
						Address address(a, b, c, d, port);
						if (address.getAddress() != 0)
						{
							// node is connected
							if (address != _nodes[i].address)
							{
								printf("node %d: node %d connected\n", _localNodeId, i);
								_nodes[i].connected = true;
								_nodes[i].address = address;
								_addrToNode[address] = &_nodes[i];
							}
						}
						else
						{
							// node is not connected
							if(_nodes[i].connected)
							{
								printf("node %d: node %d disconnected\n", _localNodeId, i);
								AddrToNode::iterator itor = _addrToNode.find(_nodes[i].address);
								assert(itor != _addrToNode.end());
								_addrToNode.erase(itor);
								_nodes[i].connected = false;
								_nodes[i].address = Address();
							}
						}
						ptr += 6;
					}
				}
				_timeoutAccumulator = 0.0f;
			}
			break;
		}
	}
	else
	{
		AddrToNode::iterator itor = _addrToNode.find(sender);
		if (itor != _addrToNode.end())
		{
			// *** packet sent from another node ***
			NodeState* node = itor->second;
			assert(node);
			int nodeId = (int)(node - &_nodes[0]);
			assert(nodeId >= 0);
			assert(nodeId < (int)_nodes.size());
			BufferedPacket * packet = new BufferedPacket;
			packet->nodeId = nodeId;
			packet->data.resize(size);
			memcpy(&packet->data[0], data, size);
			_receivedPackets.push(packet);
		}
	}
}

void Node::sendPackets(F32 dt)
{
	_sendAccumulator += dt;
	while (_sendAccumulator > _sendRate)
	{
		if (_state == Joining)
		{
			// node is joining: send "join request" packets
			unsigned char packet[5];
			packet[0] = (unsigned char) ((_protoId >> 24) & 0xFF);
			packet[1] = (unsigned char) ((_protoId >> 16) & 0xFF);
			packet[2] = (unsigned char) ((_protoId >> 8) & 0xFF);
			packet[3] = (unsigned char) ((_protoId) & 0xFF);
			packet[4] = 0;
			_socket.send(_meshAddress, packet, sizeof(packet));
		}
		else if(_state == Joined)
		{
			// node is joined: send "keep alive" packets
			unsigned char packet[5];
			packet[0] = (unsigned char) ((_protoId >> 24) & 0xFF);
			packet[1] = (unsigned char) ((_protoId >> 16) & 0xFF);
			packet[2] = (unsigned char) ((_protoId >> 8) & 0xFF);
			packet[3] = (unsigned char) ((_protoId) & 0xFF);
			packet[4] = 1;
			_socket.send(_meshAddress, packet, sizeof(packet));
		}
		_sendAccumulator -= _sendRate;
	}
}

void Node::checkForTimeout(F32 dt)
{
	if(_state == Joining || _state == Joined)
	{
		_timeoutAccumulator += dt;
		if (_timeoutAccumulator > _timeout)
		{
			if (_state == Joining)
			{
				printf("node join failed\n");
				_state = JoinFail;
			}
			else
			{
				printf("node timed out\n");
				_state = Disconnected;
			}
			clearData();
		}
	}
}

void Node::clearData()
{
	_nodes.clear();
	_addrToNode.clear();
	while (!_receivedPackets.empty())
	{
		BufferedPacket * packet = _receivedPackets.top();
		delete packet;
		_receivedPackets.pop();
	}
	_sendAccumulator = 0.0f;
	_timeoutAccumulator = 0.0f;
	_localNodeId = -1;
	_meshAddress = Address();
}