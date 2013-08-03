/*
	Simple Network Library from "Networking for Game Programmers"
	http://www.gaffer.org/networking-for-game-programmers
	Author: Glenn Fiedler <gaffer@gaffer.org>
*/

#ifndef NET_NODE_MESH_H_
#include "pbj/net/net_node_mesh.h"
#endif

using namespace pbj;
using namespace pbj::net;

NetMesh::NetMesh(U32 protoId, I32 maxNodes, F32 sendRate, F32 timeout)
{
	assert(maxNodes >= 1);
	assert(maxNodes <= 255);
	_protoId = protoId;
	_sendRate = sendRate;
	_timeout = timeout;
	_nodes.resize(maxNodes);
	_running = false;
	_sendAccumulator = 0.0f;
}

NetMesh::NetMesh(U32 protoId, I32 maxNodes, F32 sendRate)
{
	assert(maxNodes >= 1);
	assert(maxNodes <= 255);
	_protoId = protoId;
	_sendRate = sendRate;
	_timeout = 10.0f;
	_nodes.resize(maxNodes);
	_running = false;
	_sendAccumulator = 0.0f;
}

NetMesh::NetMesh(U32 protoId, I32 maxNodes)

{
	assert(maxNodes >= 1);
	assert(maxNodes <= 255);
	_protoId = protoId;
	_sendRate = 0.25f;
	_timeout = 10.0f;
	_nodes.resize(maxNodes);
	_running = false;
	_sendAccumulator = 0.0f;
}

NetMesh::NetMesh(U32 protoId)
{
	_protoId = protoId;
	_sendRate = 0.25f;
	_timeout = 10.0f;
	_nodes.resize(255);
	_running = false;
	_sendAccumulator = 0.0f;
}

NetMesh::~NetMesh()
{
	if(_running)
		stop();
}

bool NetMesh::start(I32 port)
{
	assert(!_running);
	printf("start mesh on port %d\n", port);
	if(!_socket.open(port))
		return false;
	_running = true;
	return true;
}

void NetMesh::stop()
{
	assert(_running);
	printf("stop mesh\n");
	_socket.close();
	_idToNode.clear();
	_addrToNode.clear();
	for(U32 i = 0; i < _nodes.size(); ++i)
		_nodes[i] = NodeState();
	_running = false;
	_sendAccumulator = 0.0f;
}

void NetMesh::update(F32 dt)
{
	assert(_running);
	receivePackets();
	sendPackets(dt);
	checkForTimeouts(dt);
}

bool NetMesh::isNodeConnected(I32 nodeId)
{
	assert(nodeId >= 0);
	assert(nodeId < (int) _nodes.size());
	return _nodes[nodeId].mode == NodeState::Connected;
}

Address NetMesh::getNodeAddress(I32 nodeId)
{
	assert(nodeId >= 0);
	assert(nodeId < (int)_nodes.size());
	return _nodes[nodeId].address;
}

I32 NetMesh::getMaxNodes() const
{
	assert(_nodes.size() <= 255);
	return (I32)_nodes.size();
}

void NetMesh::reserve(I32 nodeId, const Address& address)
{
	assert(nodeId >= 0);
	assert(nodeId < (int)_nodes.size());
	printf("mesh reserves node id %d for %d.%d.%d.%d:%d\n", 
		nodeId, address.getA(), address.getB(), address.getC(), address.getD(), address.getPort());
	_nodes[nodeId].mode = NodeState::ConnectionAccept;
	_nodes[nodeId].nodeId = nodeId;
	_nodes[nodeId].address = address;
	_addrToNode.insert(std::make_pair(address, &_nodes[nodeId]));
}

void NetMesh::receivePackets()
{
	while(true)
	{
		Address sender;
		U8 data[256];
		I32 size = _socket.receive(sender, data, sizeof(data));
		if(!size)
			break;
		processPacket(sender, data, size);
	}
}

void NetMesh::processPacket(const Address& sender, U8* data, I32 size)
{
	assert(sender != Address());
	assert(size > 0);
	assert(data);
	// ignore packets that dont have the correct protocol id
	U32 firstIntegerInPacket = (unsigned(data[0]) << 24) | (unsigned(data[1]) << 16) |
										(unsigned(data[2]) << 8)  | unsigned(data[3]);
	//std::cerr<<"Comparing protcol ids: " << hex(data[0])<<" "<<hex(data[1])<<" "<<hex(data[2])<<" "<<hex(data[3])<< " == " <<
	//	hex((U8)((_protoId>>24)&0xFF))<<hex((U8)((_protoId>>16)&0xFF))<<hex((U8)((_protoId>>8)&0xFF))<<hex((U8)(_protoId&0xFF))<<std::endl;
	if(firstIntegerInPacket != _protoId)
		return;
	// determine packet type
	enum PacketType { JoinRequest, KeepAlive };
	PacketType packetType;
	if(data[4] == 0)
		packetType = JoinRequest;
	else if(data[4] == 1)
		packetType = KeepAlive;
	else
		return;
	//std::cerr<<"Packet type: "<<(I32)data[4]<<std::endl;
	// process packet type
	switch (packetType)
	{
	case JoinRequest:
	{
		// is address already joining or joined?
		AddrToNode::iterator it = _addrToNode.find(sender);
		if(it == _addrToNode.end())
		{
			std::cerr<<"Mesh: address not found in nodes, starting join process"<<std::endl;
			// no entry for address, start join process...
			I32 freeSlot = -1;
			for(U32 i = 0; i < _nodes.size(); ++i)
			{
				if(_nodes[i].mode == NodeState::Disconnected)
				{
					freeSlot = (I32) i;
					break;
				}
			}
			if(freeSlot >= 0)
			{
				printf("mesh accepts %d.%d.%d.%d:%d as node %d\n", 
				sender.getA(), sender.getB(), sender.getC(), sender.getD(), sender.getPort(), freeSlot);
				assert(_nodes[freeSlot].mode == NodeState::Disconnected);
				_nodes[freeSlot].mode = NodeState::ConnectionAccept;
				_nodes[freeSlot].nodeId = freeSlot;
				_nodes[freeSlot].address = sender;
				_addrToNode.insert(std::make_pair(sender, &_nodes[freeSlot]));
			}
		}
		else if(it->second->mode == NodeState::ConnectionAccept)
		{
			// reset timeout accumulator, but only while joining
			it->second->timeoutAccumulator = 0.0f;
		}
		break;
	}
	case KeepAlive:
	{
		AddrToNode::iterator it = _addrToNode.find(sender);
		if(it != _addrToNode.end())
		{
			// progress from "connection accept" to "connected"
			if(it->second->mode == NodeState::ConnectionAccept)
			{
				it->second->mode = NodeState::Connected;
				printf("mesh completes join of node %d\n", it->second->nodeId);
			}
			// reset timeout accumulator for node
			it->second->timeoutAccumulator = 0.0f;
		}
		break;
	}
	default:
		break;
	}
}

void NetMesh::sendPackets(F32 dt)
{
	_sendAccumulator += dt;
	while(_sendAccumulator > _sendRate)
	{
		for(U32 i = 0; i < _nodes.size(); ++i)
		{
			if(_nodes[i].mode == NodeState::ConnectionAccept)
			{
				// node is negotiating join: send "connection accepted" packets
				U8 packet[7];
				packet[0] = (U8) ((_protoId >> 24) & 0xFF);
				packet[1] = (U8) ((_protoId >> 16) & 0xFF);
				packet[2] = (U8) ((_protoId >> 8) & 0xFF);
				packet[3] = (U8) ((_protoId) & 0xFF);
				packet[4] = 0;
				packet[5] = (U8) i;
				packet[6] = (U8) _nodes.size();
				/*
				std::cerr<<"Packet sent:\n\t";
				for(int j=0;j<7;++j)
					std::cerr<<hex(packet[j])<<" ";
				std::cerr<<std::endl;*/
				_socket.send(_nodes[i].address, packet, sizeof(packet));
			}
			else if(_nodes[i].mode == NodeState::Connected)
			{
				// node is connected: send "update" packets
				U8* packet = new U8[5+6*_nodes.size()];
				packet[0] = (U8) ((_protoId >> 24) & 0xFF);
				packet[1] = (U8) ((_protoId >> 16) & 0xFF);
				packet[2] = (U8) ((_protoId >> 8) & 0xFF);
				packet[3] = (U8) ((_protoId) & 0xFF);
				packet[4] = 1;
				U8* ptr = &packet[5];
				for(U32 j = 0; j < _nodes.size(); ++j)
				{
					ptr[0] = (U8) _nodes[j].address.getA();
					ptr[1] = (U8) _nodes[j].address.getB();
					ptr[2] = (U8) _nodes[j].address.getC();
					ptr[3] = (U8) _nodes[j].address.getD();
					ptr[4] = (U8) ((_nodes[j].address.getPort() >> 8) & 0xFF);
					ptr[5] = (U8) ((_nodes[j].address.getPort()) & 0xFF);
					ptr += 6;
				}
				_socket.send(_nodes[i].address, packet, (5+6*_nodes.size()) * sizeof(packet[0]));
				delete[] packet;
			}
		}
		_sendAccumulator -= _sendRate;
	}
}

void NetMesh::checkForTimeouts(F32 dt)
{
	for(U32 i = 0; i < _nodes.size(); ++i)
	{
		if(_nodes[i].mode != NodeState::Disconnected)
		{
			_nodes[i].timeoutAccumulator += dt;
			if(_nodes[i].timeoutAccumulator > _timeout)
			{
				printf("mesh timed out node %d\n", i);
				AddrToNode::iterator addr_it = _addrToNode.find(_nodes[i].address);
				assert(addr_it != _addrToNode.end());
				_addrToNode.erase(addr_it);
				_nodes[i] = NodeState();
			}
		}
	}
}