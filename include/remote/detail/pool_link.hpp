// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_POOL_LINK_HPP__
#define __REMOTE_DETAIL_POOL_LINK_HPP__

#include <remote/detail/fwd.hpp>
#include <remote/detail/counter.hpp>

#include <boost/chrono/duration.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/noncopyable.hpp>

#include <string>
#include <map>


namespace remote
{
namespace detail
{

class pool_link : public boost::noncopyable
{
public:
	BOOST_STATIC_ASSERT(sizeof(link_id_t) == sizeof(pool_link*));
	typedef boost::asio::io_service io_service;

public:
	pool_link(pool_node_ptr const& pool, io_service& ios);
	virtual ~pool_link();

	proxy_pool& get_proxy_pool();
	pool_node_ptr get_pool_node();

	link_id_t id() const { return reinterpret_cast<link_id_t>(this); }
	boost::uuids::uuid const& pool_id() const { return m_pool_id; }

	boost::chrono::milliseconds const& call_timeout() const { return m_call_timeout; }
	void set_call_timeout(boost::chrono::milliseconds const& timeout) { m_call_timeout = timeout; }

	void invoke(call_ptr const& _call, result_ptr const& _result);
	void invoke(call_ptr const& _call);

	virtual void send(call_ptr const& _call) = 0;
	virtual object_id resolve(std::string const& name, bool& cache) = 0;

protected:
	void initialize(pool_link_ptr const& self);
	void shutdown();

	bool is_attached() const;
	void attach(boost::uuids::uuid const& pid);
	void detach();
	void dispatch(call_ptr _call);

protected:
	proxy_pool_ptr m_proxy_pool;
	pool_node_weak_ptr m_weak_node;
	io_service& m_io_service;

private:
	static const boost::chrono::milliseconds::rep default_timeout = 3000;
	typedef std::map<call_id, result_weak_ptr> weak_results;

private:
	boost::uuids::uuid m_pool_id;
	boost::chrono::milliseconds m_call_timeout;

	mutable boost::mutex m_results_mutex;
	weak_results m_results;

	counter<call_id> m_call_id;

private:
	void cancel_results(exception_ptr e);
};

}
}

#endif
