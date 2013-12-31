// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#include <remote/pool_node.hpp>
#include <remote/io_runner.hpp>

#include <boost/uuid/random_generator.hpp>
#include <boost/make_shared.hpp>


namespace remote
{

pool_node::pool_node()
{}

pool_node::pool_node(io_runner& runner)
: m_node(boost::make_shared<detail::pool_node>(boost::uuids::random_generator()()))
{
	m_node->initialize(runner.get_io_service());
}

pool_node::pool_node(detail::pool_node_ptr const& node)
: m_node(node)
{}

void pool_node::unbind_name(std::string const& name)
{
	m_node->get_service_pool().unbind_name(name);
}

void pool_node::unbind(std::string const& name)
{
	m_node->get_service_pool().unbind(name);
}

void pool_node::unbind_all()
{
	m_node->get_service_pool().unbind_all();
}

}
