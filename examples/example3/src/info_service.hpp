#ifndef TUTORIAL_INFO_SERVICE_HPP
#define TUTORIAL_INFO_SERVICE_HPP
#include <remote/idl.hpp>


#define REMOTE_CLASS							\
REMOTE_CLASS_BEGIN(info_service)				\
	REMOTE_METHOD_C0(std::string, name)			\
REMOTE_CLASS_END
#include <remote/idl/class.hpp>

#define REMOTE_REGISTER_CLASS info_service
#include <remote/idl/register_class.hpp>


#endif // TUTORIAL_INFO_SERVICE_HPP
