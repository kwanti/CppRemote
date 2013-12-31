// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_PROXY_HPP__
#define __REMOTE_DETAIL_PROXY_HPP__

#include <remote/detail/fwd.hpp>
#include <remote/detail/param.hpp>
#include <remote/detail/pool_link.hpp>

#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/access.hpp>
#include <boost/variant/variant.hpp>


namespace remote
{

class attribute;

namespace detail
{

class proxy : public boost::noncopyable
{
public:
	proxy();
	proxy(pool_link_weak_ptr link, object_vid const& id);
	virtual ~proxy();

	boost::uuids::uuid pool_id() const;
	object_vid const& id() const { return m_id; }

	void update();

protected:
	friend class boost::serialization::access;
	friend class remote::attribute;

private:
	pool_link_weak_ptr m_pool_link;
	boost::uuids::uuid m_pool_id;
	object_vid m_id;
	bool m_one_way_mode;

private:
	static exception_ptr make_link_error();

	template<typename Call>
	boost::shared_ptr<typename Call::result_type>
	make_result(boost::shared_ptr<Call> const& _call)
	{
		return boost::make_shared<typename Call::result_type>(_call);
	}

protected:
	BOOST_SERIALIZATION_SPLIT_MEMBER()

	template<typename Archive>
	void save(Archive& ar, unsigned int const) const
	{
		using namespace boost::serialization;
		boost::uuids::uuid pid = pool_id();
		ar << make_nvp("pool_id", pid);
		ar << make_nvp("id", m_id);
	}

	template<typename Archive>
	void load(Archive& ar, unsigned int const)
	{
		using namespace boost::serialization;
		ar >> make_nvp("pool_id", m_pool_id);
		ar >> make_nvp("id", m_id);
	}

	template<typename Call>
	boost::shared_ptr<typename Call::basic_result_type> invoke()
	{
		typedef typename Call::basic_result_type result_type;

		pool_link_ptr link = m_pool_link.lock();
		if(!link)
			return boost::make_shared<result_type>(make_link_error());

		boost::shared_ptr<Call> _call = boost::make_shared<Call>(id());
		if(m_one_way_mode)
		{
			link->invoke(_call);
			return boost::make_shared<result_type>(one_way());;
		}
		else
		{
			boost::shared_ptr<result_type> _result = make_result(_call);
			link->invoke(_call, _result);
			return _result;
		}
	}

	template<typename Call>
	boost::shared_ptr<typename Call::basic_result_type> invoke(
		param<typename Call::arg1_type> const& a1)
	{
		typedef typename Call::basic_result_type result_type;

		pool_link_ptr link = m_pool_link.lock();
		if(!link)
			return boost::make_shared<result_type>(make_link_error());

		boost::shared_ptr<Call> _call = boost::make_shared<Call>(id(), a1);
		if(m_one_way_mode)
		{
			link->invoke(_call);
			return boost::make_shared<result_type>(one_way());;
		}
		else
		{
			boost::shared_ptr<result_type> _result = make_result(_call);
			link->invoke(_call, _result);
			return _result;
		}
	}

	template<typename Call>
	boost::shared_ptr<typename Call::basic_result_type> invoke(
		param<typename Call::arg1_type> const& a1,
		param<typename Call::arg2_type> const& a2)
	{
		typedef typename Call::basic_result_type result_type;

		pool_link_ptr link = m_pool_link.lock();
		if(!link)
			return boost::make_shared<result_type>(make_link_error());

		boost::shared_ptr<Call> _call = boost::make_shared<Call>(id(), a1, a2);
		if(m_one_way_mode)
		{
			link->invoke(_call);
			return boost::make_shared<result_type>(one_way());;
		}
		else
		{
			boost::shared_ptr<result_type> _result = make_result(_call);
			link->invoke(_call, _result);
			return _result;
		}
	}

	template<typename Call>
	boost::shared_ptr<typename Call::basic_result_type> invoke(
		param<typename Call::arg1_type> const& a1,
		param<typename Call::arg2_type> const& a2,
		param<typename Call::arg3_type> const& a3)
	{
		typedef typename Call::basic_result_type result_type;

		pool_link_ptr link = m_pool_link.lock();
		if(!link)
			return boost::make_shared<result_type>(make_link_error());

		boost::shared_ptr<Call> _call = boost::make_shared<Call>(id(), a1, a2, a3);
		if(m_one_way_mode)
		{
			link->invoke(_call);
			return boost::make_shared<result_type>(one_way());;
		}
		else
		{
			boost::shared_ptr<result_type> _result = make_result(_call);
			link->invoke(_call, _result);
			return _result;
		}
	}

	template<typename Call>
	boost::shared_ptr<typename Call::basic_result_type> invoke(
		param<typename Call::arg1_type> const& a1,
		param<typename Call::arg2_type> const& a2,
		param<typename Call::arg3_type> const& a3,
		param<typename Call::arg4_type> const& a4)
	{
		typedef typename Call::basic_result_type result_type;

		pool_link_ptr link = m_pool_link.lock();
		if(!link)
			return boost::make_shared<result_type>(make_link_error());

		boost::shared_ptr<Call> _call = boost::make_shared<Call>(id(), a1, a2, a3, a4);
		if(m_one_way_mode)
		{
			link->invoke(_call);
			return boost::make_shared<result_type>(one_way());;
		}
		else
		{
			boost::shared_ptr<result_type> _result = make_result(_call);
			link->invoke(_call, _result);
			return _result;
		}
	}

	template<typename Call>
	boost::shared_ptr<typename Call::basic_result_type> invoke(
		param<typename Call::arg1_type> const& a1,
		param<typename Call::arg2_type> const& a2,
		param<typename Call::arg3_type> const& a3,
		param<typename Call::arg4_type> const& a4,
		param<typename Call::arg5_type> const& a5)
	{
		typedef typename Call::basic_result_type result_type;

		pool_link_ptr link = m_pool_link.lock();
		if(!link)
			return boost::make_shared<result_type>(make_link_error());

		boost::shared_ptr<Call> _call = boost::make_shared<Call>(id(), a1, a2, a3, a4, a5);
		if(m_one_way_mode)
		{
			link->invoke(_call);
			return boost::make_shared<result_type>(one_way());;
		}
		else
		{
			boost::shared_ptr<result_type> _result = make_result(_call);
			link->invoke(_call, _result);
			return _result;
		}
	}

	template<typename Call>
	boost::shared_ptr<typename Call::basic_result_type> invoke(
		param<typename Call::arg1_type> const& a1,
		param<typename Call::arg2_type> const& a2,
		param<typename Call::arg3_type> const& a3,
		param<typename Call::arg4_type> const& a4,
		param<typename Call::arg5_type> const& a5,
		param<typename Call::arg6_type> const& a6)
	{
		typedef typename Call::basic_result_type result_type;

		pool_link_ptr link = m_pool_link.lock();
		if(!link)
			return boost::make_shared<result_type>(make_link_error());

		boost::shared_ptr<Call> _call = boost::make_shared<Call>(id(), a1, a2, a3, a4, a5, a6);
		if(m_one_way_mode)
		{
			link->invoke(_call);
			return boost::make_shared<result_type>(one_way());;
		}
		else
		{
			boost::shared_ptr<result_type> _result = make_result(_call);
			link->invoke(_call, _result);
			return _result;
		}
	}
};

}
}

#endif
