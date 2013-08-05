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
	_socket.localIP();
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
	PBJ_LOG(pbj::VInfo) << "Node join " << address << PBJ_LOG_END;
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

Address Node::getMeshAddress() { return _meshAddress; }
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
	if(_nodes.size() == 0)
		return false;	// not connected yet
	assert(nodeId >= 0);
	assert(nodeId < (int)_nodes.size());
	if(nodeId < 0 || nodeId >= (int)_nodes.size())
		return false;
	if(!_nodes[nodeId].connected)
		return false;
	assert(size <= _maxPacketSize);
	if(size > _maxPacketSize)
		return false;
	return _socket.send(_nodes[nodeId].address, data, size);
}

I32 Node::receivePacket(I32& nodeId, U8* data, I32 size)
{
	assert(_running);
	if(!_receivedPackets.empty())
	{
		BufferedPacket* packet = _receivedPackets.top();
		assert(packet);
		if((int)packet->data.size() <= size)
		{
			nodeId = packet->nodeId;
			memcpy(data, &packet->data[0], packet->data.size());
			I32 ret = packet->data.size();
			delete packet;
			_receivedPackets.pop();
			return ret;
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
		if(!size)
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
	if(sender == _meshAddress || sender.getPort() == 30000)
	{
		// *** packet sent from the mesh ***
		// ignore packets that dont have the correct protocol id
		U32 firstIntegerInPacket;
		readInteger(data,firstIntegerInPacket);
		if(firstIntegerInPacket != _protoId)
			return;
		// handle packet type
		switch(data[4])
		{
			case 0: //connection accepted
			{
				if(size != 7)
					return;
				if(_state == Joining)
				{
					_localNodeId = data[5];
					_nodes.resize(data[6]);
					printf("node accepts join as node %d of %d\n",_localNodeId, (int)_nodes.size());
					_state = Joined;
				}
				_timeoutAccumulator = 0.0f;
				break;
			}
			case 1: //update
			{
				if(size != (I32)(5 + _nodes.size() * 6))
					return;
				if(_state == Joined)
				{
					// process update packet
					U8* ptr = &data[5];
					for(U32 i = 0; i < _nodes.size(); ++i)
					{
						U8 a = ptr[0];
						U8 b = ptr[1];
						U8 c = ptr[2];
						U8 d = ptr[3];
						U16 port = (U16)ptr[4] << 8 | (U16)ptr[5];
						Address address(a, b, c, d, port);
						if(address.getAddress() != 0 && address != _nodes[i].address)
						{ // node is connected but not on record
							printf("node %d: node %d connected\n", _localNodeId, i);
							_nodes[i].connected = true;
							_nodes[i].address = address;
							_addrToNode[address] = &_nodes[i];
						}
						else if(address.getAddress() == 0 && _nodes[i].connected)
						{
							printf("node %d: node %d disconnected\n", _localNodeId, i);
							AddrToNode::iterator itor = _addrToNode.find(_nodes[i].address);
							assert(itor != _addrToNode.end());
							_addrToNode.erase(itor);
							_nodes[i].connected = false;
							_nodes[i].address = Address();
						}
						ptr += 6;
					}
					ptr = nullptr;
				}
				_timeoutAccumulator = 0.0f;
				break;
			}
			default: //other
				std::cerr<<"Node: Packet type received: "<<(I32)(data[4])<<std::endl;
				break;
		}
	}
	else
	{
		AddrToNode::iterator itor = _addrToNode.find(sender);
		if(itor != _addrToNode.end())
		{
			// *** packet sent from another node ***
			NodeState* node = itor->second;
			assert(node);

			I32 nodeId = (I32)(node - &_nodes[0]);
			assert(nodeId >= 0);
			assert(nodeId < (I32)_nodes.size());
			BufferedPacket* packet = new BufferedPacket;
			packet->nodeId = nodeId;
			packet->data.resize(size);
			memcpy(&packet->data[0], data, size);
			_receivedPackets.push(packet);
			//this doesn't feel safe.  I need to look over _receivedPackets to find out
			//when/if this is being deleted properly
		}
		else
		{
			//_nodes[0] contains the address of the server.  If that is the sender OR
			//if the sender has same as address we dobut with a different port, then
			//act like we got something from the server.
			if(_nodes[0].address == sender || sender.getPort() == 30002)/*
				(sender.getAddress()==_nodes[_localNodeId].address.getAddress() &&
					sender.getPort()!=_nodes[_localNodeId].address.getPort())
					|| sender.getPort() == 30002)*/
			{
				//first try - treat this like any other packet
				BufferedPacket* packet = new BufferedPacket;
				packet->nodeId = 0;
				packet->data.resize(size);
				memcpy(&packet->data[0], data, size);
				_receivedPackets.push(packet);
			}
			else
				std::cerr<<"Node: Couldn't find sender "<<sender<<std::endl;
		}
	}
}

void Node::sendPackets(F32 dt)
{
	_sendAccumulator += dt;
	while (_sendAccumulator > _sendRate)
	{
		if(_state == Joining)
		{
			// node is joining: send "join request" packets
			U8 packet[5];
			packet[0] = (U8) ((_protoId >> 24) & 0xFF);
			packet[1] = (U8) ((_protoId >> 16) & 0xFF);
			packet[2] = (U8) ((_protoId >> 8) & 0xFF);
			packet[3] = (U8) ((_protoId) & 0xFF);
			packet[4] = 0;
			_socket.send(_meshAddress, packet, sizeof(packet));
		}
		else if(_state == Joined)
		{
			// node is joined: send "keep alive" packets
			U8 packet[5];
			packet[0] = (U8) ((_protoId >> 24) & 0xFF);
			packet[1] = (U8) ((_protoId >> 16) & 0xFF);
			packet[2] = (U8) ((_protoId >> 8) & 0xFF);
			packet[3] = (U8) ((_protoId) & 0xFF);
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
		if(_timeoutAccumulator > _timeout)
		{
			if(_state == Joining)
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