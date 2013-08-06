////////////////////////////////////////////////////////////////////////////////
/// \file	Z:\Documents\PBJgame\src\pbj\net\net_connection.cpp
///
/// \brief	Implements the net connection class.
////////////////////////////////////////////////////////////////////////////////
#ifndef NET_LAN_CONNECTION_H_
#include "pbj/net/net_connection.h"
#endif

using namespace pbj;
using namespace pbj::net;

////////////////////////////////////////////////////////////////////////////////
/// \fn	Connection::Connection(U32 protoId, F32 timeout)
///
/// \brief	Constructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	protoId	Identifier for the prototype.
/// \param	timeout	The timeout.
////////////////////////////////////////////////////////////////////////////////
Connection::Connection(U32 protoId, F32 timeout)
{
	_protoId = protoId;
	_timeout = timeout;
	_mode = None;
	_running = false;
	clearData();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Connection::~Connection()
///
/// \brief	Destructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
Connection::~Connection()
{
	if(isRunning())
		stop();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Connection::start(I32 port)
///
/// \brief	Starts the given port.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	port	The port.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
bool Connection::start(I32 port)
{
	assert(!_running);
	printf("start connection on port %d\n", port);
	if(!_socket.open(port))
		return false;
	_running = true;
	onStart();
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Connection::stop()
///
/// \brief	Stops this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
void Connection::stop()
{
	assert(_running);
	printf("stop connection\n");
	bool connected = isConnected();
	clearData();
	_socket.close();
	_running = false;
	if(connected)
		onDisconnect();
	onStop();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Connection::isRunning() const
///
/// \brief	Query if this object is running.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	true if running, false if not.
////////////////////////////////////////////////////////////////////////////////
bool Connection::isRunning() const
{
	return _running;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Connection::listen()
///
/// \brief	Listens this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
void Connection::listen()
{
	printf("server listening for connection\n");
	bool connected = isConnected();
	clearData();
	if(connected)
		onDisconnect();
	_mode = Server;
	_state = Listening;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Connection::connect(const Address& address)
///
/// \brief	Connects the given address.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	address	The address to connect.
////////////////////////////////////////////////////////////////////////////////
void Connection::connect(const Address& address)
{
	PBJ_LOG(pbj::VInfo) << "Connection: Client connecting to " << address
						<< PBJ_LOG_END;
	bool connected = isConnected();
	clearData();
	if(connected)
		onDisconnect();
	///< An enum constant representing the mode option
	_mode = Client;
	_state = Connecting;
	_address = address;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Connection::isConnecting() const
///
/// \brief	Query if this object is connecting.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	true if connecting, false if not.
////////////////////////////////////////////////////////////////////////////////
bool Connection::isConnecting() const
{
	return _state == Connecting;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Connection::connectFailed() const
///
/// \brief	Checks to see if the connection attempt has failed.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	true if it the attempt has failed, false if it not.
////////////////////////////////////////////////////////////////////////////////
bool Connection::connectFailed() const
{
	return _state == ConnectFail;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Connection::isConnected() const
///
/// \brief	Query if this object is connected.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	true if connected, false if not.
////////////////////////////////////////////////////////////////////////////////
bool Connection::isConnected() const
{
	return _state == Connected;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Connection::isListening() const
///
/// \brief	Query if this object is listening.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	true if listening, false if not.
////////////////////////////////////////////////////////////////////////////////
bool Connection::isListening() const
{
	return _state == Listening;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Connection::Mode Connection::getMode() const
///
/// \brief	Gets the mode.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The mode.
////////////////////////////////////////////////////////////////////////////////
Connection::Mode Connection::getMode() const
{
	return _mode;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Connection::update(F32 dt)
///
/// \brief	Updates using a given dt.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	dt	The change in time (delta time).
////////////////////////////////////////////////////////////////////////////////
void Connection::update(F32 dt)
{
	assert(_running);
	_timeoutAccumulator += dt;
	if(_timeoutAccumulator > _timeout)
	{
		if(_state == Connecting)
		{
			printf("connect timed out\n");
			clearData();
			_state = ConnectFail;
			onDisconnect();
		}
		else if(_state == Connected)
		{
			printf("connection timed out\n");
			clearData();
			if(_state == Connecting)
				_state = ConnectFail;
			onDisconnect();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Connection::sendPacket(const U8* const data, I32 size)
///
/// \brief	Sends a packet.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	data	The data to send.
/// \param	size	The size of the data buffer.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
bool Connection::sendPacket(const U8* const data, I32 size)
{
	assert(_running);
	if(_address.getAddress() == 0)
		return false;
	U8* packet= new U8[size+getHeaderSize()];
	packet[0] = (U8) (_protoId >> 24);
	packet[1] = (U8) ((_protoId >> 16) & 0xFF);
	packet[2] = (U8) ((_protoId >> 8) & 0xFF);
	packet[3] = (U8) ((_protoId) & 0xFF);
	memcpy(&packet[4], data, size);
	bool ret = _socket.send(_address, packet, size + getHeaderSize());
	delete[] packet;
	return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	I32 Connection::receivePacket(U8* data, I32 size)
///
/// \brief	Receive packet.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param [out]	data	The buffer for storing received data.
/// \param	size			The size of the buffer.
///
/// \return	The amount of data received, in bytes.
////////////////////////////////////////////////////////////////////////////////
I32 Connection::receivePacket(U8* data, I32 size)
{
	assert(_running);
	U8* packet = new U8[size+getHeaderSize()];
	Address sender;
	int bytes_read = _socket.receive(sender, packet, size + getHeaderSize());
	if(bytes_read == 0)
	{
		delete[] packet;
		return 0;
	}
	if(bytes_read <= 4)
	{
		delete[] packet;
		return 0;
	}
	if(packet[0] != (U8) (_protoId >> 24) || 
		 packet[1] != (U8) ((_protoId >> 16) & 0xFF) ||
		 packet[2] != (U8) ((_protoId >> 8) & 0xFF) ||
		 packet[3] != (U8) (_protoId & 0xFF))
	{
		delete[] packet;
		return 0;
	}
	if(_mode == Server && !isConnected())
	{
		printf("server accepts connection from client %d.%d.%d.%d:%d\n", 
			sender.getA(), sender.getB(), sender.getC(), sender.getD(), sender.getPort());
		_state = Connected;
		_address = sender;
		onConnect();
	}
	if(sender == _address)
	{
		if(_mode == Client && _state == Connecting)
		{
			/// \brief	The printf.
			printf("client completes connection with server\n");
			_state = Connected;
			onConnect();
		}
		_timeoutAccumulator = 0.0f;
		memcpy(data, &packet[4], bytes_read - 4);
		delete[] packet;
		return bytes_read - 4;
	}
	delete[] packet;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	I32 Connection::getHeaderSize() const
///
/// \brief	Gets header size.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The header size.
////////////////////////////////////////////////////////////////////////////////
I32 Connection::getHeaderSize() const
{
	return 4;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Connection::onStart()
///
/// \brief	Executes the start action.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
void Connection::onStart() {}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Connection::onStop()
///
/// \brief	Executes the stop action.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
void Connection::onStop() {}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Connection::onConnect()
///
/// \brief	Executes the connect action.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
void Connection::onConnect() {}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Connection::onDisconnect()
///
/// \brief	Executes the disconnect action.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
void Connection::onDisconnect() {}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Connection::clearData()
///
/// \brief	Clears the data.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
void Connection::clearData()
{
	_state = Disconnected;
	_timeoutAccumulator = 0.0f;
	_address = Address();
}
