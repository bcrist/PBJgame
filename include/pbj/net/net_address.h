////////////////////////////////////////////////////////////////////////////////
/// \file	Z:\Documents\PBJgame\include\pbj\net\net_address.h
///
/// \brief	Declares the net address class.
////////////////////////////////////////////////////////////////////////////////
#ifndef NET_ADDRESS_H_
#define NET_ADDRESS_H_

#include <iostream>
#include "pbj/_pbj.h"

using std::ostream;

namespace pbj
{
namespace net
{
	////////////////////////////////////////////////////////////////////////////
	/// \class	Address
	///
	/// \brief	Address.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	////////////////////////////////////////////////////////////////////////////
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

		////////////////////////////////////////////////////////////////////////
		/// \fn	bool Address::operator==(const Address& other) const
		///
		/// \brief	Equality operator.
		///
		/// \author	Peter Bartosch
		/// \date	2013-08-05
		///
		/// \param	other	The other.
		///
		/// \return	true if the parameters are considered equivalent.
		////////////////////////////////////////////////////////////////////////
		bool operator==(const Address& other) const
		{
			return _address == other._address && _port == other._port;
		}

		////////////////////////////////////////////////////////////////////////
		/// \fn	bool Address::operator!=(const Address& other) const
		///
		/// \brief	Inequality operator.
		///
		/// \author	Peter Bartosch
		/// \date	2013-08-05
		///
		/// \param	other	The other.
		///
		/// \return	true if the parameters are not considered equivalent.
		////////////////////////////////////////////////////////////////////////
		bool operator!=(const Address& other) const
		{
			return !(*this==other);
		}

		////////////////////////////////////////////////////////////////////////
		/// \fn	bool Address::operator<(const Address& other) const
		///
		/// \brief	Less-than comparison operator.
		///
		/// \author	Peter Bartosch
		/// \date	2013-08-05
		///
		/// \param	other	The other.
		///
		/// \return	true if the first parameter is less than the second.
		/// \details This is so we can use an Address as the key in a map.
		////////////////////////////////////////////////////////////////////////
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
		/// \brief	The address.
		U32 _address;
		/// \brief	The port.
		U16 _port;
	};

	////////////////////////////////////////////////////////////////////////////
	/// \fn	inline ostream& operator<<(ostream& os, const Address& addr)
	///
	/// \brief	Stream insertion operator.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	///
	/// \param [in,out]	os	The operating system.
	/// \param	addr	  	The address.
	///
	/// \return	The shifted result.
	////////////////////////////////////////////////////////////////////////////
	inline ostream& operator<<(ostream& os, const Address& addr)
	{
		os << (U32)addr.getA() << '.' << (U32)addr.getB() << '.' << (U32)addr.getC() << '.'
			<< (U32)addr.getD() << ':' << addr.getPort();
		return os;
	}
}
}
#endif