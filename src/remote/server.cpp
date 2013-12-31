// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#include <remote/detail/server.hpp>
#include <remote/server.hpp>
#include <remote/binding.hpp>
#include <remote/io_runner.hpp>
#include <remote/global.hpp>

#include <boost/make_shared.hpp>


namespace remote
{

server::server()
: pool_node(global::io_runner())
{
	initialize(global::io_runner());
}

server::server(io_runner& runner)
: pool_node(runner)
{
	initialize(runner);
}

server::~server()
{
	if(m_impl)
		m_impl->shutdown();
}

void server::initialize(io_runner& runner)
{
	m_impl = boost::make_shared<detail::server>(m_node, boost::ref(runner.get_io_service()));
}

void server::start(binding const& _binding, handler const& _handler)
{
	BOOST_ASSERT(_handler);
	m_impl->start(_binding.m_impl, _handler);
}

void server::stop(handler const& _handler)
{
	BOOST_ASSERT(_handler);
	m_impl->stop(_handler);
}

server::state_t server::state() const
{
	return static_cast<state_t>(m_impl->state());
}

server::state_t server::wait_for_ready() const
{
	return static_cast<state_t>(m_impl->wait_for_ready());
}

void server::set_accept_handler(accept_handler const& _handler)
{
	BOOST_ASSERT(_handler);
	m_impl->set_accept_handler(_handler);
}

//server::sessions server::get_sessions() const
//{
//	return m_impl->get_sessions();
//}

bool operator < (server const& lhs, server const& rhs)
{
	return (lhs.m_impl < rhs.m_impl);
}

}
