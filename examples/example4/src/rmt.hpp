#ifndef EXAMPLE_RMT_HPP
#define EXAMPLE_RMT_HPP
#include <remote/idl.hpp>
#include <string>


#define REMOTE_CLASS						\
REMOTE_CLASS_BEGIN(rmt)						\
REMOTE_METHOD_M1(void, invoke, std::string)	\
REMOTE_CLASS_END
#include <remote/idl/class.hpp>

#define REMOTE_REGISTER_CLASS rmt
#include <remote/idl/register_class.hpp>

#endif
