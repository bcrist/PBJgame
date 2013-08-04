#ifndef NET_TRANSPORT_H_
#include "pbj/net/net_transport.h"
#endif

using namespace pbj;
using namespace pbj::net;

////////////////////////////////////////////////////////////////////////////////
//Static variables and methods
////////////////////////////////////////////////////////////////////////////////
I32 Transport::transportCount = 0;

bool Transport::init()
{
  return initSockets(); //fron net_sockets.h
}

void Transport::shutdown()
{
  shutdownSockets(); //from net_sockets.h
}

Transport* Transport::create()
{
  transportCount++;
  return new Transport();
}

void Transport::destroy(Transport* transport)
{
  if(transport != 0)
  {
    delete transport;
    transport = 0;
    transportCount--;
  }
}

bool Transport::getHostName(U8* hostname, int size)
{
  return net::getHostName(hostname, size); //from net_sockets.h
}
////////////////////////////////////////////////////////////////////////////////
//End statics
///////////////////////////////////////////////////////////////////////////////
Transport::Transport()
{
  _mesh = nullptr;
  _node = nullptr;
  _beacon = nullptr;
  _listener = nullptr;
  _beaconAccumulator = 0.0f;
  _connectingByName = false;
  _connectAccumulator = 0.0f;
  _connectFailed = false;
}

Transport::~Transport()
{
  stop();
}

bool Transport::startServer(const U8* const name)
{
  assert(!_node);
  assert(!_mesh);
  assert(!_beacon);
  assert(!_listener);
  PBJ_LOG(Verbosity::VNotice) << "Transport(LAN): Server starting" << PBJ_LOG_NL;

  _beacon = new Beacon(name, _config.protoId, _config.listenerPort, _config.meshPort);
  if(!_beacon->start(_config.beaconPort))
  {
    PBJ_LOG(Verbosity::VError) << "Failed to start beacon on port " << _config.beaconPort << PBJ_LOG_END;
    stop();
    return false;
  }
  _mesh = new NetMesh(_config.protoId, _config.maxNodes, _config.meshSendRate, _config.timeout);
  if(!_mesh->start(_config.meshPort))
  {
    PBJ_LOG(Verbosity::VError) << "Failed to start mesh on port " << _config.meshPort << PBJ_LOG_END;
    stop();
    return false;
  }
  _node = new Node(_config.protoId, _config.meshSendRate, _config.timeout);
  if(!_node->start(_config.serverPort))
  {
    PBJ_LOG(Verbosity::VError) << "Failed to start node on port " << _config.serverPort << PBJ_LOG_END;
    stop();
    return false;
  }
  _mesh->reserve(0, Address(127,0,0,1,_config.serverPort));
  _node->join(Address(127,0,0,1,_config.meshPort));
  PBJ_LOG_STREAM << "Server started" << PBJ_LOG_END;
  return true;
}

bool Transport::connectClient(const U8* const server)
{
	assert(!_node);
	assert(!_mesh);
	assert(!_beacon);
	assert(!_listener);

	// connect by address?
	unsigned int a = 0;
	unsigned int b = 0;
	unsigned int c = 0;
	unsigned int d = 0;
	unsigned int port = 0;
	bool isAddress = false;
	if(sscanf_s((const char*)server, "%d.%d.%d.%d:%d", &a, &b, &c, &d, &port))
	{
		isAddress = true;
	}
	else
	{
		port = _config.meshPort;
		if(sscanf_s((const char*)server, "%d.%d.%d.%d", &a, &b, &c, &d))
		isAddress = true;
	}
  
	if(isAddress) // yes, connect by address
	{
		printf("lan transport: client connect to address: %d.%d.%d.%d:%d\n", a, b, c, d, port);
		Address addr = Address((U8)a, (U8)b, (U8)c, (U8)d, (U16)port);
		return connectClient(addr);
	}
	else // no, connect by hostname
	{
		printf("Transport: client connect by name \"%s\"\n", server);
		_listener = new Listener(_config.protoId, _config.timeout);
		if(!_listener->start(_config.listenerPort))
		{
			printf("failed to start listener on port %d\n", _config.listenerPort);
			stop();
			return false;
		}
		PBJ_LOG(pbj::VInfo) << "Started listener on port " << _config.listenerPort << PBJ_LOG_END;
		_connectingByName = true;
		strncpy_s(_connectName, (const char*)server, sizeof(_connectName) - 1);
		_connectName[ sizeof(_connectName) - 1 ] = '\0';
		_connectAccumulator = 0.0f;
		_connectFailed = false;
	}
	return true;
}

bool Transport::connectClient(const Address& server)
{
	_node = new Node(_config.protoId, _config.meshSendRate, _config.timeout);
	if(!_node->start(_config.clientPort))
	{
		printf("failed to start node on port %d\n", _config.serverPort);
		stop();
		return false;
	}
	_node->join(server);
	return true;
}

bool Transport::enterLobby()
{
	std::cerr<<"Transport: enter lobby"<<std::endl;
	_listener = new Listener(_config.protoId, _config.timeout);
	std::cerr<<"Transport: starting listener"<<std::endl;
	if(!_listener->start(_config.listenerPort))
	{
		printf("failed to start listener on port %d\n", _config.listenerPort);
		stop();
		return false;
	}
	return true;
}

bool Transport::leaveLobby()
{
	assert(_listener);
	delete _listener;
	_listener = nullptr;
	return !_listener;
}

void Transport::stop()
{
	PBJ_LOG(Verbosity::VNotice) << "Transport: stop" << PBJ_LOG_END;
	if(_mesh)
	{
		delete _mesh;
		_mesh = 0;
	}
	if(_node)
	{
		delete _node;
		_node = 0;
	}
	if(_beacon)
	{
		delete _beacon;
		_beacon = 0;
	}
	if(_listener)
	{
		delete _listener;
		_listener = 0;
	}
	_connectingByName = false;
	_connectFailed = false;
}

bool Transport::sendPacket(I32 nodeId, const U8* const data, I32 size)
{
	assert(_node);
	return _node->sendPacket(nodeId, data, size);
}

I32 Transport::receivePacket(I32& nodeId, U8* data, I32 size)
{
	return _node->receivePacket(nodeId, data, size);
}

void Transport::update(F32 dt)
{
	//if we're still trying to connect, do what's necessary
	if(_connectingByName && !_connectFailed)
	{
		assert(_listener);
		const int entryCount = _listener->getEntryCount();
		for(int i = 0; i < entryCount; ++i)
		{
			const ListenerEntry& entry = _listener->getEntry(i);
			if(strcmp((const char*)(entry.name), _connectName) == 0)
			{
				PBJ_LOG(pbj::VInfo) << "Transport: found server " << entry.address
									<< " attempting to connect"<<PBJ_LOG_END;
				_node = new Node(_config.protoId, _config.meshSendRate, _config.timeout);
				if(!_node->start(_config.clientPort))
				{
					printf("failed to start _node on port %d\n", _config.serverPort);
					stop();
					_connectFailed = true;
					return;
				}
				_node->join(entry.address);

				//we've found a server, no need to listen for one anymore
				delete _listener;
				_listener = NULL;
				_connectingByName = false;
			}
		}
		if(_connectingByName)
		{
			_connectAccumulator += dt;
			if(_connectAccumulator > _config.timeout)
				_connectFailed = true;
		}
	}

	//update the other pieces of the trasnport layer if they exist
	//this allows the Transport to work for servers and clients
	if(_mesh)
		_mesh->update(dt);
	if(_node)
		_node->update(dt);
	if(_beacon)
	{
		_beaconAccumulator += dt;
		while(_beaconAccumulator >= 1.0f)
		{
			_beacon->update(1.0f);
			_beaconAccumulator -= 1.0f;
		}
	}
	if(_listener)
		_listener->update(dt);
	/*if(_mesh)
		_mesh->update(dt);
	if(_node)
		_node->update(dt);*/
}

////////////////////////////////////////////////////////////////////////////////
//Accessors and other lesser methods
////////////////////////////////////////////////////////////////////////////////
void Transport::configure(Config& config)
{
	_config = config;
}

const Transport::Config& Transport::getConfig() const
{
	return _config;

}
bool Transport::isConnected() const
{
  return _node && _node->isConnected();
}

bool Transport::connectFailed() const
{
  return _node && _node->joinFailed() || _connectingByName && _connectFailed;
}

I32 Transport::getLobbyEntryCount()
{
  if(_listener)
    return _listener->getEntryCount();
  else
    return 0;
}

bool Transport::getLobbyEntryAtIndex(I32 index, LobbyEntry& entry)
{
  if(!_listener || index<0 || index>=_listener->getEntryCount())
    return false;
  const ListenerEntry& e = _listener->getEntry(index);
  sprintf_s((char*)(entry.address), 65, "%d.%d.%d.%d:%d", e.address.getA(), e.address.getB(), e.address.getC(), e.address.getD(), e.address.getPort());
  strncpy_s((char*)(entry.name), 65, (const char*)(e.name), sizeof(entry.name));
  entry.name[sizeof(entry.name) - 1] = '\0';
  return true;
}



bool Transport::isNoneConnected(I32 nodeId)
{
  assert(_node);
  return _node->isNodeConnected(nodeId);
}

I32 Transport::getLocalNodeID() const
{
  assert(_node);
  return _node->getLocalNodeId();
}

I32 Transport::getMaxNodes() const
{
  assert(_node);
  return _node->getMaxNodes();
}

ReliabilitySystem& Transport::getReliability(I32)
{
  //todo: implement
  static ReliabilitySystem reliabilitySystem;
  return reliabilitySystem;
}

I32 Transport::getNumberConnected() const
{
	assert(_mesh);
	return _mesh->getNumberConnected();
}