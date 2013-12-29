// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_TYPETRAITS_HPP__
#define __REMOTE_DETAIL_TYPETRAITS_HPP__

#include <remote/detail/fwd.hpp>

#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_abstract.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>


#if defined BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_FUTURE future
#else
#define BOOST_THREAD_FUTURE unique_future
#endif

namespace boost
{

template <typename R>
class BOOST_THREAD_FUTURE;

template <typename R>
class shared_future;

}

namespace remote
{
namespace detail
{

// future traits
template<typename T>
struct is_future : public boost::mpl::false_
{};

template<typename T>
struct is_future<remote::future<T> >
: public boost::mpl::true_
{};

template<typename T>
struct is_future<remote::shared_future<T> >
: public boost::mpl::true_
{};

template<typename T>
struct is_future<boost::BOOST_THREAD_FUTURE<T> >
: public boost::mpl::true_
{};

template<typename T>
struct is_future<boost::shared_future<T> >
: public boost::mpl::true_
{};


// shared_ptr traits
template<typename T>
struct is_shared_ptr : public boost::mpl::false_
{};

template<typename T>
struct is_shared_ptr<boost::shared_ptr<T> > : public boost::mpl::true_
{};

template<typename T>
struct is_shared_ptr<boost::shared_ptr<T> const> : public boost::mpl::true_
{};

template<typename T>
struct remove_shared_ptr
{
	typedef T type;
};

template<typename T>
struct remove_shared_ptr<boost::shared_ptr<T> >
{
	typedef T type;
};

template<typename T>
struct remove_shared_ptr<boost::shared_ptr<T> const>
{
	typedef T type;
};

// weak_ptr traits
template<typename T>
struct is_weak_ptr : public boost::mpl::false_
{};

template<typename T>
struct is_weak_ptr<boost::weak_ptr<T> > : public boost::mpl::true_
{};

template<typename T>
struct is_weak_ptr<boost::weak_ptr<T> const> : public boost::mpl::true_
{};

template<typename T>
struct remove_weak_ptr
{
	typedef T type;
};

template<typename T>
struct remove_weak_ptr<boost::weak_ptr<T> >
{
	typedef T type;
};

template<typename T>
struct remove_weak_ptr<boost::weak_ptr<T> const>
{
	typedef T type;
};


// any pointer traits
template<typename T>
struct is_any_pointer
: public boost::mpl::or_<boost::is_pointer<T>, is_shared_ptr<T>, is_weak_ptr<T> >
{};

template<typename T>
struct remove_any_pointer
{
	typedef typename boost::mpl::eval_if<boost::is_pointer<T>,
		boost::remove_pointer<T>,
		boost::mpl::eval_if<is_shared_ptr<T>,
			remove_shared_ptr<T>,
			boost::mpl::eval_if<is_weak_ptr<T>,
				remove_weak_ptr<T>,
				boost::mpl::identity<T> > > >::type type;
};


template<typename T>
struct remove_ref_and_ptr
{
	typedef typename boost::mpl::eval_if<boost::is_pointer<T>,
		boost::remove_pointer<T>,
		boost::mpl::eval_if<is_shared_ptr<T>,
			remove_shared_ptr<T>,
			boost::mpl::eval_if<is_weak_ptr<T>,
				remove_weak_ptr<T>,
				boost::mpl::eval_if<boost::is_reference<T>,
					boost::remove_reference<T>,
					boost::mpl::identity<T> > > > >::type type;
};

template<typename T>
struct remove_cv_ptr
{
	typedef typename boost::remove_cv<typename remove_any_pointer<T>::type>::type type;
};

template<typename T>
struct remove_cv_ref
{
	typedef typename boost::remove_cv<typename boost::remove_reference<T>::type>::type type;
};

template<typename T>
struct remove_cvrp
{
	typedef typename boost::remove_cv<typename remove_ref_and_ptr<T>::type>::type type;
};


// remote traits
template<typename T, typename U>
struct to_service
{};

template<typename T>
struct is_remote
: public boost::is_base_and_derived<proxy, T>
{};

template<typename T>
struct is_remote_pointer
: public boost::mpl::and_<boost::is_pointer<T>,
			is_remote<typename boost::remove_pointer<T>::type> >
{};

template<typename T>
struct is_remote_shared_ptr
: public boost::mpl::and_<is_shared_ptr<T>,
			is_remote<typename remove_shared_ptr<T>::type> >
{};

template<typename T>
struct is_remote_weak_ptr
: public boost::mpl::and_<is_weak_ptr<T>,
			is_remote<typename remove_weak_ptr<T>::type> >
{};

template<typename T>
struct is_any_remote_pointer
: public boost::mpl::and_<is_any_pointer<T>,
			is_remote<typename remove_any_pointer<T>::type> >
{};

template<typename T>
struct is_non_remote_pointer
: public boost::mpl::and_<is_any_pointer<T>,
			boost::mpl::not_<is_remote<typename remove_any_pointer<T>::type> > >
{};

template<typename T>
struct is_remote_reference
: public boost::mpl::and_<boost::is_reference<T>,
			is_remote<typename boost::remove_reference<T>::type> >
{};

template<typename T>
struct is_non_remote_reference
: public boost::mpl::and_<boost::is_reference<T>,
			boost::mpl::not_<is_remote<typename boost::remove_reference<T>::type> > >
{};


// out param traits
template<typename T, typename RawT = typename boost::remove_reference<T>::type>
struct is_out_param_reference
: public boost::mpl::and_<boost::is_reference<T>,
			boost::mpl::not_<is_remote<RawT> >,
			boost::mpl::not_<boost::is_const<RawT> >,
			boost::mpl::not_<boost::is_abstract<RawT> > >
{};

template<typename T, typename RawT = typename remove_any_pointer<T>::type>
struct is_out_param_pointer
: public boost::mpl::and_<is_any_pointer<T>,
			boost::mpl::not_<is_remote<RawT> >,
			boost::mpl::not_<boost::is_const<RawT> >,
			boost::mpl::not_<boost::is_abstract<RawT> > >
{};

template<typename T>
struct is_out_param
: public boost::mpl::or_<is_out_param_reference<T>, is_out_param_pointer<T> >
{};


// remote container traits
template<typename T>
struct is_remote_container : public boost::mpl::false_
{};

template<typename T>
struct need_to_marshal
: public boost::mpl::or_<is_any_remote_pointer<T>, is_remote_reference<T>,
			is_remote_container<typename remove_ref_and_ptr<T>::type> >
{};

}
}

#endif
