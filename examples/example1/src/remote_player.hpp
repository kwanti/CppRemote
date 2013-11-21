#ifndef EXAMPLE_REMOTE_PLAYER_HPP
#define EXAMPLE_REMOTE_PLAYER_HPP

#include <remote/idl.hpp>

namespace example
{
#define REMOTE_CLASS									\
REMOTE_CLASS_BEGIN(remote_player)						\
	REMOTE_METHOD_M1(void, play, std::string const&)	\
	REMOTE_METHOD_M0(void, stop)						\
	REMOTE_METHOD_C0(bool, is_playing)					\
	REMOTE_ASYNC_M1(int, rewind, int)					\
REMOTE_CLASS_END
#include <remote/idl/class.hpp>
}

#define REMOTE_REGISTER_CLASS example::remote_player
#include <remote/idl/register_class.hpp>

#endif
