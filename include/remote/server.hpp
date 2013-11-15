// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under GPL v3 license. You can redistribute it and/or
// modify it under the terms of the GNU General Public License version 3 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef REMOTE_SERVER_HPP__
#define REMOTE_SERVER_HPP__

#include <remote/fwd.hpp>
#include <remote/pool_node.hpp>

#include <boost/move/move.hpp>
#include <boost/function.hpp>


namespace remote
{

class server : public pool_node
{
	BOOST_MOVABLE_BUT_NOT_COPYABLE(server)

public:
	enum state_t {stopped, starting, started, stopping};

	typedef boost::function<void(exception_ptr)> handler;
	typedef boost::function<void(exception_ptr, session&)> accept_handler;

	static void default_handler(exception_ptr) {}
	static void default_accept_handler(exception_ptr, session&) {}

public:
	server();
	explicit server(io_runner& runner);
	server(BOOST_RV_REF(server) src);
	server& operator = (BOOST_RV_REF(server) src);
	~server();

	void start(binding const& _binding, handler const& _handler = default_handler);
	void stop(handler const& _handler = default_handler);
	state_t state() const;
	state_t wait_for_ready() const;

	void set_accept_handler(accept_handler const& _handler = default_accept_handler);

private:
	friend bool operator < (server const&, server const&);
	detail::server_ptr m_impl;

	void initialize(io_runner& runner);
};

bool operator < (server const& lhs, server const& rhs);
}

#endif
