// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_CALL_HPP__
#define __REMOTE_DETAIL_CALL_HPP__

#include <remote/fwd.hpp>
#include <remote/detail/type_traits.hpp>
#include <remote/detail/serialize_param.hpp>
#include <remote/detail/serialize_primitive.hpp>
#include <remote/remote_error.hpp>

#include <boost/smart_ptr/make_shared.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <vector>

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4512)
#endif

#include <boost/serialization/variant.hpp>

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif


namespace remote
{
namespace detail
{

template<typename R>
class result_0;

template<typename R, typename A1>
class result_1;

template<typename R, typename A1, typename A2>
class result_2;

template<typename R, typename A1, typename A2, typename A3>
class result_3;

template<typename R, typename A1, typename A2, typename A3, typename A4>
class result_4;

template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
class result_5;

template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class result_6;


class call
{
public:
	call();
	call(object_vid const& oid);
	virtual ~call() {}

	call_id id() const { return m_id; }
	void set_id(call_id id) { m_id = id; }
	bool one_way() const { return (m_id == 0); }

	object_vid object_id() const { return m_object_id; }
	bool ready() const { return m_ready; }

	void set_exception(exception_ptr const& e);
	void invoke(pool_node_ptr const& node, service_ptr const& svc);

	virtual void invoke(pool_node&, service&) = 0;

protected:
	friend class result;
	typedef std::vector<boost::shared_ptr<void const> > container_type;

protected:
	call_id m_id;
	bool m_ready;
	object_vid m_object_id;

	exception_ptr m_exception;
	container_type m_pointers;

protected:
	template<typename T, typename Call>
	void invoke(pool_node& node, service& svc, Call& _call)
	{
		T* svc_base = dynamic_cast<T*>(&svc);
		if(!svc_base)
			throw remote_error(remote_error::invalid_service_type);

		svc_base->invoke(node, _call);
	}

	template<typename Archive>
	struct scoped_pointer_tracker: public boost::noncopyable
	{
		scoped_pointer_tracker(Archive& ar, container_type& pointers)
		: m_ar(ar)
		{
			set_container(pointers, typename Archive::is_loading());
		}

		~scoped_pointer_tracker()
		{
			clear_container(typename Archive::is_loading());
		}

	private:
		Archive& m_ar;

		void set_container(container_type& pointers, boost::mpl::true_)
		{
			m_ar.reset_container(&pointers);
		}

		void set_container(container_type&, boost::mpl::false_)
		{}

		void clear_container(boost::mpl::true_)
		{
			m_ar.reset_container(0);
		}

		void clear_container(boost::mpl::false_)
		{}
	};

	template<typename Archive, typename T>
	void serialize_param(Archive& ar, char const* tag, param<T>& p, unsigned int const v)
	{
		serialize_param(ar, tag, p, v, is_out_param<T>());
	}

	template<typename Archive, typename T>
	void serialize_result(Archive& ar, param<T>& r, unsigned int const)
	{
		ar & boost::serialization::make_nvp("r", r);
	}

	template<typename Archive>
	void serialize_result(Archive&, param<void>&, unsigned int const)
	{}

	template<typename Archive>
	void serialize(Archive& ar, unsigned int const)
	{
		using namespace boost::serialization;
		ar & make_nvp("id", m_id);
		ar & make_nvp("object_id", m_object_id);
		ar & make_nvp("ready", m_ready);

		if(m_ready)
		{
			exception* e = m_exception.get();
			ar & make_nvp("exception", e);
			reset_pointer(ar, m_exception, e, typename Archive::is_loading());
		}
	}

private:
	template<typename Archive, typename T>
	void reset_pointer(Archive&, boost::shared_ptr<T>&, T*, boost::mpl::false_)
	{}

	template<typename Archive, typename T>
	void reset_pointer(Archive& ar, boost::shared_ptr<T>& s, T* t, boost::mpl::true_)
	{
		ar.reset(s, t);
	}

	template<typename Archive, typename T>
	void serialize_param(Archive& ar, char const* tag, param<T>& p, unsigned int const, boost::mpl::false_)
	{
		if(m_ready)
			return;

		ar & boost::serialization::make_nvp(tag, p);
	}

	template<typename Archive, typename T>
	void serialize_param(Archive& ar, char const* tag, param<T>& p, unsigned int const, boost::mpl::true_)
	{
		ar & boost::serialization::make_nvp(tag, p);
	}
};

template<typename R>
class basic_call : public call
{
public:
	typedef R value_type;
	typedef basic_result<R> basic_result_type;

public:
	basic_call()
	{}

	basic_call(object_vid const& oid)
	: call(oid)
	{}

protected:
	param<R> m_r;

protected:
	friend class service;
	friend class basic_result<R>;

	template<typename Archive>
	void serialize(Archive& ar, unsigned int const v)
	{
		call::serialize(ar, v);
		if(!m_ready || this->m_exception)
			return;

		serialize_result(ar, m_r, v);
	}
};

template<typename R>
class basic_call_0 : public basic_call<R>
{
public:
	typedef result_0<R> result_type;

public:
	basic_call_0()
	{}

	basic_call_0(object_vid const& oid)
	: base_type(oid)
	{}

private:
	friend class service;
	friend class result_0<R>;
	typedef basic_call<R> base_type;

protected:
	template<typename Archive>
	void serialize(Archive& ar, unsigned int const v)
	{
		base_type::serialize(ar, v);
	}
};

template<typename R, typename A1>
class basic_call_1 : public basic_call<R>
{
public:
	typedef result_1<R, A1> result_type;
	typedef A1 arg1_type;

public:
	basic_call_1()
	{}

	basic_call_1(object_vid const& oid, param<A1> const& a1)
	: base_type(oid)
	, m_a1(a1)
	{}

private:
	friend class service;
	friend class result_1<R, A1>;
	typedef basic_call<R> base_type;

protected:
	param<A1> m_a1;

protected:
	template<typename Archive>
	void serialize(Archive& ar, unsigned int const v)
	{
		base_type::serialize(ar, v);
		if(this->m_exception)
			return;

		this->serialize_param(ar, "a1", m_a1, v);
	}
};

template<typename R, typename A1, typename A2>
class basic_call_2 : public basic_call<R>
{
public:
	typedef result_2<R, A1, A2> result_type;
	typedef A1 arg1_type;
	typedef A2 arg2_type;

public:
	basic_call_2()
	{}

	basic_call_2(object_vid const& oid, param<A1> const& a1, param<A2> const& a2)
	: base_type(oid)
	, m_a1(a1)
	, m_a2(a2)
	{}

private:
	friend class service;
	friend class result_2<R, A1, A2>;
	typedef basic_call<R> base_type;

protected:
	param<A1> m_a1;
	param<A2> m_a2;

protected:
	template<typename Archive>
	void serialize(Archive& ar, unsigned int const v)
	{
		base_type::serialize(ar, v);
		if(this->m_exception)
			return;

		this->serialize_param(ar, "a1", m_a1, v);
		this->serialize_param(ar, "a2", m_a2, v);
	}
};

template<typename R, typename A1, typename A2, typename A3>
class basic_call_3 : public basic_call<R>
{
public:
	typedef result_3<R, A1, A2, A3> result_type;
	typedef A1 arg1_type;
	typedef A2 arg2_type;
	typedef A3 arg3_type;

public:
	basic_call_3()
	{}

	basic_call_3(object_vid const& oid, param<A1> const& a1, param<A2> const& a2,
										param<A3> const& a3)
	: base_type(oid)
	, m_a1(a1)
	, m_a2(a2)
	, m_a3(a3)
	{}

private:
	friend class service;
	friend class result_3<R, A1, A2, A3>;
	typedef basic_call<R> base_type;

protected:
	param<A1> m_a1;
	param<A2> m_a2;
	param<A3> m_a3;

protected:
	template<typename Archive>
	void serialize(Archive& ar, unsigned int const v)
	{
		base_type::serialize(ar, v);
		if(this->m_exception)
			return;

		this->serialize_param(ar, "a1", m_a1, v);
		this->serialize_param(ar, "a2", m_a2, v);
		this->serialize_param(ar, "a3", m_a3, v);
	}
};

template<typename R, typename A1, typename A2, typename A3, typename A4>
class basic_call_4 : public basic_call<R>
{
public:
	typedef result_4<R, A1, A2, A3, A4> result_type;
	typedef A1 arg1_type;
	typedef A2 arg2_type;
	typedef A3 arg3_type;
	typedef A4 arg4_type;

public:
	basic_call_4()
	{}

	basic_call_4(object_vid const& oid, param<A1> const& a1, param<A2> const& a2,
										param<A3> const& a3, param<A4> const& a4)
	: base_type(oid)
	, m_a1(a1)
	, m_a2(a2)
	, m_a3(a3)
	, m_a4(a4)
	{}

private:
	friend class service;
	friend class result_4<R, A1, A2, A3, A4>;
	typedef basic_call<R> base_type;

protected:
	param<A1> m_a1;
	param<A2> m_a2;
	param<A3> m_a3;
	param<A4> m_a4;

protected:
	template<typename Archive>
	void serialize(Archive& ar, unsigned int const v)
	{
		base_type::serialize(ar, v);
		if(this->m_exception)
			return;

		this->serialize_param(ar, "a1", m_a1, v);
		this->serialize_param(ar, "a2", m_a2, v);
		this->serialize_param(ar, "a3", m_a3, v);
		this->serialize_param(ar, "a4", m_a4, v);
	}
};

template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
class basic_call_5 : public basic_call<R>
{
public:
	typedef result_5<R, A1, A2, A3, A4, A5> result_type;
	typedef A1 arg1_type;
	typedef A2 arg2_type;
	typedef A3 arg3_type;
	typedef A4 arg4_type;
	typedef A5 arg5_type;

public:
	basic_call_5()
	{}

	basic_call_5(object_vid const& oid, param<A1> const& a1, param<A2> const& a2,
										param<A3> const& a3, param<A4> const& a4,
										param<A5> const& a5)
	: base_type(oid)
	, m_a1(a1)
	, m_a2(a2)
	, m_a3(a3)
	, m_a4(a4)
	, m_a5(a5)
	{}

private:
	friend class service;
	friend class result_5<R, A1, A2, A3, A4, A5>;
	typedef basic_call<R> base_type;

protected:
	param<A1> m_a1;
	param<A2> m_a2;
	param<A3> m_a3;
	param<A4> m_a4;
	param<A5> m_a5;

protected:
	template<typename Archive>
	void serialize(Archive& ar, unsigned int const v)
	{
		base_type::serialize(ar, v);
		if(this->m_exception)
			return;

		this->serialize_param(ar, "a1", m_a1, v);
		this->serialize_param(ar, "a2", m_a2, v);
		this->serialize_param(ar, "a3", m_a3, v);
		this->serialize_param(ar, "a4", m_a4, v);
		this->serialize_param(ar, "a5", m_a5, v);
	}
};

template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class basic_call_6 : public basic_call<R>
{
public:
	typedef result_6<R, A1, A2, A3, A4, A5, A6> result_type;
	typedef A1 arg1_type;
	typedef A2 arg2_type;
	typedef A3 arg3_type;
	typedef A4 arg4_type;
	typedef A5 arg5_type;
	typedef A6 arg6_type;

public:
	basic_call_6()
	{}

	basic_call_6(object_vid const& oid, param<A1> const& a1, param<A2> const& a2,
										param<A3> const& a3, param<A4> const& a4,
										param<A5> const& a5, param<A6> const& a6)
	: base_type(oid)
	, m_a1(a1)
	, m_a2(a2)
	, m_a3(a3)
	, m_a4(a4)
	, m_a5(a5)
	, m_a6(a6)
	{}

private:
	friend class service;
	friend class result_6<R, A1, A2, A3, A4, A5, A6>;
	typedef basic_call<R> base_type;

protected:
	param<A1> m_a1;
	param<A2> m_a2;
	param<A3> m_a3;
	param<A4> m_a4;
	param<A5> m_a5;
	param<A6> m_a6;

protected:
	template<typename Archive>
	void serialize(Archive& ar, unsigned int const v)
	{
		base_type::serialize(ar, v);
		if(this->m_exception)
			return;

		this->serialize_param(ar, "a1", m_a1, v);
		this->serialize_param(ar, "a2", m_a2, v);
		this->serialize_param(ar, "a3", m_a3, v);
		this->serialize_param(ar, "a4", m_a4, v);
		this->serialize_param(ar, "a5", m_a5, v);
		this->serialize_param(ar, "a6", m_a6, v);
	}
};

template<typename T, typename M, typename R>
class call_0 : public basic_call_0<R>
{
public:
	typedef M tag;

	call_0()
	{}

	call_0(object_vid const& oid)
	: base_type(oid)
	{}

	void invoke(pool_node& node, service& svc)
	{
		call::invoke<T>(node, svc, *this);
	}

private:
	friend class boost::serialization::access;
	typedef basic_call_0<R> base_type;
	typedef call_0<T, M, R> this_type;

	template<typename Archive>
	void serialize(Archive& ar, unsigned int const v)
	{
		call::scoped_pointer_tracker<Archive> track(ar, this->m_pointers);
		boost::serialization::void_cast_register<this_type, call>();
		base_type::serialize(ar, v);
	}
};

template<typename T, typename M, typename R, typename A1>
class call_1 : public basic_call_1<R, A1>
{
public:
	typedef M tag;

	call_1()
	{}

	call_1(object_vid const& oid, param<A1> const& a1)
	: base_type(oid, a1)
	{}

	void invoke(pool_node& node, service& svc)
	{
		call::invoke<T>(node, svc, *this);
	}

private:
	friend class boost::serialization::access;
	typedef basic_call_1<R, A1> base_type;
	typedef call_1<T, M, R, A1> this_type;

	template<typename Archive>
	void serialize(Archive& ar, unsigned int const v)
	{
		call::scoped_pointer_tracker<Archive> track(ar, this->m_pointers);
		boost::serialization::void_cast_register<this_type, call>();
		base_type::serialize(ar, v);
	}
};

template<typename T, typename M, typename R, typename A1, typename A2>
class call_2 : public basic_call_2<R, A1, A2>
{
public:
	typedef M tag;

	call_2()
	{}

	call_2(object_vid const& oid, param<A1> const& a1, param<A2> const& a2)
	: base_type(oid, a1, a2)
	{}

	void invoke(pool_node& node, service& svc)
	{
		call::invoke<T>(node, svc, *this);
	}

private:
	friend class boost::serialization::access;
	typedef basic_call_2<R, A1, A2> base_type;
	typedef call_2<T, M, R, A1, A2> this_type;

	template<typename Archive>
	void serialize(Archive& ar, unsigned int const v)
	{
		call::scoped_pointer_tracker<Archive> track(ar, this->m_pointers);
		boost::serialization::void_cast_register<this_type, call>();
		base_type::serialize(ar, v);
	}
};

template<typename T, typename M, typename R, typename A1, typename A2, typename A3>
class call_3 : public basic_call_3<R, A1, A2, A3>
{
public:
	typedef M tag;

	call_3()
	{}

	call_3(object_vid const& oid, param<A1> const& a1, param<A2> const& a2,
			param<A3> const& a3)
	: base_type(oid, a1, a2, a3)
	{}

	void invoke(pool_node& node, service& svc)
	{
		call::invoke<T>(node, svc, *this);
	}

private:
	friend class boost::serialization::access;
	typedef basic_call_3<R, A1, A2, A3> base_type;
	typedef call_3<T, M, R, A1, A2, A3> this_type;

	template<typename Archive>
	void serialize(Archive& ar, unsigned int const v)
	{
		call::scoped_pointer_tracker<Archive> track(ar, this->m_pointers);
		boost::serialization::void_cast_register<this_type, call>();
		base_type::serialize(ar, v);
	}
};

template<typename T, typename M, typename R, typename A1, typename A2, typename A3, typename A4>
class call_4 : public basic_call_4<R, A1, A2, A3, A4>
{
public:
	typedef M tag;

	call_4()
	{}

	call_4(object_vid const& oid, param<A1> const& a1, param<A2> const& a2,
			param<A3> const& a3, param<A4> const& a4)
	: base_type(oid, a1, a2, a3, a4)
	{}

	void invoke(pool_node& node, service& svc)
	{
		call::invoke<T>(node, svc, *this);
	}

private:
	friend class boost::serialization::access;
	typedef basic_call_4<R, A1, A2, A3, A4> base_type;
	typedef call_4<T, M, R, A1, A2, A3, A4> this_type;

	template<typename Archive>
	void serialize(Archive& ar, unsigned int const v)
	{
		call::scoped_pointer_tracker<Archive> track(ar, this->m_pointers);
		boost::serialization::void_cast_register<this_type, call>();
		base_type::serialize(ar, v);
	}
};

template<typename T, typename M, typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
class call_5 : public basic_call_5<R, A1, A2, A3, A4, A5>
{
public:
	typedef M tag;

	call_5()
	{}

	call_5(object_vid const& oid, param<A1> const& a1, param<A2> const& a2,
			param<A3> const& a3, param<A4> const& a4, param<A5> const& a5)
	: base_type(oid, a1, a2, a3, a4, a5)
	{}

	void invoke(pool_node& node, service& svc)
	{
		call::invoke<T>(node, svc, *this);
	}

private:
	friend class boost::serialization::access;
	typedef basic_call_5<R, A1, A2, A3, A4, A5> base_type;
	typedef call_5<T, M, R, A1, A2, A3, A4, A5> this_type;

	template<typename Archive>
	void serialize(Archive& ar, unsigned int const v)
	{
		call::scoped_pointer_tracker<Archive> track(ar, this->m_pointers);
		boost::serialization::void_cast_register<this_type, call>();
		base_type::serialize(ar, v);
	}
};

template<typename T, typename M, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class call_6 : public basic_call_6<R, A1, A2, A3, A4, A5, A6>
{
public:
	typedef M tag;

	call_6()
	{}

	call_6(object_vid const& oid, param<A1> const& a1, param<A2> const& a2,
			param<A3> const& a3, param<A4> const& a4, param<A5> const& a5,
			param<A6> const& a6)
	: base_type(oid, a1, a2, a3, a4, a5, a6)
	{}

	void invoke(pool_node& node, service& svc)
	{
		call::invoke<T>(node, svc, *this);
	}

private:
	friend class boost::serialization::access;
	typedef basic_call_6<R, A1, A2, A3, A4, A5, A6> base_type;
	typedef call_6<T, M, R, A1, A2, A3, A4, A5, A6> this_type;

	template<typename Archive>
	void serialize(Archive& ar, unsigned int const v)
	{
		call::scoped_pointer_tracker<Archive> track(ar, this->m_pointers);
		boost::serialization::void_cast_register<this_type, call>();
		base_type::serialize(ar, v);
	}
};



}
}

#endif
