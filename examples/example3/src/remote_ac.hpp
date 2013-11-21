#ifndef TUTORIAL_REMOTE_AC_HPP
#define TUTORIAL_REMOTE_AC_HPP
#include <remote/idl.hpp>


#define REMOTE_CLASS								\
REMOTE_CLASS_BEGIN(remote_ac)						\
	REMOTE_METHOD_M1(void, set_temperature, int)	\
	REMOTE_METHOD_C0(int, get_temperature)			\
REMOTE_CLASS_END
#include <remote/idl/class.hpp>

#define REMOTE_REGISTER_CLASS remote_ac
#include <remote/idl/register_class.hpp>


#endif // TUTORIAL_REMOTE_AC_HPP
