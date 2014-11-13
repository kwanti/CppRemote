// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_IO_RUNNER_HPP__
#define __REMOTE_IO_RUNNER_HPP__

#include <boost/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/optional/optional.hpp>


namespace remote
{

class io_runner : public boost::noncopyable
{
public:
	static const std::size_t max = static_cast<std::size_t>(-1);

public:
	explicit io_runner(std::size_t concurrency);
	~io_runner();
	boost::asio::io_service& get_io_service();
	boost::asio::io_service& get_idle_io_service();

	static std::size_t poll_one(boost::asio::io_service& ios);

private:
	boost::thread_group m_threads;
	boost::asio::io_service m_io_service;
	boost::asio::io_service m_idle_io_service;
	boost::optional<boost::asio::io_service::work> m_work;

private:
	void run();
	void run_idle();
	static std::size_t adjust_concurrency(std::size_t concurrency);
};

}

#endif
