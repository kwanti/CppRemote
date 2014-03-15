//-----------------------------------------------------------------------------
// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------



template<typename Proxy, typename R>
R session::invoke(R(Proxy::*mem_fn)() REMOTE_CC, std::string const& name)
{
	return m_link->invoke(mem_fn, name);
}

template<typename Proxy, typename R, typename A1>
R session::invoke(R(Proxy::*mem_fn)(A1) REMOTE_CC, std::string const& name,
	typename boost::mpl::identity<A1>::type a1)
{
	return m_link->invoke(mem_fn, name, a1);
}

template<typename Proxy, typename R, typename A1, typename A2>
R session::invoke(R(Proxy::*mem_fn)(A1, A2) REMOTE_CC, std::string const& name,
	typename boost::mpl::identity<A1>::type a1,
	typename boost::mpl::identity<A2>::type a2)
{
	return m_link->invoke(mem_fn, name, a1, a2);
}

template<typename Proxy, typename R, typename A1, typename A2, typename A3>
R session::invoke(R(Proxy::*mem_fn)(A1, A2, A3) REMOTE_CC, std::string const& name,
	typename boost::mpl::identity<A1>::type a1,
	typename boost::mpl::identity<A2>::type a2,
	typename boost::mpl::identity<A3>::type a3)
{
	return m_link->invoke(mem_fn, name, a1, a2, a3);
}

template<typename Proxy, typename R, typename A1, typename A2, typename A3, typename A4>
R session::invoke(R(Proxy::*mem_fn)(A1, A2, A3, A4) REMOTE_CC, std::string const& name,
	typename boost::mpl::identity<A1>::type a1,
	typename boost::mpl::identity<A2>::type a2,
	typename boost::mpl::identity<A3>::type a3,
	typename boost::mpl::identity<A4>::type a4)
{
	return m_link->invoke(mem_fn, name, a1, a2, a3, a4);
}

template<typename Proxy, typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
R session::invoke(R(Proxy::*mem_fn)(A1, A2, A3, A4, A5) REMOTE_CC, std::string const& name,
	typename boost::mpl::identity<A1>::type a1,
	typename boost::mpl::identity<A2>::type a2,
	typename boost::mpl::identity<A3>::type a3,
	typename boost::mpl::identity<A4>::type a4,
	typename boost::mpl::identity<A5>::type a5)
{
	return m_link->invoke(mem_fn, name, a1, a2, a3, a4, a5);
}

template<typename Proxy, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
R session::invoke(R(Proxy::*mem_fn)(A1, A2, A3, A4, A5, A6) REMOTE_CC, std::string const& name,
	typename boost::mpl::identity<A1>::type a1,
	typename boost::mpl::identity<A2>::type a2,
	typename boost::mpl::identity<A3>::type a3,
	typename boost::mpl::identity<A4>::type a4,
	typename boost::mpl::identity<A5>::type a5,
	typename boost::mpl::identity<A6>::type a6)
{
	return m_link->invoke(mem_fn, name, a1, a2, a3, a4, a5, a6);
}

