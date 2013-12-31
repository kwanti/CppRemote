// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef REMOTE_DETAIL_MARSHAL_HPP__
#define REMOTE_DETAIL_MARSHAL_HPP__

#include <remote/detail/fwd.hpp>
#include <remote/detail/type_traits.hpp>
#include <remote/detail/pool_node.hpp>
#include <remote/detail/proxy_pool.hpp>
#include <remote/remote_error.hpp>

#include <boost/static_assert.hpp>


namespace remote
{
namespace detail
{

class forward_marshaller
{
public:
	template<typename T>
	static T& apply(pool_node&, T& t)
	{
		return t;
	}

	template<typename T>
	static T const& apply(pool_node&, T const& t)
	{
		return t;
	}
};

class remote_ptr_marshaller
{
public:
	template<typename T>
	static T const& apply(pool_node& node, T const& t)
	{
		return apply(node, const_cast<T&>(t));
	}

	template<typename T>
	static T*& apply(pool_node& node, T*& t)
	{
		if(!t)
			return t;

		proxy_pool_ptr pool = node.find_pool(t->pool_id());
		if(!pool)
			throw remote_error(remote_error::remote_param_error, "pool not found");

		t = pool->get<T>(raw_tag(), t->id());
		if(!t)
			throw remote_error(remote_error::remote_param_error, "proxy not found");

		return t;
	}

	template<typename T>
	static boost::shared_ptr<T>& apply(pool_node& node, boost::shared_ptr<T>& t)
	{
		if(!t)
			return t;

		proxy_pool_ptr pool = node.find_pool(t->pool_id());
		if(!pool)
			throw remote_error(remote_error::remote_param_error, "pool not found");

		t = pool->get<T>(shared_tag(), t->id());
		if(!t)
			throw remote_error(remote_error::remote_param_error, "proxy not found");

		return t;
	}

	template<typename T>
	static boost::weak_ptr<T>& apply(pool_node& node, boost::weak_ptr<T>& t)
	{
		boost::shared_ptr<T> sp = t.lock();
		if(!sp)
			return t;

		proxy_pool_ptr pool = node.find_pool(sp->pool_id());
		if(!pool)
			throw remote_error(remote_error::remote_param_error, "pool not found");

		t = pool->get<T>(shared_tag(), sp->id());
		if(t.expired())
			throw remote_error(remote_error::remote_param_error, "proxy not found");

		return t;
	}
};

class remote_ref_marshaller
{
	template<typename T>
	static T const& apply(pool_node& node, T const& t)
	{
		return apply(node, const_cast<T&>(t));
	}

	template<typename T>
	static T& apply(pool_node& node, T& t)
	{
		return *remote_ptr_marshaller::apply(node, &t);
	}
};

template<typename T>
T const& marshal_container(pool_node& node, T const& t)
{
	return marshal_container(node, const_cast<T&>(t));
}

template<typename T>
T& marshal_container(pool_node&, T& t)
{
	// no marshal function is implemented for container T
	// check if the marshaller file for T is included
	BOOST_STATIC_ASSERT(sizeof(t) == 0);
}

class container_ptr_marshaller
{
public:
	template<typename T>
	static T const& apply(pool_node& node, T const& t)
	{
		return apply(node, const_cast<T&>(t));
	}

	template<typename T>
	static T*& apply(pool_node& node, T*& t)
	{
		if(!t)
			return t;

		marshal_container(node, *t);
		return t;
	}

	template<typename T>
	static boost::shared_ptr<T>& apply(pool_node& node, boost::shared_ptr<T>& t)
	{
		if(!t)
			return t;

		marshal_container(node, *t);
		return t;
	}

	template<typename T>
	static boost::weak_ptr<T>& apply(pool_node& node, boost::weak_ptr<T>& t)
	{
		boost::shared_ptr<T> sp = t.lock();
		if(!sp)
			return t;

		marshal_container(node, *sp);
		return t;
	}
};

class container_ref_marshaller
{
public:
	template<typename T>
	static T const& apply(pool_node& node, T const& t)
	{
		return apply(node, const_cast<T&>(t));
	}

	template<typename T>
	static T& apply(pool_node& node, T& t)
	{
		return marshal_container(node, t);
	}
};

template<typename T>
T& marshal(pool_node& node, T& t)
{
	using namespace boost::mpl;
	typedef typename remove_any_pointer<T>::type RawT;

	typedef typename
	eval_if<is_remote<RawT>,
		eval_if<is_any_pointer<T>,
			identity<remote_ptr_marshaller>,
			identity<remote_ref_marshaller> >,
		eval_if<is_remote_container<RawT>,
			eval_if<is_any_pointer<T>,
				identity<container_ptr_marshaller>,
				identity<container_ref_marshaller> >,
			identity<forward_marshaller> > >::type marshaller;

	return marshaller::template apply(node, t);
}

template<typename T>
T const& marshal(pool_node& node, T const& t)
{
	using namespace boost::mpl;
	typedef typename remove_any_pointer<T>::type RawT;

	typedef typename
	eval_if<is_remote<RawT>,
		eval_if<is_any_pointer<T>,
			identity<remote_ptr_marshaller>,
			identity<remote_ref_marshaller> >,
		eval_if<is_remote_container<RawT>,
			eval_if<is_any_pointer<T>,
				identity<container_ptr_marshaller>,
				identity<container_ref_marshaller> >,
			identity<forward_marshaller> > >::type marshaller;

	return marshaller::template apply(node, t);
}

struct marshal_op
{
	pool_node& m_node;

	marshal_op(pool_node& node): m_node(node)
	{}

	marshal_op(marshal_op const& src): m_node(src.m_node)
	{}

	template<typename T>
	void operator () (T& t)
	{
		marshal(m_node, t);
	}

private:
	marshal_op const& operator = (marshal_op const& src);
};

inline marshal_op marshal_(pool_node& node)
{
	return marshal_op(node);
}

template<typename C>
struct marshal_into_op
{
	pool_node& m_node;
	C& m_c;

	marshal_into_op(pool_node& node, C& c): m_node(node), m_c(c)
	{}

	marshal_into_op(marshal_into_op const& src): m_node(src.m_node), m_c(src.m_c)
	{}

	template<typename T>
	void operator () (T& t)
	{
		m_c.insert(marshal(m_node, t));
	}

private:
	marshal_into_op const& operator = (marshal_into_op const& src);
};

template<typename C>
marshal_into_op<C> marshal_into_(pool_node& node, C& c)
{
	return marshal_into_op<C>(node, c);
}

}
}

#endif
