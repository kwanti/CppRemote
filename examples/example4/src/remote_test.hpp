#ifndef EXAMPLE_REMOTE_TEST_HPP
#define EXAMPLE_REMOTE_TEST_HPP

#include <remote/idl.hpp>
#include <remote/vector.hpp>
#include <remote/shared_ptr.hpp>

#include "rmt.hpp"


#define REMOTE_CLASS														\
REMOTE_CLASS_BEGIN(remote_test)												\
REMOTE_METHOD_M1(void, set_rmt_ptr, boost::shared_ptr<rmt>)					\
REMOTE_METHOD_M0(boost::shared_ptr<rmt>, get_rmt_ptr)						\
REMOTE_METHOD_M1(void, invoke_all, std::vector<boost::shared_ptr<rmt> >)	\
REMOTE_CLASS_END
#include <remote/idl/class.hpp>


#define REMOTE_REGISTER_CLASS remote_test
#include <remote/idl/register_class.hpp>


#endif // EXAMPLE_REMOTE_TEST_HPP
