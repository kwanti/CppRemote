// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#include <boost/config.hpp>

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4512)
#endif

#include <remote/io_runner.hpp>
#include <boost/utility/in_place_factory.hpp>


namespace remote
{

io_runner::io_runner(std::size_t concurrency)
: m_io_service(adjust_concurrency(concurrency))
, m_work(boost::in_place(boost::ref(m_io_service)))
{
	std::size_t c = adjust_concurrency(concurrency);

	BOOST_ASSERT(c > 0);
	m_threads.create_thread(boost::bind(&io_runner::run_idle, this));

	for(std::size_t i = 1; i < c; ++i)
	{
		m_threads.create_thread(boost::bind(&io_runner::run, this));
	}
}

io_runner::~io_runner()
{
	m_work = boost::none;
//	m_io_service.stop();
	m_threads.join_all();
}

boost::asio::io_service& io_runner::get_io_service()
{
	return m_io_service;
}

boost::asio::io_service& io_runner::get_idle_io_service()
{
	return m_idle_io_service;
}

std::size_t io_runner::adjust_concurrency(std::size_t concurrency)
{
	if(concurrency == max)
		concurrency = boost::thread::hardware_concurrency();

	return (concurrency == 0) ? 1 : concurrency;
}

void io_runner::run()
{
	try
	{
		m_io_service.run();
		return;
	}
	catch(std::exception& e)
	{
		// log error here
		std::cerr << "io service throw: " << e.what() << std::endl;
	}
	catch(...)
	{
		// not to continue on unknown exception
		std::cerr << "io service throw !!!" << std::endl;
	}
	std::terminate();
}

void io_runner::run_idle()
{
	try
	{
		while (!!m_work)
		{
			m_io_service.poll();
			m_idle_io_service.poll_one();
		}
		return;
	}
	catch (std::exception& e)
	{
		std::cerr << "io service throw: " << e.what() << std::endl;
	}
	catch (...)
	{
		// not to continue on unknown exception
		std::cerr << "io service throw !!!" << std::endl;
	}
	std::terminate();
}

std::size_t io_runner::poll_one(boost::asio::io_service& ios)
{
	try
	{
		return ios.poll_one();
	}
	catch(std::exception& e)
	{
		// log error here
		std::cerr << "io service throw: " << e.what() << std::endl;
	}
	catch(...)
	{
		// not to continue on unknown exception
		std::cerr << "io service throw !!!" << std::endl;
	}
	std::terminate();
}


}


#ifdef BOOST_MSVC
#pragma warning(pop)
#endif
