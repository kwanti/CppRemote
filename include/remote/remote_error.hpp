// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_REMOTE_ERROR_HPP__
#define __REMOTE_REMOTE_ERROR_HPP__

#include <remote/exception.hpp>

#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
#include <stdexcept>


namespace remote
{

class remote_error
: public exception_base<remote_error>
, public std::runtime_error
{
public:
	enum error_code
	{
		unknown_error,
		timeout,
		no_result,

		invalid_endpoint,
		operation_aborted,
		operation_pending,
		already_stopped,

		remote_param_error,
		invalid_return_type,
		invalid_service_type,
		service_not_found,
		target_not_found
	};

public:
	remote_error(error_code ec, std::string const& msg = "")
	: std::runtime_error(msg.c_str())
	, m_code(ec)
	{
		BOOST_ASSERT(this == &this_ref());
	}

	error_code code() const { return m_code; }

	virtual char const* what() const BOOST_NOEXCEPT_OR_NOTHROW
	{
		return std::runtime_error::what();
	}


private:
	error_code m_code;

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive&, unsigned int const)
	{
		boost::serialization::base_object<remote::exception>(*this);
	}
};

inline exception_ptr make_remote_error(remote_error::error_code ec, std::string const& msg)
{
	return boost::make_shared<remote_error>(ec, msg);
}

}

namespace boost
{
namespace serialization
{

template<typename Archive>
void save_construct_data(Archive& ar, remote::remote_error const* e, unsigned int const)
{
	std::string msg(e->what());
	remote::remote_error::error_code code(e->code());

	ar << make_nvp("msg", msg);
	ar << make_nvp("code", code);
}

template<typename Archive>
void load_construct_data(Archive& ar, remote::remote_error* e, unsigned int const)
{
	std::string msg;
	remote::remote_error::error_code code;

	ar >> make_nvp("msg", msg);
	ar >> make_nvp("code", code);

	::new(e)remote::remote_error(code, msg);
}

}
}

BOOST_CLASS_TRACKING(remote::remote_error, boost::serialization::track_never)
BOOST_CLASS_EXPORT_KEY(remote::remote_error)

#endif
