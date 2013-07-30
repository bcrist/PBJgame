#ifndef NET_RELIABLE_CONNECTION_H_
#include "pbj/net/net_reliable_connection.h"
#endif

using namespace pbj;
using namespace pbj::net;

ReliableConnection::ReliableConnection(U32 protocolId, F32 timeout, U32 max_sequence)
	: Connection(protocolId, timeout)
{
	_reliabilitySystem = ReliabilitySystem(1.0f, max_sequence);
	clearData();
}

ReliableConnection::~ReliableConnection()
{
	if (isRunning())
		stop();
}
		
bool ReliableConnection::sendPacket(const U8* const data, I32 size)
{
	const I32 header = 12;
	U8* packet = new U8[header+size];
	U32 seq = _reliabilitySystem.getLocalSequence();
	U32 ack = _reliabilitySystem.getRemoteSequence();
	U32 ack_bits = _reliabilitySystem.generateAckBits();
	writeHeader(packet, seq, ack, ack_bits);
	memcpy(packet + header, data, size);
	if (!Connection::sendPacket(packet, size + header))
		return false;
	_reliabilitySystem.packetSent(size);
	delete[] packet;
	return true;
}	

I32 ReliableConnection::receivePacket(U8* data, I32 size)
{
	const I32 header = 12;
	if (size <= header)
		return false;
	U8* packet = new U8[header+size];
	I32 received_bytes = Connection::receivePacket(packet, size + header);
	if (received_bytes == 0)
		return false;
	if (received_bytes <= header)
		return false;
	U32 packet_sequence = 0;
	U32 packet_ack = 0;
	U32 packet_ack_bits = 0;
	readHeader(packet, packet_sequence, packet_ack, packet_ack_bits);
	_reliabilitySystem.packetReceived(packet_sequence, received_bytes - header);
	_reliabilitySystem.processAck(packet_ack, packet_ack_bits);
	memcpy(data, packet + header, received_bytes - header);
	delete[] packet;
	return received_bytes - header;
}

void ReliableConnection::update(F32 dt)
{
	Connection::update(dt);
	_reliabilitySystem.update(dt);
}

I32 ReliableConnection::getHeaderSize() const
{
	return Connection::getHeaderSize() + _reliabilitySystem.getHeaderSize();
}

ReliabilitySystem& ReliableConnection::getReliabilitySystem()
{
	return _reliabilitySystem;
}

void ReliableConnection::writeHeader(U8* header, U32 sequence, U32 ack, U32 ackBits)
{
	writeInteger(header, sequence);
	writeInteger(header + 4, ack);
	writeInteger(header + 8, ackBits);
}

void ReliableConnection::readHeader(const U8* header, U32& sequence, U32& ack, U32& ackBits)
{
	readInteger(header, sequence);
	readInteger(header + 4, ack);
	readInteger(header + 8, ackBits);
}

void ReliableConnection::onStop()
{
	clearData();
}

void ReliableConnection::onDisconnect()
{
	clearData();
}

void ReliableConnection::clearData()
{
	_reliabilitySystem.reset();
}