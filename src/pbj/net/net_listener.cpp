#ifndef NET_LISTENER_H_
#include "pbj/net/net_listener.h"
#endif

using namespace pbj;
using namespace net;

Listener::Listener(U32 protoId, F32 timeout)
{
	std::cerr<<"Listener construction"<<std::endl;
	_protoId = protoId;
	_timeout = timeout;
	_running = false;
	clearData();
	std::cerr<<"Listener data cleared"<<std::endl;
}

Listener::Listener(U32 protoId)
{
	_protoId = protoId;
	_timeout = 10.0f;
	_running = false;
	clearData();
}

Listener::~Listener()
{
	if(_running)
		stop();
}

bool Listener::start(I32 port)
{
	assert(!_running);
	PBJ_LOG(pbj::VInfo) << "Starting listener on port " << port << PBJ_LOG_END;
	if(!_socket.open(port))
		return false;
	_running = true;
	return true;
}

void Listener::stop()
{
	assert(_running);
	PBJ_LOG(pbj::VInfo) << "Stopping listener" << PBJ_LOG_END;
	_socket.close();
	_running = false;
	clearData();
}

void Listener::update(F32 dt)
{
	assert(_running);
	U8 packet[256];
	while(true)
	{
		Address sender;
		I32 bytesRead = _socket.receive(sender, packet, 256);
		if(bytesRead == 0) //this is how we break out of the loop
			break;
		if(bytesRead < 13)
			continue;
		std::cerr<<"Listener: received " << bytesRead << " bytes." <<std::endl;
		U32 packetZero;
		U32 packetProtoId;
		U16 packetServerPort;
		U8 packetStrLength;
		readInteger(packet, packetZero);
		readInteger(packet+4, packetProtoId);
		readShort(packet+8, packetServerPort);
		packetStrLength = packet[12];
		if(packetZero != 0 || packetProtoId != _protoId || packetStrLength > 63 ||
			packetStrLength+12+1 > bytesRead)
		{
			PBJ_LOG(pbj::VInfo) << "This is not the packet we are looking for" << PBJ_LOG_END;
			continue;
		}
		ListenerEntry entry;
		memcpy(entry.name, packet+13, packetStrLength);
		entry.name[packetStrLength] = '\0';
		entry.address = Address(sender.getAddress(), packetServerPort);
		entry.timeoutAccumulator = 0.0f;
		ListenerEntry* existingEntry = findEntry(entry);
		if(existingEntry)
			existingEntry->timeoutAccumulator = 0.0f;
		else
			_entries.push_back(entry);
	}
	vector<ListenerEntry>::iterator itor = _entries.begin();
	while(itor != _entries.end())
	{
		itor->timeoutAccumulator += dt;
		if(itor->timeoutAccumulator > _timeout)
			itor = _entries.erase(itor);
		else
			++itor;
	}
}

I32 Listener::getEntryCount() const
{
	return _entries.size();
}

const ListenerEntry& Listener::getEntry(I32 idx) const
{
	assert(idx >= 0);
	assert(idx < (I32)_entries.size());
	return _entries[idx];
}

ListenerEntry* Listener::findEntry(const ListenerEntry& entry)
{
	for(I32 i=0;i<(I32)_entries.size();++i)
		if(_entries[i].address == entry.address && std::strcmp((const char*)_entries[i].name, (const char*)entry.name)==0)
			return &_entries[i];
	return 0;
}

void Listener::clearData() { _entries.clear(); }