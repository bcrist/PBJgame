////////////////////////////////////////////////////////////////////////////////
/// \file	Z:\Documents\PBJgame\include\pbj\net\net_listener.h
///
/// \brief	Declares the net listener class.
////////////////////////////////////////////////////////////////////////////////
#ifndef NET_LISTENER_H_
#define NET_LISTENER_H_

#include <assert.h>
#include <vector>
#include "pbj/_pbj.h"
#include "pbj/net/net_sockets.h"

using std::vector;

namespace pbj
{
namespace net
{
	////////////////////////////////////////////////////////////////////////////
	/// \struct	ListenerEntry
	///
	/// \brief	Listener entry.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	////////////////////////////////////////////////////////////////////////////
	struct ListenerEntry
	{
		I8 name[64+1];
		Address address;
		F32 timeoutAccumulator;
	};

	////////////////////////////////////////////////////////////////////////////
	/// \class	Listener
	///
	/// \brief	Listener.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-05
	////////////////////////////////////////////////////////////////////////////
	class Listener
	{
	public:
		Listener(U32, F32);
		Listener(U32);
		~Listener();

		bool start(I32);
		void stop();
		void update(F32);
		I32 getEntryCount() const;
		const ListenerEntry& getEntry(I32) const;

	protected:
		ListenerEntry* findEntry(const ListenerEntry&);

	private:
		void clearData();

		vector<ListenerEntry> _entries;
		U32 _protoId;
		F32 _timeout;
		bool _running;
		Socket _socket;
	};
}
}
#endif