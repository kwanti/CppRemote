// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef REMOTE_SESSION_HPP__
#define REMOTE_SESSION_HPP__

#include <remote/fwd.hpp>
#include <remote/pool_node.hpp>

#include <boost/chrono/duration.hpp>
#include <boost/move/move.hpp>
#include <boost/function.hpp>


namespace remote
{

class session : public pool_node
{
	BOOST_MOVABLE_BUT_NOT_COPYABLE(session)

public:
	enum state_t {stopped, starting, started, stopping};

	typedef boost::function<void(exception_ptr)> handler;
	static void default_handler(exception_ptr) {}

public:
	session();
	explicit session(io_runner& runner);
	explicit session(detail::comm_link_ptr const& link);

	session(BOOST_RV_REF(session) src)
	: pool_node(boost::move(static_cast<pool_node&>(src)))
	, m_link(boost::move(src.m_link))
	{
		src.m_link.reset();
	}

	session& operator = (BOOST_RV_REF(session) src)
	{
		if(this != &src)
		{
			pool_node::operator = (boost::move(static_cast<pool_node&>(src)));
			m_link = boost::move(src.m_link);
			src.m_link.reset();
		}
		return *this;
	}

	~session();

	void start(binding const& _binding, handler const& _handler = default_handler);
	void stop(handler const& _handler = default_handler);
	state_t state() const;
	state_t wait_for_ready() const;


	boost::chrono::milliseconds const& call_timeout() const;
	void set_call_timeout(boost::chrono::milliseconds const& timeout);

	void set_error_handler(handler const& _handler);


	template<typename Proxy>
	boost::shared_ptr<Proxy> get(std::string const& name, bool cache = false);

	template<typename Proxy>
	Proxy* get_raw(std::string const& name, bool cache = false);

	template<typename Proxy>
	void release(Proxy* _proxy);

	template<typename Proxy>
	void release(boost::shared_ptr<Proxy> const& _proxy);

#define REMOTE_CC BOOST_PP_EMPTY()
#include <remote/session_invoke_declare.ipp>
#undef REMOTE_CC

#define REMOTE_CC const
#include <remote/session_invoke_declare.ipp>
#undef REMOTE_CC

private:
	struct no_init {};
	friend class remote::detail::server;
	friend bool operator < (session const&, session const&);

	detail::comm_link_ptr m_link;

private:
	explicit session(no_init);
	void initialize(io_runner& runner);
};

bool operator < (session const& lhs, session const& rhs);

}

#include <remote/session.ipp>
#endif
