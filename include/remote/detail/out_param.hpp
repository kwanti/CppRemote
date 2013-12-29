// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_OUT_PARAM_HPP__
#define __REMOTE_DETAIL_OUT_PARAM_HPP__

#include <remote/detail/type_traits.hpp>
#include <remote/detail/marshal.hpp>
#include <remote/detail/param.hpp>

#include <boost/type_traits/is_pod.hpp>
#include <boost/utility/swap.hpp>
#include <boost/noncopyable.hpp>


namespace remote
{
namespace detail
{

template<typename T>
void assign_or_swap(T& dst, T& src, boost::mpl::true_)
{
	dst = src;
}

template<typename T>
void assign_or_swap(T& dst, T& src, boost::mpl::false_)
{
	boost::swap(dst, src);
}

template<typename T>
void assign_or_swap(T& dst, T& src)
{
	assign_or_swap(dst, src, boost::is_pod<T>());
}


template<typename T>
class out_param : public boost::noncopyable
{
public:
	out_param(param<T> const&)
	{}

	void update(pool_node&, param<T> const&)
	{}
};

template<typename T>
class out_param<T&> : public boost::noncopyable
{
public:
	out_param(param<T&> const& p)
	: m_t(p.get())
	{}

	void update(pool_node& node, param<T&> const& p)
	{
		typedef typename boost::mpl::or_<is_remote<T>,
			boost::is_abstract<T> >::type is_abstract;

		update(node, p, is_abstract());
	}

private:
	T& m_t;

	void update(pool_node&, param<T&> const&, boost::mpl::true_)
	{}

	void update(pool_node& node, param<T&> const& p, boost::mpl::false_)
	{
		T& src = marshal(node, p.get());
		assign_or_swap(m_t, src);
	}
};

template<typename T>
class out_param<T const&> : public boost::noncopyable
{
public:
	out_param(param<T const&> const&)
	{}

	void update(pool_node&, param<T const&> const&)
	{}
};

template<typename T>
class out_param<T*> : public boost::noncopyable
{
public:
	out_param(param<T*> const& p)
	: m_ptr(p.get())
	{}

	void update(pool_node& node, param<T*> const& p)
	{
		typedef typename boost::mpl::or_<is_remote<T>,
			boost::is_abstract<T> >::type is_abstract;

		update(node, p, is_abstract());
	}

private:
	T* m_ptr;

	void update(pool_node&, param<T*> const&, boost::mpl::true_)
	{}

	void update(pool_node& node, param<T*> const& p, boost::mpl::false_)
	{
		if(!m_ptr)
			return;

		T* src_ptr = marshal(node, p.get());
		assign_or_swap(*m_ptr, *src_ptr);
	}
};

template<typename T>
class out_param<T const*> : public boost::noncopyable
{
public:
	out_param(param<T const*> const&)
	{}

	void update(pool_node&, param<T const*> const&)
	{}
};

template<typename T>
class out_param<boost::shared_ptr<T> > : public boost::noncopyable
{
public:
	out_param(param<boost::shared_ptr<T> > const& p)
	: m_ptr(p.get())
	{}

	void update(pool_node& node, param<boost::shared_ptr<T> > const& p)
	{
		typedef typename boost::mpl::or_<is_remote<T>,
			boost::is_abstract<T> >::type is_abstract;

		update(node, p, is_abstract());
	}

private:
	boost::shared_ptr<T> m_ptr;

	void update(pool_node&, param<boost::shared_ptr<T> > const&, boost::mpl::true_)
	{}

	void update(pool_node& node, param<boost::shared_ptr<T> > const& p, boost::mpl::false_)
	{
		if(!m_ptr)
			return;

		boost::shared_ptr<T> src_ptr = marshal(node, p.get());
		assign_or_swap(*m_ptr, *src_ptr);
	}
};

template<typename T>
class out_param<boost::shared_ptr<T const> > : public boost::noncopyable
{
public:
	out_param(param<boost::shared_ptr<T const> > const&)
	{}

	void update(pool_node&, param<boost::shared_ptr<T const> > const&)
	{}
};

template<typename T>
class out_param<boost::weak_ptr<T> > : public boost::noncopyable
{
public:
	out_param(param<boost::weak_ptr<T> > const& p)
	: m_ptr(p.get())
	{}

	void update(pool_node& node, param<boost::weak_ptr<T> > const& p)
	{
		typedef typename boost::mpl::or_<is_remote<T>,
			boost::is_abstract<T> >::type is_abstract;

		update(node, p, is_abstract());
	}

private:
	boost::weak_ptr<T> m_ptr;

	void update(pool_node&, param<boost::weak_ptr<T> > const&, boost::mpl::true_)
	{}

	void update(pool_node& node, param<boost::weak_ptr<T> > const& p, boost::mpl::false_)
	{
		boost::shared_ptr<T> sp = m_ptr.lock();
		if(!sp)
			return;

		boost::shared_ptr<T> src_ptr = marshal(node, p.get()).lock();
		if(!src_ptr)
			return;

		assign_or_swap(*sp, *src_ptr);
	}
};

template<typename T>
class out_param<boost::weak_ptr<T const> > : public boost::noncopyable
{
public:
	out_param(param<boost::weak_ptr<T const> > const&)
	{}

	void update(pool_node&, param<boost::weak_ptr<T const> > const&)
	{}
};

}
}

#endif
