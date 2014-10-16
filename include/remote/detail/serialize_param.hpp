// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_SERIALIZE_PARAM_HPP__
#define __REMOTE_DETAIL_SERIALIZE_PARAM_HPP__

#include <remote/detail/param.hpp>

#include <boost/version.hpp>
#if (BOOST_VERSION >= 105600)	// workaround compile error in gcc mingw
#include <boost/serialization/singleton.hpp>
#include <boost/serialization/extended_type_info.hpp>
#endif

#include <boost/serialization/item_version_type.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/level.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/weak_ptr.hpp>
#include <boost/type_traits/is_same.hpp>


namespace boost
{
namespace serialization
{

template<typename Archive, typename T>
void serialize(Archive& ar, remote::detail::param<T>& p, unsigned int const v)
{
	split_free(ar, p, v);
}

template<typename Archive, typename T>
void save(Archive& ar, remote::detail::param<T> const& p, unsigned int const v)
{
	typedef typename is_same<typename implementation_level<T>::type,
						mpl::int_<object_class_info> >::type has_info;

	save(ar, p, v, has_info());
}

template<typename Archive, typename T>
void load(Archive& ar, remote::detail::param<T>& p, unsigned int const v)
{
	typedef typename is_same<typename implementation_level<T>::type,
						mpl::int_<object_class_info> >::type has_info;

	load(ar, p, v, has_info());
}

template<typename Archive, typename T>
void save(Archive& ar, remote::detail::param<T> const& p, unsigned int const, mpl::true_)
{
	item_version_type const item_version(version<T>::value);
	ar << make_nvp("item_version", item_version);

	save_construct_data_adl(ar, &p.get(), item_version);
	ar << make_nvp("param", p.get());
}

template<typename Archive, typename T>
void load(Archive& ar, remote::detail::param<T>& p, unsigned int const, mpl::true_)
{
	item_version_type item_version(0);
	ar >> make_nvp("item_version", item_version);

	typename remote::detail::param<T>::raw_type* ptr = p.allocate();

	load_construct_data_adl(ar, ptr, item_version);
	ar >> make_nvp("param", *ptr);
}

template<typename Archive, typename T>
void save(Archive& ar, remote::detail::param<T> const& p, unsigned int const, mpl::false_)
{
	save_construct_data_adl(ar, &p.get(), item_version_type(version<T>::value));
	ar << make_nvp("param", p.get());
}

template<typename Archive, typename T>
void load(Archive& ar, remote::detail::param<T>& p, unsigned int const, mpl::false_)
{
	typename remote::detail::param<T>::raw_type* ptr = p.allocate();

	load_construct_data_adl(ar, ptr, item_version_type(version<T>::value));
	ar >> make_nvp("param", *ptr);
}

template<typename Archive, typename T>
void save(Archive& ar, remote::detail::param<T&> const& p, unsigned int const)
{
	ar << make_nvp("param", p.m_ptr);
}

template<typename Archive, typename T>
void load(Archive& ar, remote::detail::param<T&>& p, unsigned int const)
{
	typename boost::remove_const<T>::type* ptr;
	ar >> make_nvp("param", ptr);
	p.m_ptr = ptr;
}

template<typename Archive, typename T>
void save(Archive& ar, remote::detail::param<T*> const& p, unsigned int const)
{
	ar << make_nvp("param", p.m_ptr);
}

template<typename Archive, typename T>
void load(Archive& ar, remote::detail::param<T*>& p, unsigned int const)
{
	typename boost::remove_const<T>::type* ptr;
	ar >> make_nvp("param", ptr);
	p.m_ptr = ptr;
}

template<typename Archive, typename T>
void save(Archive& ar, remote::detail::param<shared_ptr<T> > const& p, unsigned int const)
{
	ar << make_nvp("param", p.m_ptr);
}

template<typename Archive, typename T>
void load(Archive& ar, remote::detail::param<shared_ptr<T> >& p, unsigned int const)
{
	typedef typename boost::remove_const<T>::type U;
	shared_ptr<U> ptr;
	ar >> make_nvp("param", ptr);
	p.m_ptr = ptr;
}

template<typename Archive, typename T>
void save(Archive& ar, remote::detail::param<weak_ptr<T> > const& p, unsigned int const)
{
	boost::shared_ptr<T> sp = p.m_ptr.lock();
	ar << make_nvp("param", sp);
}

template<typename Archive, typename T>
void load(Archive& ar, remote::detail::param<weak_ptr<T> >& p, unsigned int const)
{
	boost::shared_ptr<T> sp;
	ar >> make_nvp("param",sp);
	p.m_ptr = sp;
}

}
}

#endif
