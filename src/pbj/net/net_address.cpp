#ifndef NET_ADDRESS_H_
#include "pbj/net/net_address.h"
#endif

using namespace pbj;
using namespace pbj::net;

Address::Address()
{
	_address = 0;
	_port = 0;
}

Address::Address(U8 a, U8 b, U8 c, U8 d, U16 port)
{
	_address = (a<<24) | (b<<16) | (c<<8) | d;
	_port = port;
}

Address::Address(U32 address, U16 port)
{
	_address = address;
	_port = port;
}

pbj::U32 Address::getAddress() const
{
	return _address;
}
	
pbj::U8 Address::getA() const
{
	return (U8)(_address >> 24);
}
	
pbj::U8 Address::getB() const
{
	return (U8)(_address >> 16);
}
	
pbj::U8 Address::getC() const
{
	return (U8)(_address >> 8);
}
	
pbj::U8 Address::getD() const
{
	return (U8)(_address);
}
	
pbj::U16 Address::getPort() const
{ 
	return _port;
}