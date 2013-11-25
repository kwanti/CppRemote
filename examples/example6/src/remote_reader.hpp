#ifndef EXAMPLE_REMOTE_READER_HPP
#define EXAMPLE_REMOTE_READER_HPP

#include <remote/idl.hpp>
#include <string>

#define REMOTE_CLASS							\
REMOTE_CLASS_BEGIN(remote_reader)				\
REMOTE_METHOD_M1(void, on_notify, std::string)	\
REMOTE_CLASS_END
#include <remote/idl/class.hpp>

#define REMOTE_REGISTER_CLASS remote_reader
#include <remote/idl/register_class.hpp>

#endif
