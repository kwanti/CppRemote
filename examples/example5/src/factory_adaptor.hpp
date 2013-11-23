#ifndef EXAMPLE_FACTORY_ADAPTOR_HPP
#define EXAMPLE_FACTORY_ADAPTOR_HPP

#include "model.hpp"
#include "remote_player.hpp"
#include <remote/pool_node.hpp>
#include <algorithm>


class factory_adaptor
: public boost::noncopyable
{
	remote::pool_node& m_node;
	player_factory& m_factory;
public:
	factory_adaptor(remote::pool_node& node, player_factory& factory)
	: m_node(node), m_factory(factory)
	{}

	remote_player* create(std::string type)
	{
		return m_node.remote_cast<remote_player>(m_factory.create(type));
	}

	void destroy(remote_player* p)
	{
		m_factory.destroy(m_node.target_cast<player>(p));
		m_node.unbind(p);
	}

	std::set<remote_player*> all_player() const
	{
		std::set<remote_player*> rp;
		std::set<player*> p = m_factory.all_player();

		std::transform(p.begin(), p.end(), std::inserter(rp, rp.begin()),
				boost::bind(&remote_cast, boost::ref(m_node), _1));
		return rp;
	}

	static remote_player* remote_cast(remote::pool_node& node, player* p)
	{
		return node.remote_cast<remote_player>(p);
	}
};

#endif
