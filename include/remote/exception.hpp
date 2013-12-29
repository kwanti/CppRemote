// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_EXCEPTION_H__
#define __REMOTE_EXCEPTION_H__

#include <remote/detail/config.hpp>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <exception>


namespace remote
{

class exception;
typedef boost::shared_ptr<exception> exception_ptr;

class exception
{
public:
	virtual ~exception() throw() {}
	virtual char const* what() const BOOST_NOEXCEPT_OR_NOTHROW = 0;

protected:
	virtual void throw_this() const = 0;
	virtual exception_ptr clone() const = 0;

	friend exception_ptr make_exception_ptr(exception const&);
	friend void rethrow_exception(exception_ptr const&);
	friend std::ostream& operator << (std::ostream& os, exception const& e);
};

template<typename T>
class exception_base: public exception
{
protected:
	virtual void throw_this() const
	{
		throw this_ref();
	}

	virtual exception_ptr clone() const
	{
		return boost::make_shared<T>(this_ref());
	}

	T const& this_ref() const
	{
		// should we use dynamic_cast here?
		return static_cast<T const&>(*this);
	}
};


exception_ptr current_exception();

inline exception_ptr make_exception_ptr(exception const& e)
{
	return e.clone();
}

inline void rethrow_exception(exception_ptr const& e)
{
	BOOST_ASSERT(e);
	e->throw_this();
}

std::ostream& operator << (std::ostream& os, exception const& e);

}

#endif
