#ifndef EXAMPLE_REMOTE_TOPIC_HPP
#define EXAMPLE_REMOTE_TOPIC_HPP

#include "remote_reader.hpp"
#include <remote/shared_ptr.hpp>

#define REMOTE_CLASS													\
REMOTE_CLASS_BEGIN(remote_topic)										\
REMOTE_METHOD_C0(std::string, about)									\
REMOTE_METHOD_M1(void, post, std::string)								\
REMOTE_METHOD_M1(void, subscribe, boost::shared_ptr<remote_reader>)		\
REMOTE_METHOD_M1(void, unsubscribe, boost::shared_ptr<remote_reader>)	\
REMOTE_CLASS_END
#include <remote/idl/class.hpp>

#define REMOTE_REGISTER_CLASS remote_topic
#include <remote/idl/register_class.hpp>

#endif
