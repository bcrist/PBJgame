/*
	Simple Network Library from "Networking for Game Programmers"
	http://www.gaffer.org/networking-for-game-programmers
	Author: Glenn Fiedler <gaffer@gaffer.org>
*/

#ifndef NET_PLATFORM_H_
#define NET_PLATFORM_H_

#include <assert.h>
#include <stdio.h>
#include <Windows.h>
#include "pbj/_pbj.h"

namespace pbj
{
namespace net
{
	/// \b wait for n seconds
	inline void waitSeconds(float n)
	{
		Sleep( (int) ( n * 1000.0f ) );
	}
}
}
#endif
