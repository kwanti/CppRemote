// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_TARGET_ERROR_HPP__
#define __REMOTE_TARGET_ERROR_HPP__

#include <remote/exception.hpp>

#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
#include <stdexcept>

namespace remote
{

class target_error: public exception
{};

template<typename T>
class target_error_base: public target_error
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

class target_std_error
: public target_error_base<target_std_error>
, public std::runtime_error
{
public:
	explicit target_std_error(std::string const& msg)
	: std::runtime_error(msg.c_str())
	{
		BOOST_ASSERT(this == &this_ref());
	}

	explicit target_std_error(std::exception const& e)
	: std::runtime_error(e.what())
	, m_name(typeid(e).name())
	{
		BOOST_ASSERT(this == &this_ref());
	}

	~target_std_error() throw() {}

	std::string const& name() const { return m_name; }

	virtual char const* what() const BOOST_NOEXCEPT_OR_NOTHROW
	{
		return std::runtime_error::what();
	}

private:
	std::string m_name;

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, unsigned int const)
	{
		using namespace boost::serialization;
		base_object<remote::exception>(*this);
		ar & make_nvp("name", m_name);
	}
};

class target_unknown_error
: public target_error_base<target_unknown_error>
, public std::runtime_error
{
public:
	target_unknown_error()
	: std::runtime_error("unknown target error")
	{
		BOOST_ASSERT(this == &this_ref());
	}

	virtual char const* what() const BOOST_NOEXCEPT_OR_NOTHROW
	{
		return std::runtime_error::what();
	}

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive&, unsigned int const)
	{
		boost::serialization::base_object<remote::exception>(*this);
	}
};

}

namespace boost
{
namespace serialization
{

template<typename Archive>
void save_construct_data(Archive& ar, remote::target_std_error const* e, unsigned int const)
{
	std::string msg(e->what());
	ar << make_nvp("msg", msg);
}

template<typename Archive>
void load_construct_data(Archive& ar, remote::target_std_error* e, unsigned int const)
{
	std::string msg;
	ar >> make_nvp("msg", msg);
	::new(e)remote::target_std_error(msg);
}

}
}


BOOST_CLASS_TRACKING(remote::target_std_error, boost::serialization::track_never)
BOOST_CLASS_EXPORT_KEY(remote::target_std_error)

BOOST_CLASS_TRACKING(remote::target_unknown_error, boost::serialization::track_never)
BOOST_CLASS_EXPORT_KEY(remote::target_unknown_error)


#endif
