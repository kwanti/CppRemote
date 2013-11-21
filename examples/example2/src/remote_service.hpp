#ifndef EXAMPLE_REMOTE_SERVICE_HPP
#define EXAMPLE_REMOTE_SERVICE_HPP

#include "person.hpp"

#include <remote/idl.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>


#define REMOTE_CLASS													\
REMOTE_CLASS_BEGIN(remote_service)										\
	REMOTE_METHOD_M1(void, pass_person, person)							\
	REMOTE_METHOD_M1(void, pass_people, std::vector<person>)			\
	REMOTE_METHOD_M1(void, pass_reference, person&)						\
	REMOTE_METHOD_M1(void, pass_pointer, person*)						\
	REMOTE_METHOD_M1(void, pass_shared_ptr, boost::shared_ptr<person>)	\
	REMOTE_METHOD_M1(void, pass_weak_ptr, boost::weak_ptr<person>)		\
REMOTE_CLASS_END
#include <remote/idl/class.hpp>

#define REMOTE_REGISTER_CLASS remote_service
#include <remote/idl/register_class.hpp>


#endif
