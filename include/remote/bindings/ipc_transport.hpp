// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_IPC_TRANSPORT_HPP__
#define __REMOTE_BINDINGS_IPC_TRANSPORT_HPP__
#include <remote/bindings/named_pipe_transport.hpp>
#include <remote/bindings/local_transport.hpp>

namespace remote
{
namespace bindings
{

#if defined(BOOST_ASIO_HAS_WINDOWS_STREAM_HANDLE)
typedef named_pipe_transport ipc_transport;
#elif defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
typedef local_transport ipc_transport;
#endif

}
}

#endif

