#ifndef EXAMPLE_REMOTE_FACTORY_HPP
#define EXAMPLE_REMOTE_FACTORY_HPP

#include "remote_player.hpp"
#include <remote/set.hpp>


#define REMOTE_CLASS 									\
REMOTE_CLASS_BEGIN(remote_factory)						\
REMOTE_METHOD_M1(remote_player*, create, std::string)	\
REMOTE_METHOD_M1(void, destroy, remote_player*)			\
REMOTE_METHOD_C0(std::set<remote_player*>, all_player)	\
REMOTE_CLASS_END
#include <remote/idl/class.hpp>

#define REMOTE_REGISTER_CLASS remote_factory
#include <remote/idl/register_class.hpp>

#endif
