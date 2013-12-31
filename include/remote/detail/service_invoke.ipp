// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------


template<typename T, typename TR, typename R>
void invoke(pool_node& node, TR (T::*mem_fn)() REMOTE_CC,
			boost::shared_ptr<T> const& target, basic_call_0<R>& _call, REMOTE_CC_TAG tag)
{
	invoke(node, mem_fn, target, _call, tag, is_future<TR>());
}

template<typename T, typename TR, typename R, typename A1>
void invoke(pool_node& node, TR (T::*mem_fn)(typename boost::mpl::identity<A1>::type) REMOTE_CC,
			boost::shared_ptr<T> const& target, basic_call_1<R, A1>& _call, REMOTE_CC_TAG tag)
{
	invoke(node, mem_fn, target, _call, tag, is_future<TR>());
}

template<typename T, typename TR, typename R, typename A1, typename A2>
void invoke(pool_node& node, TR (T::*mem_fn)(typename boost::mpl::identity<A1>::type,
											 typename boost::mpl::identity<A2>::type) REMOTE_CC,
			boost::shared_ptr<T> const& target, basic_call_2<R, A1, A2>& _call, REMOTE_CC_TAG tag)
{
	invoke(node, mem_fn, target, _call, tag, is_future<TR>());
}

template<typename T, typename TR, typename R, typename A1, typename A2, typename A3>
void invoke(pool_node& node, TR (T::*mem_fn)(typename boost::mpl::identity<A1>::type,
											 typename boost::mpl::identity<A2>::type,
											 typename boost::mpl::identity<A3>::type) REMOTE_CC,
			boost::shared_ptr<T> const& target, basic_call_3<R, A1, A2, A3>& _call, REMOTE_CC_TAG tag)
{
	invoke(node, mem_fn, target, _call, tag, is_future<TR>());
}

template<typename T, typename TR, typename R, typename A1, typename A2, typename A3, typename A4>
void invoke(pool_node& node, TR (T::*mem_fn)(typename boost::mpl::identity<A1>::type,
											 typename boost::mpl::identity<A2>::type,
											 typename boost::mpl::identity<A3>::type,
											 typename boost::mpl::identity<A4>::type) REMOTE_CC,
			boost::shared_ptr<T> const& target, basic_call_4<R, A1, A2, A3, A4>& _call, REMOTE_CC_TAG tag)
{
	invoke(node, mem_fn, target, _call, tag, is_future<TR>());
}

template<typename T, typename TR, typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
void invoke(pool_node& node, TR (T::*mem_fn)(typename boost::mpl::identity<A1>::type,
											 typename boost::mpl::identity<A2>::type,
											 typename boost::mpl::identity<A3>::type,
											 typename boost::mpl::identity<A4>::type,
											 typename boost::mpl::identity<A5>::type) REMOTE_CC,
			boost::shared_ptr<T> const& target, basic_call_5<R, A1, A2, A3, A4, A5>& _call, REMOTE_CC_TAG tag)
{
	invoke(node, mem_fn, target, _call, tag, is_future<TR>());
}

template<typename T, typename TR, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
void invoke(pool_node& node, TR (T::*mem_fn)(typename boost::mpl::identity<A1>::type,
											 typename boost::mpl::identity<A2>::type,
											 typename boost::mpl::identity<A3>::type,
											 typename boost::mpl::identity<A4>::type,
											 typename boost::mpl::identity<A5>::type,
											 typename boost::mpl::identity<A6>::type) REMOTE_CC,
			boost::shared_ptr<T> const& target, basic_call_6<R, A1, A2, A3, A4, A5, A6>& _call, REMOTE_CC_TAG tag)
{
	invoke(node, mem_fn, target, _call, tag, is_future<TR>());
}


template<typename T, typename MF>
void invoke(pool_node&, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_0<void>&, REMOTE_CC_TAG, sync_tag)
{
	((*target).*mem_fn)();
}

template<typename T, typename MF, typename A1>
void invoke(pool_node& node, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_1<void, A1>& _call, REMOTE_CC_TAG, sync_tag)
{
	((*target).*mem_fn)(marshal(node, _call.m_a1.get()));
}

template<typename T, typename MF, typename A1, typename A2>
void invoke(pool_node& node, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_2<void, A1, A2>& _call, REMOTE_CC_TAG, sync_tag)
{
	((*target).*mem_fn)(marshal(node, _call.m_a1.get()),
						marshal(node, _call.m_a2.get()));
}

template<typename T, typename MF, typename A1, typename A2, typename A3>
void invoke(pool_node& node, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_3<void, A1, A2, A3>& _call, REMOTE_CC_TAG, sync_tag)
{
	((*target).*mem_fn)(marshal(node, _call.m_a1.get()),
						marshal(node, _call.m_a2.get()),
						marshal(node, _call.m_a3.get()));
}

template<typename T, typename MF, typename A1, typename A2, typename A3, typename A4>
void invoke(pool_node& node, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_4<void, A1, A2, A3, A4>& _call, REMOTE_CC_TAG, sync_tag)
{
	((*target).*mem_fn)(marshal(node, _call.m_a1.get()),
						marshal(node, _call.m_a2.get()),
						marshal(node, _call.m_a3.get()),
						marshal(node, _call.m_a4.get()));
}

template<typename T, typename MF, typename A1, typename A2, typename A3, typename A4, typename A5>
void invoke(pool_node& node, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_5<void, A1, A2, A3, A4, A5>& _call, REMOTE_CC_TAG, sync_tag)
{
	((*target).*mem_fn)(marshal(node, _call.m_a1.get()),
						marshal(node, _call.m_a2.get()),
						marshal(node, _call.m_a3.get()),
						marshal(node, _call.m_a4.get()),
						marshal(node, _call.m_a5.get()));
}

template<typename T, typename MF, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
void invoke(pool_node& node, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_6<void, A1, A2, A3, A4, A5, A6>& _call, REMOTE_CC_TAG, sync_tag)
{
	((*target).*mem_fn)(marshal(node, _call.m_a1.get()),
						marshal(node, _call.m_a2.get()),
						marshal(node, _call.m_a3.get()),
						marshal(node, _call.m_a4.get()),
						marshal(node, _call.m_a5.get()),
						marshal(node, _call.m_a6.get()));
}


template<typename T, typename MF, typename R>
void invoke(pool_node&, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_0<R>& _call, REMOTE_CC_TAG, sync_tag)
{
	_call.m_r = ((*target).*mem_fn)();
}

template<typename T, typename MF, typename R, typename A1>
void invoke(pool_node& node, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_1<R, A1>& _call, REMOTE_CC_TAG, sync_tag)
{
	_call.m_r = ((*target).*mem_fn)(marshal(node, _call.m_a1.get()));
}

template<typename T, typename MF, typename R, typename A1, typename A2>
void invoke(pool_node& node, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_2<R, A1, A2>& _call, REMOTE_CC_TAG, sync_tag)
{
	_call.m_r = ((*target).*mem_fn)(marshal(node, _call.m_a1.get()),
									marshal(node, _call.m_a2.get()));
}

template<typename T, typename MF, typename R, typename A1, typename A2, typename A3>
void invoke(pool_node& node, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_3<R, A1, A2, A3>& _call, REMOTE_CC_TAG, sync_tag)
{
	_call.m_r = ((*target).*mem_fn)(marshal(node, _call.m_a1.get()),
									marshal(node, _call.m_a2.get()),
									marshal(node, _call.m_a3.get()));
}

template<typename T, typename MF, typename R, typename A1, typename A2, typename A3, typename A4>
void invoke(pool_node& node, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_4<R, A1, A2, A3, A4>& _call, REMOTE_CC_TAG, sync_tag)
{
	_call.m_r = ((*target).*mem_fn)(marshal(node, _call.m_a1.get()),
									marshal(node, _call.m_a2.get()),
									marshal(node, _call.m_a3.get()),
									marshal(node, _call.m_a4.get()));
}

template<typename T, typename MF, typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
void invoke(pool_node& node, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_5<R, A1, A2, A3, A4, A5>& _call, REMOTE_CC_TAG, sync_tag)
{
	_call.m_r = ((*target).*mem_fn)(marshal(node, _call.m_a1.get()),
									marshal(node, _call.m_a2.get()),
									marshal(node, _call.m_a3.get()),
									marshal(node, _call.m_a4.get()),
									marshal(node, _call.m_a5.get()));
}

template<typename T, typename MF, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
void invoke(pool_node& node, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_6<R, A1, A2, A3, A4, A5, A6>& _call, REMOTE_CC_TAG, sync_tag)
{
	_call.m_r = ((*target).*mem_fn)(marshal(node, _call.m_a1.get()),
									marshal(node, _call.m_a2.get()),
									marshal(node, _call.m_a3.get()),
									marshal(node, _call.m_a4.get()),
									marshal(node, _call.m_a5.get()),
									marshal(node, _call.m_a6.get()));
}


template<typename T, typename MF, typename R>
void invoke(pool_node&, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_0<R>& _call, REMOTE_CC_TAG, async_tag)
{
	poll_result(_call.m_r, ((*target).*mem_fn)());
}

template<typename T, typename MF, typename R, typename A1>
void invoke(pool_node& node, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_1<R, A1>& _call, REMOTE_CC_TAG, async_tag)
{
	poll_result(_call.m_r, ((*target).*mem_fn)(marshal(node, _call.m_a1.get())));
}

template<typename T, typename MF, typename R, typename A1, typename A2>
void invoke(pool_node& node, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_2<R, A1, A2>& _call, REMOTE_CC_TAG, async_tag)
{
	poll_result(_call.m_r, ((*target).*mem_fn)(marshal(node, _call.m_a1.get()),
												marshal(node, _call.m_a2.get())));
}

template<typename T, typename MF, typename R, typename A1, typename A2, typename A3>
void invoke(pool_node& node, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_3<R, A1, A2, A3>& _call, REMOTE_CC_TAG, async_tag)
{
	poll_result(_call.m_r, ((*target).*mem_fn)(marshal(node, _call.m_a1.get()),
												marshal(node, _call.m_a2.get()),
												marshal(node, _call.m_a3.get())));
}

template<typename T, typename MF, typename R, typename A1, typename A2, typename A3, typename A4>
void invoke(pool_node& node, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_4<R, A1, A2, A3, A4>& _call, REMOTE_CC_TAG, async_tag)
{
	poll_result(_call.m_r, ((*target).*mem_fn)(marshal(node, _call.m_a1.get()),
												marshal(node, _call.m_a2.get()),
												marshal(node, _call.m_a3.get()),
												marshal(node, _call.m_a4.get())));
}

template<typename T, typename MF, typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
void invoke(pool_node& node, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_5<R, A1, A2, A3, A4, A5>& _call, REMOTE_CC_TAG, async_tag)
{
	poll_result(_call.m_r, ((*target).*mem_fn)(marshal(node, _call.m_a1.get()),
												marshal(node, _call.m_a2.get()),
												marshal(node, _call.m_a3.get()),
												marshal(node, _call.m_a4.get()),
												marshal(node, _call.m_a5.get())));
}

template<typename T, typename MF, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
void invoke(pool_node& node, MF mem_fn, boost::shared_ptr<T> const& target, basic_call_6<R, A1, A2, A3, A4, A5, A6>& _call, REMOTE_CC_TAG, async_tag)
{
	poll_result(_call.m_r, ((*target).*mem_fn)(marshal(node, _call.m_a1.get()),
												marshal(node, _call.m_a2.get()),
												marshal(node, _call.m_a3.get()),
												marshal(node, _call.m_a4.get()),
												marshal(node, _call.m_a5.get()),
												marshal(node, _call.m_a6.get())));
}
