////////////////////////////////////////////////////////////////////////////////
/// \file	Z:\Documents\PBJgame\src\pbj\net\net_transport.cpp
///
/// \brief	Implements the net transport class.
////////////////////////////////////////////////////////////////////////////////
#ifndef NET_TRANSPORT_H_
#include "pbj/net/net_transport.h"
#endif

using namespace pbj;
using namespace pbj::net;

/// \brief	Number of transports.
I32 Transport::transportCount = 0;

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Transport::init()
///
/// \brief	Initialises this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
bool Transport::init()
{
  return initSockets(); //fron net_sockets.h
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Transport::shutdown()
///
/// \brief	Shuts down this object and frees any resources it is using.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
void Transport::shutdown()
{
  shutdownSockets(); //from net_sockets.h
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Transport* Transport::create()
///
/// \brief	Creates this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	null if it fails, else.
////////////////////////////////////////////////////////////////////////////////
Transport* Transport::create()
{
  transportCount++;
  return new Transport();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Transport::destroy(Transport* transport)
///
/// \brief	Destroys the given transport.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param [in,out]	transport	If non-null, the transport to destroy.
////////////////////////////////////////////////////////////////////////////////
void Transport::destroy(Transport* transport)
{
  if(transport != 0)
  {
    delete transport;
    transport = 0;
    transportCount--;
  }
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Transport::getHostName(U8* hostname, int size)
///
/// \brief	Gets host name.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param [in,out]	hostname	If non-null, the hostname.
/// \param	size				The size.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
bool Transport::getHostName(U8* hostname, int size)
{
  return net::getHostName(hostname, size); //from net_sockets.h
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Transport::Transport()
///
/// \brief	Default constructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	Transport::~Transport()
///
/// \brief	Destructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
Transport::~Transport()
{
  stop();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Transport::startServer(const U8* const name)
///
/// \brief	Starts a server.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	name	The name.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Transport::connectClient(const U8* const server)
///
/// \brief	Connects a client.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	server	The server.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Transport::connectClient(const Address& server)
///
/// \brief	Connects a client.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	server	The server.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Transport::enterLobby()
///
/// \brief	Determines if we can enter lobby.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Transport::leaveLobby()
///
/// \brief	Determines if we can leave lobby.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
bool Transport::leaveLobby()
{
	assert(_listener);
	delete _listener;
	_listener = nullptr;
	return !_listener;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Transport::stop()
///
/// \brief	Stops this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Transport::sendPacket(I32 nodeId, const U8* const data, I32 size)
///
/// \brief	Sends a packet.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	nodeId	Identifier for the node.
/// \param	data  	The data.
/// \param	size  	The size.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
bool Transport::sendPacket(I32 nodeId, const U8* const data, I32 size)
{
	assert(_node);
	return _node->sendPacket(nodeId, data, size);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	I32 Transport::receivePacket(I32& nodeId, U8* data, I32 size)
///
/// \brief	Receive packet.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param [in,out]	nodeId	Identifier for the node.
/// \param [in,out]	data  	If non-null, the data.
/// \param	size		  	The size.
///
/// \return	.
////////////////////////////////////////////////////////////////////////////////
I32 Transport::receivePacket(I32& nodeId, U8* data, I32 size)
{
	return _node->receivePacket(nodeId, data, size);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Address Transport::getNodeAt(I32 idx)
///
/// \brief	Gets node at.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	idx	The index.
///
/// \return	The node at.
////////////////////////////////////////////////////////////////////////////////
Address Transport::getNodeAt(I32 idx)
{
	return _node->getNodeAddress(idx);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Address Transport::getNodeMeshAddress()
///
/// \brief	Gets node mesh address.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The node mesh address.
////////////////////////////////////////////////////////////////////////////////
Address Transport::getNodeMeshAddress() { return _node->getMeshAddress(); }

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Transport::update(F32 dt)
///
/// \brief	Updates the given dt.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	dt	The dt.
////////////////////////////////////////////////////////////////////////////////
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
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Transport::configure(Config& config)
///
/// \brief	Configures the given configuration.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param [in,out]	config	The configuration.
////////////////////////////////////////////////////////////////////////////////
void Transport::configure(Config& config)
{
	_config = config;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	const Transport::Config& Transport::getConfig() const
///
/// \brief	Gets the configuration.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The configuration.
////////////////////////////////////////////////////////////////////////////////
const Transport::Config& Transport::getConfig() const
{
	return _config;

}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Transport::isConnected() const
///
/// \brief	Query if this object is connected.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	true if connected, false if not.
////////////////////////////////////////////////////////////////////////////////
bool Transport::isConnected() const
{
  return _node && _node->isConnected();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Transport::connectFailed() const
///
/// \brief	Connects the failed.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
bool Transport::connectFailed() const
{
  return _node && _node->joinFailed() || _connectingByName && _connectFailed;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	I32 Transport::getLobbyEntryCount()
///
/// \brief	Gets lobby entry count.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The lobby entry count.
////////////////////////////////////////////////////////////////////////////////
I32 Transport::getLobbyEntryCount()
{
  if(_listener)
    return _listener->getEntryCount();
  else
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Transport::getLobbyEntryAtIndex(I32 index, LobbyEntry& entry)
///
/// \brief	Gets lobby entry at index.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	index		 	Zero-based index of the.
/// \param [in,out]	entry	The entry.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Transport::isNoneConnected(I32 nodeId)
///
/// \brief	Query if 'nodeId' is none connected.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	nodeId	Identifier for the node.
///
/// \return	true if none connected, false if not.
////////////////////////////////////////////////////////////////////////////////
bool Transport::isNoneConnected(I32 nodeId)
{
  assert(_node);
  return _node->isNodeConnected(nodeId);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	I32 Transport::getLocalNodeID() const
///
/// \brief	Gets local node identifier.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The local node identifier.
////////////////////////////////////////////////////////////////////////////////
I32 Transport::getLocalNodeID() const
{
  assert(_node);
  return _node->getLocalNodeId();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	I32 Transport::getMaxNodes() const
///
/// \brief	Gets maximum nodes.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The maximum nodes.
////////////////////////////////////////////////////////////////////////////////
I32 Transport::getMaxNodes() const
{
  assert(_node);
  return _node->getMaxNodes();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	ReliabilitySystem& Transport::getReliability(I32)
///
/// \brief	Gets a reliability.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	parameter1	The first parameter.
///
/// \return	The reliability.
////////////////////////////////////////////////////////////////////////////////
ReliabilitySystem& Transport::getReliability(I32)
{
  //todo: implement
  static ReliabilitySystem reliabilitySystem;
  return reliabilitySystem;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	I32 Transport::getNumberConnected() const
///
/// \brief	Gets number connected.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The number connected.
////////////////////////////////////////////////////////////////////////////////
I32 Transport::getNumberConnected() const
{
	assert(_mesh);
	return _mesh->getNumberConnected();
}