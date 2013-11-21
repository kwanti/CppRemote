#ifndef TUTORIAL_REMOTE_PLAYER_HPP
#define TUTORIAL_REMOTE_PLAYER_HPP
#include <remote/idl.hpp>


#define REMOTE_CLASS							\
REMOTE_CLASS_BEGIN(remote_player)				\
	REMOTE_METHOD_M1(void, play, std::string)	\
	REMOTE_METHOD_M0(void, stop)				\
REMOTE_CLASS_END
#include <remote/idl/class.hpp>

#define REMOTE_REGISTER_CLASS remote_player
#include <remote/idl/register_class.hpp>


#endif // TUTORIAL_REMOTE_PLAYER_HPP
