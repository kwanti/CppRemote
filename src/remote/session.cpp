// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under GPL v3 license. You can redistribute it and/or
// modify it under the terms of the GNU General Public License version 3 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#include <remote/session.hpp>
#include <remote/global.hpp>
#include <remote/binding.hpp>

#include <boost/thread/thread.hpp>
#include <boost/smart_ptr/make_shared.hpp>


namespace remote
{

session::session()
: pool_node(global::io_runner())
{
	initialize(global::io_runner());
}

session::session(io_runner& runner)
: pool_node(runner)
{
	initialize(runner);
}

session::session(no_init)
{}

session::session(detail::comm_link_ptr const& link)
: pool_node(link->get_pool_node())
, m_link(link)
{}

session::session(BOOST_RV_REF(session) src)
: pool_node(boost::move(static_cast<pool_node&>(src)))
, m_link(boost::move(src.m_link))
{
	src.m_link.reset();
}

session& session::operator = (BOOST_RV_REF(session) src)
{
	if(this != &src)
	{
		pool_node::operator = (boost::move(static_cast<pool_node&>(src)));
		m_link = boost::move(src.m_link);
		src.m_link.reset();
	}
	return *this;
}

session::~session()
{
	if(m_link)
		m_link->shutdown();
}

void session::initialize(io_runner& runner)
{
	m_link = boost::make_shared<detail::comm_link>(m_node, boost::ref(runner.get_io_service()));
	m_link->initialize();
}

void session::start(binding const& _binding, handler const& _handler)
{
	BOOST_ASSERT(_handler);
	m_link->connect(_binding.m_impl, _handler);
}

void session::stop(handler const& _handler)
{
	BOOST_ASSERT(_handler);
	m_link->disconnect(_handler);
}

session::state_t session::state() const
{
	return static_cast<state_t>(m_link->state());
}

session::state_t session::wait_for_ready() const
{
	for(int i=0; i<20; i++)
	{
		if(!m_link->ready())
			break;

		boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
	}

	while(!m_link->ready())
	{
		boost::this_thread::sleep_for(boost::chrono::milliseconds(50));
	}

	return state();
}

boost::chrono::milliseconds const& session::call_timeout() const
{
	return m_link->call_timeout();
}

void session::set_call_timeout(boost::chrono::milliseconds const& timeout)
{
	m_link->set_call_timeout(timeout);
}

void session::set_error_handler(handler const& _handler)
{
	BOOST_ASSERT(_handler);
	m_link->set_error_handler(_handler);
}

bool operator < (session const& lhs, session const& rhs)
{
	return (lhs.m_link < rhs.m_link);
}

}
