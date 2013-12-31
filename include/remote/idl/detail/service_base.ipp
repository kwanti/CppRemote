// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#define REMOTE_CLASS_BEGIN_IMPL(class_name)									\
class class_name##_service_base												\
: public ::remote::detail::service

#define REMOTE_CLASS_BODY_IMPL(class_name)									\
{																			\
	typedef class_name##_service_base this_type;							\
																			\
public:																		\
	class_name##_service_base()												\
	{}																		\
																			\
	class_name##_service_base(::remote::detail::object_id const& id)		\
	: ::remote::detail::service(id)											\
	{}


#define REMOTE_CLASS_BEGIN(class_name) REMOTE_CLASS_BEGIN_0(class_name)


#define REMOTE_CLASS_BEGIN_0(class_name)									\
REMOTE_CLASS_BEGIN_IMPL(class_name)											\
REMOTE_CLASS_BODY_IMPL(class_name)


#define REMOTE_CLASS_BEGIN_1(class_name, B1)								\
REMOTE_CLASS_BEGIN_IMPL(class_name)											\
REMOTE_CLASS_BODY_IMPL(class_name)


#define REMOTE_CLASS_BEGIN_2(class_name, B1, B2)							\
REMOTE_CLASS_BEGIN_IMPL(class_name)											\
REMOTE_CLASS_BODY_IMPL(class_name)


#define REMOTE_CLASS_BEGIN_3(class_name, B1, B2, B3)						\
REMOTE_CLASS_BEGIN_IMPL(class_name)											\
REMOTE_CLASS_BODY_IMPL(class_name)


#define REMOTE_CLASS_END													\
};


#define REMOTE_METHOD_M0(R, method_name)									\
	struct method_name##_tag_m0{};											\
	typedef ::remote::detail::call_0<this_type, method_name##_tag_m0, R> method_name##_m0;	\
	virtual void invoke(::remote::detail::pool_node&, method_name##_m0&) = 0;

#define REMOTE_METHOD_M1(R, method_name, A1)								\
	struct method_name##_tag_m1{};											\
	typedef ::remote::detail::call_1<this_type, method_name##_tag_m1, R, A1> method_name##_m1;	\
	virtual void invoke(::remote::detail::pool_node&, method_name##_m1&) = 0;

#define REMOTE_METHOD_M2(R, method_name, A1, A2)							\
	struct method_name##_tag_m2{};											\
	typedef ::remote::detail::call_2<this_type, method_name##_tag_m2, R, A1, A2> method_name##_m2;	\
	virtual void invoke(::remote::detail::pool_node&, method_name##_m2&) = 0;

#define REMOTE_METHOD_M3(R, method_name, A1, A2, A3)						\
	struct method_name##_tag_m3{};											\
	typedef ::remote::detail::call_3<this_type, method_name##_tag_m3, R, A1, A2, A3> method_name##_m3;	\
	virtual void invoke(::remote::detail::pool_node&, method_name##_m3&) = 0;

#define REMOTE_METHOD_M4(R, method_name, A1, A2, A3, A4)					\
	struct method_name##_tag_m4{};											\
	typedef ::remote::detail::call_4<this_type, method_name##_tag_m4, R, A1, A2, A3, A4> method_name##_m4;	\
	virtual void invoke(::remote::detail::pool_node&, method_name##_m4&) = 0;

#define REMOTE_METHOD_M5(R, method_name, A1, A2, A3, A4, A5)				\
	struct method_name##_tag_m5{};											\
	typedef ::remote::detail::call_5<this_type, method_name##_tag_m5, R, A1, A2, A3, A4, A5> method_name##_m5;	\
	virtual void invoke(::remote::detail::pool_node&, method_name##_m5&) = 0;

#define REMOTE_METHOD_M6(R, method_name, A1, A2, A3, A4, A5, A6)			\
	struct method_name##_tag_m6{};											\
	typedef ::remote::detail::call_6<this_type, method_name##_tag_m6, R, A1, A2, A3, A4, A5, A6> method_name##_m6;	\
	virtual void invoke(::remote::detail::pool_node&, method_name##_m6&) = 0;


#define REMOTE_METHOD_C0(R, method_name)									\
	struct method_name##_tag_c0;											\
	typedef ::remote::detail::call_0<this_type, method_name##_tag_c0, R> method_name##_c0;	\
	virtual void invoke(::remote::detail::pool_node&, method_name##_c0&) = 0;

#define REMOTE_METHOD_C1(R, method_name, A1)								\
	struct method_name##_tag_c1{};											\
	typedef ::remote::detail::call_1<this_type, method_name##_tag_c1, R, A1> method_name##_c1;	\
	virtual void invoke(::remote::detail::pool_node&, method_name##_c1&) = 0;

#define REMOTE_METHOD_C2(R, method_name, A1, A2)							\
	struct method_name##_tag_c2{};											\
	typedef ::remote::detail::call_2<this_type, method_name##_tag_c2, R, A1, A2> method_name##_c2;	\
	virtual void invoke(::remote::detail::pool_node&, method_name##_c2&) = 0;

#define REMOTE_METHOD_C3(R, method_name, A1, A2, A3)						\
	struct method_name##_tag_c3{};											\
	typedef ::remote::detail::call_3<this_type, method_name##_tag_c3, R, A1, A2, A3> method_name##_c3;	\
	virtual void invoke(::remote::detail::pool_node&, method_name##_c3&) = 0;

#define REMOTE_METHOD_C4(R, method_name, A1, A2, A3, A4)					\
	struct method_name##_tag_c4{};											\
	typedef ::remote::detail::call_4<this_type, method_name##_tag_c4, R, A1, A2, A3, A4> method_name##_c4;	\
	virtual void invoke(::remote::detail::pool_node&, method_name##_c4&) = 0;

#define REMOTE_METHOD_C5(R, method_name, A1, A2, A3, A4, A5)				\
	struct method_name##_tag_c5{};											\
	typedef ::remote::detail::call_5<this_type, method_name##_tag_c5, R, A1, A2, A3, A4, A5> method_name##_c5;	\
	virtual void invoke(::remote::detail::pool_node&, method_name##_c5&) = 0;

#define REMOTE_METHOD_C6(R, method_name, A1, A2, A3, A4, A5, A6)			\
	struct method_name##_tag_c6{};											\
	typedef ::remote::detail::call_6<this_type, method_name##_tag_c6, R, A1, A2, A3, A4, A5, A6> method_name##_c6;	\
	virtual void invoke(::remote::detail::pool_node&, method_name##_c6&) = 0;


#define REMOTE_ASYNC_M0(R, method_name)							\
		REMOTE_METHOD_M0(R, method_name)

#define REMOTE_ASYNC_M1(R, method_name, A1)						\
		REMOTE_METHOD_M1(R, method_name, A1)

#define REMOTE_ASYNC_M2(R, method_name, A1, A2)					\
		REMOTE_METHOD_M2(R, method_name, A1, A2)

#define REMOTE_ASYNC_M3(R, method_name, A1, A2, A3)				\
		REMOTE_METHOD_M3(R, method_name, A1, A2, A3)

#define REMOTE_ASYNC_M4(R, method_name, A1, A2, A3, A4)			\
		REMOTE_METHOD_M4(R, method_name, A1, A2, A3, A4)

#define REMOTE_ASYNC_M5(R, method_name, A1, A2, A3, A4, A5)		\
		REMOTE_METHOD_M5(R, method_name, A1, A2, A3, A4, A5)

#define REMOTE_ASYNC_M6(R, method_name, A1, A2, A3, A4, A5, A6)	\
		REMOTE_METHOD_M6(R, method_name, A1, A2, A3, A4, A5, A6)


#define REMOTE_ASYNC_C0(R, method_name)							\
		REMOTE_METHOD_C0(R, method_name)

#define REMOTE_ASYNC_C1(R, method_name, A1)						\
		REMOTE_METHOD_C1(R, method_name, A1)

#define REMOTE_ASYNC_C2(R, method_name, A1, A2)					\
		REMOTE_METHOD_C2(R, method_name, A1, A2)

#define REMOTE_ASYNC_C3(R, method_name, A1, A2, A3)				\
		REMOTE_METHOD_C3(R, method_name, A1, A2, A3)

#define REMOTE_ASYNC_C4(R, method_name, A1, A2, A3, A4)			\
		REMOTE_METHOD_C4(R, method_name, A1, A2, A3, A4)

#define REMOTE_ASYNC_C5(R, method_name, A1, A2, A3, A4, A5)		\
		REMOTE_METHOD_C5(R, method_name, A1, A2, A3, A4, A5)

#define REMOTE_ASYNC_C6(R, method_name, A1, A2, A3, A4, A5, A6)	\
		REMOTE_METHOD_C6(R, method_name, A1, A2, A3, A4, A5, A6)


REMOTE_CLASS

#include <remote/idl/detail/undefine.ipp>
