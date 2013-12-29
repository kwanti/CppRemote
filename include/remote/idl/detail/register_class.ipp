// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#define REMOTE_CLASS_BEGIN(class_name)
#define REMOTE_CLASS_BEGIN_0(class_name)
#define REMOTE_CLASS_BEGIN_1(class_name, B1)
#define REMOTE_CLASS_BEGIN_2(class_name, B1, B2)
#define REMOTE_CLASS_BEGIN_3(class_name, B1, B2, B3)

#define REMOTE_CLASS_BEGIN_IMPL(class_name)
#define REMOTE_CLASS_BODY_IMPL(class_name)

#define REMOTE_CLASS_END													\
namespace remote															\
{																			\
namespace detail															\
{																			\
																			\
template<typename U>														\
struct to_service<REMOTE_REGISTER_CLASS, U>									\
{																			\
	typedef BOOST_PP_CAT(REMOTE_REGISTER_CLASS, _service<U>) type;			\
};																			\
																			\
}																			\
}


#define REMOTE_METHOD_M0(R, method_name)									\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_m0)

#define REMOTE_METHOD_M1(R, method_name, A1)								\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_m1)

#define REMOTE_METHOD_M2(R, method_name, A1, A2)							\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_m2)

#define REMOTE_METHOD_M3(R, method_name, A1, A2, A3)						\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_m3)

#define REMOTE_METHOD_M4(R, method_name, A1, A2, A3, A4)					\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_m4)

#define REMOTE_METHOD_M5(R, method_name, A1, A2, A3, A4, A5)				\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_m5)

#define REMOTE_METHOD_M6(R, method_name, A1, A2, A3, A4, A5, A6)			\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_m6)


#define REMOTE_METHOD_C0(R, method_name)									\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_c0)

#define REMOTE_METHOD_C1(R, method_name, A1)								\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_c1)

#define REMOTE_METHOD_C2(R, method_name, A1, A2)							\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_c2)

#define REMOTE_METHOD_C3(R, method_name, A1, A2, A3)						\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_c3)

#define REMOTE_METHOD_C4(R, method_name, A1, A2, A3, A4)					\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_c4)

#define REMOTE_METHOD_C5(R, method_name, A1, A2, A3, A4, A5)				\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_c5)

#define REMOTE_METHOD_C6(R, method_name, A1, A2, A3, A4, A5, A6)			\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_c6)


#define REMOTE_ASYNC_M0(R, method_name)								\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_m0)

#define REMOTE_ASYNC_M1(R, method_name, A1)							\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_m1)

#define REMOTE_ASYNC_M2(R, method_name, A1, A2)						\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_m2)

#define REMOTE_ASYNC_M3(R, method_name, A1, A2, A3)					\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_m3)

#define REMOTE_ASYNC_M4(R, method_name, A1, A2, A3, A4)				\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_m4)

#define REMOTE_ASYNC_M5(R, method_name, A1, A2, A3, A4, A5)			\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_m5)

#define REMOTE_ASYNC_M6(R, method_name, A1, A2, A3, A4, A5, A6)		\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_m6)


#define REMOTE_ASYNC_C0(R, method_name)								\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_c0)

#define REMOTE_ASYNC_C1(R, method_name, A1)							\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_c1)

#define REMOTE_ASYNC_C2(R, method_name, A1, A2)						\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_c2)

#define REMOTE_ASYNC_C3(R, method_name, A1, A2, A3)					\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_c3)

#define REMOTE_ASYNC_C4(R, method_name, A1, A2, A3, A4)				\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_c4)

#define REMOTE_ASYNC_C5(R, method_name, A1, A2, A3, A4, A5)			\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_c5)

#define REMOTE_ASYNC_C6(R, method_name, A1, A2, A3, A4, A5, A6)		\
REMOTE_REGISTER_CALL(REMOTE_REGISTER_CLASS::method_name##_c6)


REMOTE_CLASS

#include <remote/idl/detail/undefine.ipp>
