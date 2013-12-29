// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#define REMOTE_CLASS_BEGIN_IMPL(class_name)										\
	REMOTE_IMPLEMENT_CLASS::class_name()										\
	{}																			\
																				\
	REMOTE_IMPLEMENT_CLASS::class_name(::remote::detail::pool_link_weak_ptr const& link,\
										::remote::detail::object_vid const& id)	\
	: ::remote::detail::proxy(link, id)											\
	{}

#define REMOTE_CLASS_BODY_IMPL(class_name)

#define REMOTE_CLASS_BEGIN(class_name) REMOTE_CLASS_BEGIN_0(class_name)

#define REMOTE_CLASS_BEGIN_0(class_name)										\
REMOTE_CLASS_BEGIN_IMPL(class_name)

#define REMOTE_CLASS_BEGIN_1(class_name, B1)									\
REMOTE_CLASS_BEGIN_IMPL(class_name)

#define REMOTE_CLASS_BEGIN_2(class_name, B1, B2)								\
REMOTE_CLASS_BEGIN_IMPL(class_name)

#define REMOTE_CLASS_BEGIN_3(class_name, B1, B2, B3)							\
REMOTE_CLASS_BEGIN_IMPL(class_name)


#define REMOTE_CLASS_END


#define REMOTE_METHOD_M0(R, method_name)																		\
	R REMOTE_IMPLEMENT_CLASS::method_name()																		\
	{																											\
		return ::remote::detail::proxy::invoke<method_name##_m0>()->get();										\
	}

#define REMOTE_METHOD_M1(R, method_name, A1)																	\
	R REMOTE_IMPLEMENT_CLASS::method_name(A1 a1)																\
	{																											\
		return ::remote::detail::proxy::invoke<method_name##_m1>(a1)->get();									\
	}

#define REMOTE_METHOD_M2(R, method_name, A1, A2)																\
	R REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2)															\
	{																											\
		return ::remote::detail::proxy::invoke<method_name##_m2>(a1, a2)->get();								\
	}

#define REMOTE_METHOD_M3(R, method_name, A1, A2, A3)															\
	R REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2, A3 a3)													\
	{																											\
		return ::remote::detail::proxy::invoke<method_name##_m3>(a1, a2, a3)->get();							\
	}

#define REMOTE_METHOD_M4(R, method_name, A1, A2, A3, A4)														\
	R REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2, A3 a3, A4 a4)											\
	{																											\
		return ::remote::detail::proxy::invoke<method_name##_m4>(a1, a2, a3, a4)->get();						\
	}

#define REMOTE_METHOD_M5(R, method_name, A1, A2, A3, A4, A5)													\
	R REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)									\
	{																											\
		return ::remote::detail::proxy::invoke<method_name##_m5>(a1, a2, a3, a4, a5)->get();					\
	}

#define REMOTE_METHOD_M6(R, method_name, A1, A2, A3, A4, A5, A6)												\
	R REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)								\
	{																											\
		return ::remote::detail::proxy::invoke<method_name##_m6>(a1, a2, a3, a4, a5, a6)->get();				\
	}


#define REMOTE_METHOD_C0(R, method_name)																		\
	R REMOTE_IMPLEMENT_CLASS::method_name() const																\
	{																											\
		this_type* _this = const_cast<this_type*>(this);														\
		return _this->::remote::detail::proxy::invoke<method_name##_c0>()->get();								\
	}

#define REMOTE_METHOD_C1(R, method_name, A1)																	\
	R REMOTE_IMPLEMENT_CLASS::method_name(A1 a1) const															\
	{																											\
		this_type* _this = const_cast<this_type*>(this);														\
		return _this->::remote::detail::proxy::invoke<method_name##_c1>(a1)->get();								\
	}

#define REMOTE_METHOD_C2(R, method_name, A1, A2)																\
	R REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2) const													\
	{																											\
		this_type* _this = const_cast<this_type*>(this);														\
		return _this->::remote::detail::proxy::invoke<method_name##_c2>(a1, a2)->get();							\
	}

#define REMOTE_METHOD_C3(R, method_name, A1, A2, A3)															\
	R REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2, A3 a3) const											\
	{																											\
		this_type* _this = const_cast<this_type*>(this);														\
		return _this->::remote::detail::proxy::invoke<method_name##_c3>(a1, a2, a3)->get();						\
	}

#define REMOTE_METHOD_C4(R, method_name, A1, A2, A3, A4)														\
	R REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2, A3 a3, A4 a4) const										\
	{																											\
		this_type* _this = const_cast<this_type*>(this);														\
		return _this->::remote::detail::proxy::invoke<method_name##_c4>(a1, a2, a3, a4)->get();					\
	}

#define REMOTE_METHOD_C5(R, method_name, A1, A2, A3, A4, A5)													\
	R REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const								\
	{																											\
		this_type* _this = const_cast<this_type*>(this);														\
		return _this->::remote::detail::proxy::invoke<method_name##_c5>(a1, a2, a3, a4, a5)->get();				\
	}

#define REMOTE_METHOD_C6(R, method_name, A1, A2, A3, A4, A5, A6)												\
	R REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const						\
	{																											\
		this_type* _this = const_cast<this_type*>(this);														\
		return _this->::remote::detail::proxy::invoke<method_name##_c6>(a1, a2, a3, a4, a5, a6)->get();			\
	}


#define REMOTE_ASYNC_M0(R, method_name)																	\
	::remote::future<R> REMOTE_IMPLEMENT_CLASS::method_name()											\
	{																											\
		return ::remote::future<R>(::remote::detail::proxy::invoke<method_name##_m0>());					\
	}

#define REMOTE_ASYNC_M1(R, method_name, A1)																\
	::remote::future<R> REMOTE_IMPLEMENT_CLASS::method_name(A1 a1)										\
	{																											\
		return ::remote::future<R>(::remote::detail::proxy::invoke<method_name##_m1>(a1));				\
	}

#define REMOTE_ASYNC_M2(R, method_name, A1, A2)															\
	::remote::future<R> REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2)								\
	{																											\
		return ::remote::future<R>(::remote::detail::proxy::invoke<method_name##_m2>(a1, a2));			\
	}

#define REMOTE_ASYNC_M3(R, method_name, A1, A2, A3)														\
	::remote::future<R> REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2, A3 a3)							\
	{																											\
		return ::remote::future<R>(::remote::detail::proxy::invoke<method_name##_m3>(a1, a2, a3));		\
	}

#define REMOTE_ASYNC_M4(R, method_name, A1, A2, A3, A4)													\
	::remote::future<R> REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2, A3 a3, A4 a4)					\
	{																											\
		return ::remote::future<R>(::remote::detail::proxy::invoke<method_name##_m4>(a1, a2, a3, a4));	\
	}

#define REMOTE_ASYNC_M5(R, method_name, A1, A2, A3, A4, A5)												\
	::remote::future<R> REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)			\
	{																											\
		return ::remote::future<R>(::remote::detail::proxy::invoke<method_name##_m5>(a1, a2, a3, a4, a5));	\
	}

#define REMOTE_ASYNC_M6(R, method_name, A1, A2, A3, A4, A5, A6)											\
	::remote::future<R> REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)	\
	{																											\
		return ::remote::future<R>(::remote::detail::proxy::invoke<method_name##_m6>(a1, a2, a3, a4, a5, a6));	\
	}


#define REMOTE_ASYNC_C0(R, method_name)																	\
	::remote::future<R> REMOTE_IMPLEMENT_CLASS::method_name() const										\
	{																											\
		this_type* _this = const_cast<this_type*>(this);														\
		return ::remote::future<R>(_this->::remote::detail::proxy::invoke<method_name##_c0>());			\
	}

#define REMOTE_ASYNC_C1(R, method_name, A1)																\
	::remote::future<R> REMOTE_IMPLEMENT_CLASS::method_name(A1 a1) const									\
	{																											\
		this_type* _this = const_cast<this_type*>(this);														\
		return ::remote::future<R>(_this->::remote::detail::proxy::invoke<method_name##_c1>(a1));		\
	}

#define REMOTE_ASYNC_C2(R, method_name, A1, A2)															\
	::remote::future<R> REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2) const							\
	{																											\
		this_type* _this = const_cast<this_type*>(this);														\
		return ::remote::future<R>(_this->::remote::detail::proxy::invoke<method_name##_c2>(a1, a2));	\
	}

#define REMOTE_ASYNC_C3(R, method_name, A1, A2, A3)														\
	::remote::future<R> REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2, A3 a3) const					\
	{																											\
		this_type* _this = const_cast<this_type*>(this);														\
		return ::remote::future<R>(_this->::remote::detail::proxy::invoke<method_name##_c3>(a1, a2, a3));	\
	}

#define REMOTE_ASYNC_C4(R, method_name, A1, A2, A3, A4)													\
	::remote::future<R> REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2, A3 a3, A4 a4) const			\
	{																											\
		this_type* _this = const_cast<this_type*>(this);														\
		return ::remote::future<R>(_this->::remote::detail::proxy::invoke<method_name##_c4>(a1, a2, a3, a4));	\
	}

#define REMOTE_ASYNC_C5(R, method_name, A1, A2, A3, A4, A5)												\
	::remote::future<R> REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const		\
	{																											\
		this_type* _this = const_cast<this_type*>(this);														\
		return ::remote::future<R>(_this->::remote::detail::proxy::invoke<method_name##_c5>(a1, a2, a3, a4, a5));	\
	}

#define REMOTE_ASYNC_C6(R, method_name, A1, A2, A3, A4, A5, A6)											\
	::remote::future<R> REMOTE_IMPLEMENT_CLASS::method_name(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const	\
	{																											\
		this_type* _this = const_cast<this_type*>(this);														\
		return ::remote::future<R>(_this->::remote::detail::proxy::invoke<method_name##_c6>(a1, a2, a3, a4, a5, a6));	\
	}


REMOTE_CLASS

#include <remote/idl/detail/undefine.ipp>
