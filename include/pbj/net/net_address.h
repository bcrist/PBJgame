#ifndef NET_ADDRESS_H_
#define NET_ADDRESS_H_

#include "pbj/_pbj.h"

namespace pbj
{
namespace net
{
	class Address
	{
	public:
		Address();
		Address(U8, U8, U8, U8, U16);
		Address(U32, U16);

		U32 getAddress() const;
		U8 getA() const;
		U8 getB() const;
		U8 getC() const;
		U8 getD() const;
		U16 getPort() const;

		bool operator==(const Address& other) const
		{
			return _address == other._address && _port == other._port;
		}

		bool operator!=(const Address& other) const
		{
			return !(*this==other);
		}

		//this is so we can use an Address as a key in a map
		bool operator<(const Address& other) const
		{
			if(_address < other._address)
				return true;
			if(_address > other._address)
				return false;
			else
				return _port < other._port;
		}

	private:
		U32 _address;
		U16 _port;
	};
}
}
#endif