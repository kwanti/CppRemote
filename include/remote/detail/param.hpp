// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_PARAM_HPP__
#define __REMOTE_DETAIL_PARAM_HPP__

#include <remote/detail/stack_allocate.hpp>

#include <boost/serialization/wrapper.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>


namespace remote
{
namespace detail
{

template<typename T>
class param
: public boost::serialization::wrapper_traits<param<T> >
{
public:
	param()
	: m_ptr(0)
	{}

	param(T const& t)
	: m_ptr(0)
	{
		construct(t);
	}

	param(param const& src)
	: m_ptr(0)
	{
		BOOST_ASSERT(src.m_ptr != 0);
		construct(src.get());
	}

	param const& operator = (T const& t)
	{
		BOOST_ASSERT(m_ptr == 0);
		construct(t);
		return *this;
	}

	param const& operator = (param const& src)
	{
		BOOST_ASSERT(m_ptr == 0);
		BOOST_ASSERT(src.m_ptr != 0);

		construct(src.get());
		return *this;
	}

	~param()
	{
		if(m_ptr == 0)
			return;

		destruct();
	}

	T const& get()
	{
		return *m_ptr;
	}

	T const& get() const
	{
		return *m_ptr;
	}

public:	// for serialization access only
	typedef typename boost::remove_const<T>::type raw_type;

	raw_type* allocate()
	{
		BOOST_ASSERT(m_ptr == 0);
		BOOST_ASSERT(m_ptr != m_storage.address());

		m_ptr = m_storage.address();
		return m_ptr;
	}

	raw_type* m_ptr;

private:
	stack_allocate<raw_type> m_storage;

private:

	void construct(T const& t)
	{
		BOOST_ASSERT(m_ptr == 0);
		BOOST_ASSERT(m_ptr != m_storage.address());

		m_ptr = m_storage.address();
		::new(m_ptr) raw_type(t);
	}

	void destruct()
	{
		BOOST_ASSERT(m_ptr != 0);
		BOOST_ASSERT(m_ptr == m_storage.address());

		m_ptr->~T();
		m_ptr = 0;
	}
};

template<typename T>
class param<T&>
: public boost::serialization::wrapper_traits<param<T&> >
{
public:
	param()
	: m_ptr(0)
	{}

	param(T& t)
	: m_ptr(&t)
	{}

	param(param const& src)
	: m_ptr(src.m_ptr)
	{}

	param const& operator = (T& t)
	{
		m_ptr = &t;
		return *this;
	}

	param const& operator = (param const& src)
	{
		m_ptr = src.m_ptr;
		return *this;
	}

	~param()
	{}

	T& get()
	{
		return *m_ptr;
	}

	T& get() const
	{
		return *m_ptr;
	}

public:	// for serialization access only
	T* m_ptr;
};

template<typename T>
class param<T*>
: public boost::serialization::wrapper_traits<param<T*> >
{
public:
	param()
	: m_ptr(0)
	{}

	param(T* ptr)
	: m_ptr(ptr)
	{}

	param(param const& src)
	: m_ptr(src.m_ptr)
	{}

	param const& operator = (T* ptr)
	{
		m_ptr = ptr;
		return *this;
	}

	param const& operator = (param const& src)
	{
		m_ptr = src.m_ptr;
		return *this;
	}

	~param()
	{}

	T* get()
	{
		return m_ptr;
	}

	T* get() const
	{
		return m_ptr;
	}

public:	// for serialization access only
	T* m_ptr;
};

template<typename T>
class param<boost::shared_ptr<T> >
: public boost::serialization::wrapper_traits<param<boost::shared_ptr<T> > >
{
public:
	param()
	{}

	param(boost::shared_ptr<T> const& ptr)
	: m_ptr(ptr)
	{}

	param(param const& src)
	: m_ptr(src.m_ptr)
	{}

	param const& operator = (boost::shared_ptr<T> const& ptr)
	{
		m_ptr = ptr;
		return *this;
	}

	param const& operator = (param const& src)
	{
		m_ptr = src.m_ptr;
		return *this;
	}

	~param()
	{}

	boost::shared_ptr<T> const& get()
	{
		return m_ptr;
	}

	boost::shared_ptr<T> const& get() const
	{
		return m_ptr;
	}

public: // for serialization access only
	boost::shared_ptr<T> m_ptr;
};

template<typename T>
class param<boost::weak_ptr<T> >
: public boost::serialization::wrapper_traits<param<boost::weak_ptr<T> > >
{
public:
	param()
	{}

	param(boost::weak_ptr<T> const& ptr)
	: m_ptr(ptr)
	{}

	param(param const& src)
	: m_ptr(src.m_ptr)
	{}

	param const& operator = (boost::weak_ptr<T> const& ptr)
	{
		m_ptr = ptr;
		return *this;
	}

	param const& operator = (param const& src)
	{
		m_ptr = src.m_ptr;
		return *this;
	}

	~param()
	{}

	boost::weak_ptr<T> const& get()
	{
		return m_ptr;
	}

	boost::weak_ptr<T> const& get() const
	{
		return m_ptr;
	}

public: // for serialization access only
	boost::weak_ptr<T> m_ptr;
};

template<>
class param<void>
{
public:
	void get() const
	{}
};

}
}

#endif
