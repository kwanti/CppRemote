// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#define REMOTE_CLASS_BEGIN_IMPL(class_name)											\
template<typename Target>															\
class class_name##_service															\
: public class_name##_service_base

#define REMOTE_CLASS_BODY_IMPL(class_name)											\
{																					\
public:																				\
	class_name##_service()															\
	{}																				\
																					\
	class_name##_service(::boost::weak_ptr<Target> const& _weak_target,				\
						::remote::detail::object_id const& id)						\
	: class_name##_service_base(id)													\
	, m_weak_target(_weak_target)													\
	{}																				\
																					\
	::boost::weak_ptr<Target> weak_target()											\
	{																				\
		return m_weak_target;														\
	}																				\
																					\
private:																			\
	::boost::weak_ptr<Target> m_weak_target;										\
																					\
	::boost::shared_ptr<Target> target()											\
	{																				\
		::boost::shared_ptr<Target> target = m_weak_target.lock();					\
		if(!target)																	\
		{																			\
			throw ::remote::remote_error(::remote::remote_error::target_not_found,	\
					"target not found");											\
		}																			\
		return target;																\
	}


#define REMOTE_CLASS_BEGIN(class_name) REMOTE_CLASS_BEGIN_0(class_name)


#define REMOTE_CLASS_BEGIN_0(class_name)											\
REMOTE_CLASS_BEGIN_IMPL(class_name)													\
REMOTE_CLASS_BODY_IMPL(class_name)


#define REMOTE_CLASS_BEGIN_1(class_name, B1)										\
REMOTE_CLASS_BEGIN_IMPL(class_name)													\
REMOTE_CLASS_BODY_IMPL(class_name)


#define REMOTE_CLASS_BEGIN_2(class_name, B1, B2)									\
REMOTE_CLASS_BEGIN_IMPL(class_name)													\
REMOTE_CLASS_BODY_IMPL(class_name)


#define REMOTE_CLASS_BEGIN_3(class_name, B1, B2, B3)								\
REMOTE_CLASS_BEGIN_IMPL(class_name)													\
REMOTE_CLASS_BODY_IMPL(class_name)


#define REMOTE_CLASS_END															\
};

#define REMOTE_METHOD_MX(method_name, arity)										\
	void invoke(::remote::detail::pool_node& node, method_name##_m##arity& call)	\
	{																				\
		::remote::detail::service::invoke(node, &Target::method_name, target(), call, ::remote::detail::non_const_tag());	\
	}

#define REMOTE_METHOD_CX(method_name, arity)										\
	void invoke(::remote::detail::pool_node& node, method_name##_c##arity& call)	\
	{																				\
		::remote::detail::service::invoke(node, &Target::method_name, target(), call, ::remote::detail::const_tag());	\
	}

#define REMOTE_METHOD_M0(R, method_name) REMOTE_METHOD_MX(method_name, 0)
#define REMOTE_METHOD_M1(R, method_name, A1) REMOTE_METHOD_MX(method_name, 1)
#define REMOTE_METHOD_M2(R, method_name, A1, A2) REMOTE_METHOD_MX(method_name, 2)
#define REMOTE_METHOD_M3(R, method_name, A1, A2, A3) REMOTE_METHOD_MX(method_name, 3)
#define REMOTE_METHOD_M4(R, method_name, A1, A2, A3, A4) REMOTE_METHOD_MX(method_name, 4)
#define REMOTE_METHOD_M5(R, method_name, A1, A2, A3, A4, A5) REMOTE_METHOD_MX(method_name, 5)
#define REMOTE_METHOD_M6(R, method_name, A1, A2, A3, A4, A5, A6) REMOTE_METHOD_MX(method_name, 6)

#define REMOTE_METHOD_C0(R, method_name) REMOTE_METHOD_CX(method_name, 0)
#define REMOTE_METHOD_C1(R, method_name, A1) REMOTE_METHOD_CX(method_name, 1)
#define REMOTE_METHOD_C2(R, method_name, A1, A2) REMOTE_METHOD_CX(method_name, 2)
#define REMOTE_METHOD_C3(R, method_name, A1, A2, A3) REMOTE_METHOD_CX(method_name, 3)
#define REMOTE_METHOD_C4(R, method_name, A1, A2, A3, A4) REMOTE_METHOD_CX(method_name, 4)
#define REMOTE_METHOD_C5(R, method_name, A1, A2, A3, A4, A5) REMOTE_METHOD_CX(method_name, 5)
#define REMOTE_METHOD_C6(R, method_name, A1, A2, A3, A4, A5, A6) REMOTE_METHOD_CX(method_name, 6)

#define REMOTE_ASYNC_M0(R, method_name) REMOTE_METHOD_MX(method_name, 0)
#define REMOTE_ASYNC_M1(R, method_name, A1) REMOTE_METHOD_MX(method_name, 1)
#define REMOTE_ASYNC_M2(R, method_name, A1, A2) REMOTE_METHOD_MX(method_name, 2)
#define REMOTE_ASYNC_M3(R, method_name, A1, A2, A3) REMOTE_METHOD_MX(method_name, 3)
#define REMOTE_ASYNC_M4(R, method_name, A1, A2, A3, A4)  REMOTE_METHOD_MX(method_name, 4)
#define REMOTE_ASYNC_M5(R, method_name, A1, A2, A3, A4, A5) REMOTE_METHOD_MX(method_name, 5)
#define REMOTE_ASYNC_M6(R, method_name, A1, A2, A3, A4, A5, A6) REMOTE_METHOD_MX(method_name, 6)

#define REMOTE_ASYNC_C0(R, method_name) REMOTE_METHOD_CX(method_name, 0)
#define REMOTE_ASYNC_C1(R, method_name, A1) REMOTE_METHOD_CX(method_name, 1)
#define REMOTE_ASYNC_C2(R, method_name, A1, A2) REMOTE_METHOD_CX(method_name, 2)
#define REMOTE_ASYNC_C3(R, method_name, A1, A2, A3) REMOTE_METHOD_CX(method_name, 3)
#define REMOTE_ASYNC_C4(R, method_name, A1, A2, A3, A4) REMOTE_METHOD_CX(method_name, 4)
#define REMOTE_ASYNC_C5(R, method_name, A1, A2, A3, A4, A5) REMOTE_METHOD_CX(method_name, 5)
#define REMOTE_ASYNC_C6(R, method_name, A1, A2, A3, A4, A5, A6) REMOTE_METHOD_CX(method_name, 6)

REMOTE_CLASS

#include <remote/idl/detail/undefine.ipp>

