#ifndef NET_LAN_CONNECTION_H_
#include "pbj/net/net_connection.h"
#endif

using namespace pbj;
using namespace pbj::net;

Connection::Connection(U32 protoId, F32 timeout)
{
	_protoId = protoId;
	_timeout = timeout;
	_mode = None;
	_running = false;
	clearData();
}

Connection::~Connection()
{
	if(isRunning())
		stop();
}

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

bool Connection::isRunning() const
{
	return _running;
}

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

void Connection::connect(const Address& address)
{
	printf("client connecting to %d.%d.%d.%d:%d\n", 
		address.getA(), address.getB(), address.getC(), address.getD(), address.getPort());
	bool connected = isConnected();
	clearData();
	if(connected)
		onDisconnect();
	_mode = Client;
	_state = Connecting;
	_address = address;
}

bool Connection::isConnecting() const
{
	return _state == Connecting;
}

bool Connection::connectFailed() const
{
	return _state == ConnectFail;
}

bool Connection::isConnected() const
{
	return _state == Connected;
}

bool Connection::isListening() const
{
	return _state == Listening;
}

Connection::Mode Connection::getMode() const
{
	return _mode;
}

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

I32 Connection::getHeaderSize() const
{
	return 4;
}

void Connection::onStart() {}
void Connection::onStop() {}
void Connection::onConnect() {}
void Connection::onDisconnect() {}

void Connection::clearData()
{
	_state = Disconnected;
	_timeoutAccumulator = 0.0f;
	_address = Address();
}
