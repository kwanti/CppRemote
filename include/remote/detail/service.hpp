// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_SERVICE_HPP__
#define __REMOTE_DETAIL_SERVICE_HPP__

#include <remote/detail/fwd.hpp>
#include <remote/detail/call.hpp>
#include <remote/detail/marshal.hpp>

#include <boost/noncopyable.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>


namespace remote
{
namespace detail
{

class service
: public boost::noncopyable
, public boost::enable_shared_from_this<service>
{
public:
	service()
	{}

	service(object_id const& id)
	: m_id(id)
	{}

	virtual ~service()
	{}

	object_id id() const { return m_id; }

protected:
	object_id m_id;

protected:
	typedef boost::mpl::true_ async_tag;
	typedef boost::mpl::false_ sync_tag;

	template<typename R, typename F>
	void poll_result(param<R>& r, F f)
	{
		r = f.get();
	}

	template<typename F>
	void poll_result(param<void>&, F f)
	{
		f.get();
	}

#define REMOTE_CC
#define REMOTE_CC_TAG non_const_tag
#include <remote/detail/service_invoke.ipp>
#undef REMOTE_CC
#undef REMOTE_CC_TAG

#define REMOTE_CC const
#define REMOTE_CC_TAG const_tag
#include <remote/detail/service_invoke.ipp>
#undef REMOTE_CC
#undef REMOTE_CC_TAG

};

}
}

#endif
