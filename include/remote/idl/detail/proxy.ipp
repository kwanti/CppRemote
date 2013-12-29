// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------


#define REMOTE_CLASS_BEGIN_IMPL(class_name)										\
class class_name																\
: public ::remote::detail::proxy

#define REMOTE_CLASS_BODY_IMPL(class_name)										\
{																				\
	typedef class_name this_type;												\
	typedef class_name##_service_base service_type;								\
	friend class ::boost::serialization::access;								\
																				\
	template<typename Archive>													\
	void serialize(Archive& ar, unsigned int const version)						\
	{																			\
		::remote::detail::proxy::serialize(ar, version);						\
	}																			\
																				\
public:																			\
	class_name();																\
	class_name(::remote::detail::pool_link_weak_ptr const& link, ::remote::detail::object_vid const& id);


#define REMOTE_CLASS_BEGIN(class_name) REMOTE_CLASS_BEGIN_0(class_name)


#define REMOTE_CLASS_BEGIN_0(class_name)										\
REMOTE_CLASS_BEGIN_IMPL(class_name)												\
REMOTE_CLASS_BODY_IMPL(class_name)


#define REMOTE_CLASS_BEGIN_1(class_name, B1)									\
REMOTE_CLASS_BEGIN_IMPL(class_name)												\
, public B1																		\
REMOTE_CLASS_BODY_IMPL(class_name)


#define REMOTE_CLASS_BEGIN_2(class_name, B1, B2)								\
REMOTE_CLASS_BEGIN_IMPL(class_name)												\
, public B1, public B2															\
REMOTE_CLASS_BODY_IMPL(class_name)


#define REMOTE_CLASS_BEGIN_3(class_name, B1, B2, B3)							\
REMOTE_CLASS_BEGIN_IMPL(class_name)												\
, public B1, public B2, public B3												\
REMOTE_CLASS_BODY_IMPL(class_name)


#define REMOTE_CLASS_END														\
};


#define REMOTE_METHOD_M0(R, method_name)										\
	typedef service_type::method_name##_m0 method_name##_m0;					\
	R method_name();

#define REMOTE_METHOD_M1(R, method_name, A1)									\
	typedef service_type::method_name##_m1 method_name##_m1;					\
	R method_name(A1 a1);

#define REMOTE_METHOD_M2(R, method_name, A1, A2)								\
	typedef service_type::method_name##_m2 method_name##_m2;					\
	R method_name(A1 a1, A2 a2);

#define REMOTE_METHOD_M3(R, method_name, A1, A2, A3)							\
	typedef service_type::method_name##_m3 method_name##_m3;					\
	R method_name(A1 a1, A2 a2, A3 a3);

#define REMOTE_METHOD_M4(R, method_name, A1, A2, A3, A4)						\
	typedef service_type::method_name##_m4 method_name##_m4;					\
	R method_name(A1 a1, A2 a2, A3 a3, A4 a4);

#define REMOTE_METHOD_M5(R, method_name, A1, A2, A3, A4, A5)					\
	typedef service_type::method_name##_m5 method_name##_m5;					\
	R method_name(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5);

#define REMOTE_METHOD_M6(R, method_name, A1, A2, A3, A4, A5, A6)				\
	typedef service_type::method_name##_m6 method_name##_m6;					\
	R method_name(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6);


#define REMOTE_METHOD_C0(R, method_name)										\
	typedef service_type::method_name##_c0 method_name##_c0;					\
	R method_name() const;

#define REMOTE_METHOD_C1(R, method_name, A1)									\
	typedef service_type::method_name##_c1 method_name##_c1;					\
	R method_name(A1 a1) const;

#define REMOTE_METHOD_C2(R, method_name, A1, A2)								\
	typedef service_type::method_name##_c2 method_name##_c2;					\
	R method_name(A1 a1, A2 a2) const;

#define REMOTE_METHOD_C3(R, method_name, A1, A2, A3)							\
	typedef service_type::method_name##_c3 method_name##_c3;					\
	R method_name(A1 a1, A2 a2, A3 a3) const;

#define REMOTE_METHOD_C4(R, method_name, A1, A2, A3, A4)						\
	typedef service_type::method_name##_c4 method_name##_c4;					\
	R method_name(A1 a1, A2 a2, A3 a3, A4 a4) const;

#define REMOTE_METHOD_C5(R, method_name, A1, A2, A3, A4, A5)					\
	typedef service_type::method_name##_c5 method_name##_c5;					\
	R method_name(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const;

#define REMOTE_METHOD_C6(R, method_name, A1, A2, A3, A4, A5, A6)				\
	typedef service_type::method_name##_c6 method_name##_c6;					\
	R method_name(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const;


#define REMOTE_ASYNC_M0(R, method_name)									\
	typedef service_type::method_name##_m0 method_name##_m0;					\
	::remote::future<R> method_name();

#define REMOTE_ASYNC_M1(R, method_name, A1)								\
	typedef service_type::method_name##_m1 method_name##_m1;					\
	::remote::future<R> method_name(A1 a1);

#define REMOTE_ASYNC_M2(R, method_name, A1, A2)							\
	typedef service_type::method_name##_m2 method_name##_m2;					\
	::remote::future<R> method_name(A1 a1, A2 a2);

#define REMOTE_ASYNC_M3(R, method_name, A1, A2, A3)						\
	typedef service_type::method_name##_m3 method_name##_m3;					\
	::remote::future<R> method_name(A1 a1, A2 a2, A3 a3);

#define REMOTE_ASYNC_M4(R, method_name, A1, A2, A3, A4)					\
	typedef service_type::method_name##_m4 method_name##_m4;					\
	::remote::future<R> method_name(A1 a1, A2 a2, A3 a3, A4 a4);

#define REMOTE_ASYNC_M5(R, method_name, A1, A2, A3, A4, A5)				\
	typedef service_type::method_name##_m5 method_name##_m5;					\
	::remote::future<R> method_name(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5);

#define REMOTE_ASYNC_M6(R, method_name, A1, A2, A3, A4, A5, A6)			\
	typedef service_type::method_name##_m6 method_name##_m6;					\
	::remote::future<R> method_name(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6);


#define REMOTE_ASYNC_C0(R, method_name)									\
	typedef service_type::method_name##_c0 method_name##_c0;					\
	::remote::future<R> method_name() const;

#define REMOTE_ASYNC_C1(R, method_name, A1)								\
	typedef service_type::method_name##_c1 method_name##_c1;					\
	::remote::future<R> method_name(A1 a1) const;

#define REMOTE_ASYNC_C2(R, method_name, A1, A2)							\
	typedef service_type::method_name##_c2 method_name##_c2;					\
	::remote::future<R> method_name(A1 a1, A2 a2) const;

#define REMOTE_ASYNC_C3(R, method_name, A1, A2, A3)						\
	typedef service_type::method_name##_c3 method_name##_c3;					\
	::remote::future<R> method_name(A1 a1, A2 a2, A3 a3) const;

#define REMOTE_ASYNC_C4(R, method_name, A1, A2, A3, A4)					\
	typedef service_type::method_name##_c4 method_name##_c4;					\
	::remote::future<R> method_name(A1 a1, A2 a2, A3 a3, A4 a4) const;

#define REMOTE_ASYNC_C5(R, method_name, A1, A2, A3, A4, A5)				\
	typedef service_type::method_name##_c5 method_name##_c5;					\
	::remote::future<R> method_name(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const;

#define REMOTE_ASYNC_C6(R, method_name, A1, A2, A3, A4, A5, A6)			\
	typedef service_type::method_name##_c6 method_name##_c6;					\
	::remote::future<R> method_name(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const;


REMOTE_CLASS

#include <remote/idl/detail/undefine.ipp>

