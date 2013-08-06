////////////////////////////////////////////////////////////////////////////////
/// \file	Z:\Documents\PBJgame\src\pbj\net\net_reliable_connection.cpp
///
/// \brief	Implements the net reliable connection class.
////////////////////////////////////////////////////////////////////////////////
#ifndef NET_RELIABLE_CONNECTION_H_
#include "pbj/net/net_reliable_connection.h"
#endif

using namespace pbj;
using namespace pbj::net;

////////////////////////////////////////////////////////////////////////////////
/// \fn	ReliableConnection::ReliableConnection(U32 protocolId, F32 timeout,
/// 	U32 max_sequence)
///
/// \brief	Constructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	protocolId  	Identifier for the protocol.
/// \param	timeout			The timeout.
/// \param	max_sequence	The maximum sequence.
////////////////////////////////////////////////////////////////////////////////
ReliableConnection::ReliableConnection(U32 protocolId, F32 timeout, U32 max_sequence)
	: Connection(protocolId, timeout)
{
	_reliabilitySystem = ReliabilitySystem(1.0f, max_sequence);
	clearData();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	ReliableConnection::~ReliableConnection()
///
/// \brief	Destructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
ReliableConnection::~ReliableConnection()
{
	if (isRunning())
		stop();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool ReliableConnection::sendPacket(const U8* const data, I32 size)
///
/// \brief	Sends a packet.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	data	The data.
/// \param	size	The size.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	I32 ReliableConnection::receivePacket(U8* data, I32 size)
///
/// \brief	Receive packet.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param [in,out]	data	If non-null, the data.
/// \param	size			The size.
///
/// \return	.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	void ReliableConnection::update(F32 dt)
///
/// \brief	Updates the given dt.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	dt	The dt.
////////////////////////////////////////////////////////////////////////////////
void ReliableConnection::update(F32 dt)
{
	Connection::update(dt);
	_reliabilitySystem.update(dt);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	I32 ReliableConnection::getHeaderSize() const
///
/// \brief	Gets header size.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The header size.
////////////////////////////////////////////////////////////////////////////////
I32 ReliableConnection::getHeaderSize() const
{
	return Connection::getHeaderSize() + _reliabilitySystem.getHeaderSize();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	ReliabilitySystem& ReliableConnection::getReliabilitySystem()
///
/// \brief	Gets reliability system.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The reliability system.
////////////////////////////////////////////////////////////////////////////////
ReliabilitySystem& ReliableConnection::getReliabilitySystem()
{
	return _reliabilitySystem;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void ReliableConnection::writeHeader(U8* header, U32 sequence,
/// 	U32 ack, U32 ackBits)
///
/// \brief	Writes a header.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param [in,out]	header	If non-null, the header.
/// \param	sequence	  	The sequence.
/// \param	ack			  	The acknowledge.
/// \param	ackBits		  	The acknowledge bits.
////////////////////////////////////////////////////////////////////////////////
void ReliableConnection::writeHeader(U8* header, U32 sequence, U32 ack, U32 ackBits)
{
	writeInteger(header, sequence);
	writeInteger(header + 4, ack);
	writeInteger(header + 8, ackBits);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void ReliableConnection::readHeader(const U8* header, U32& sequence,
/// 	U32& ack, U32& ackBits)
///
/// \brief	Reads a header.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	header				The header.
/// \param [in,out]	sequence	The sequence.
/// \param [in,out]	ack			The acknowledge.
/// \param [in,out]	ackBits 	The acknowledge bits.
////////////////////////////////////////////////////////////////////////////////
void ReliableConnection::readHeader(const U8* header, U32& sequence, U32& ack, U32& ackBits)
{
	readInteger(header, sequence);
	readInteger(header + 4, ack);
	readInteger(header + 8, ackBits);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void ReliableConnection::onStop()
///
/// \brief	Executes the stop action.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
void ReliableConnection::onStop()
{
	clearData();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void ReliableConnection::onDisconnect()
///
/// \brief	Executes the disconnect action.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
void ReliableConnection::onDisconnect()
{
	clearData();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void ReliableConnection::clearData()
///
/// \brief	Clears the data.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
void ReliableConnection::clearData()
{
	_reliabilitySystem.reset();
}