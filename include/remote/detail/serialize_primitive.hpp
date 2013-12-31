// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_SERIALIZE_PRIMITIVE_HPP__
#define __REMOTE_DETAIL_SERIALIZE_PRIMITIVE_HPP__

#include <remote/detail/param.hpp>
#include <remote/detail/pointer_cast.hpp>

#include <boost/serialization/strong_typedef.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
#include <boost/cstdint.hpp>


// special handling for primitive parameter
// required by boost serialization library
#define REMOTE_SERIALIZE_PRIMITIVE(primitive, ptr_type)											\
namespace boost																					\
{																								\
namespace serialization																			\
{																								\
BOOST_STRONG_TYPEDEF(primitive, ptr_type)														\
template<typename Archive>																		\
void serialize(Archive& ar, ptr_type& px, unsigned int const)									\
{																								\
	ar & make_nvp("ptr", px.t);																	\
}																								\
template<typename Archive>																		\
void save(Archive& ar, remote::detail::param<primitive&> const& p, unsigned int const)			\
{																								\
	ptr_type const* ptr = reinterpret_cast<ptr_type const*>(p.m_ptr);							\
	ar << make_nvp("param", ptr);																\
}																								\
template<typename Archive>																		\
void load(Archive& ar, remote::detail::param<primitive&>& p, unsigned int const)				\
{																								\
	ptr_type* ptr;																				\
	ar >> make_nvp("param", ptr);																\
	p.m_ptr = reinterpret_cast<primitive*>(ptr);												\
}																								\
template<typename Archive>																		\
void save(Archive& ar, remote::detail::param<primitive const&> const& p, unsigned int const)	\
{																								\
	ptr_type const* ptr = reinterpret_cast<ptr_type const*>(p.m_ptr);							\
	ar << make_nvp("param", ptr);																\
}																								\
template<typename Archive>																		\
void load(Archive& ar, remote::detail::param<primitive const&>& p, unsigned int const)			\
{																								\
	ptr_type* ptr;																				\
	ar >> make_nvp("param", ptr);																\
	p.m_ptr = reinterpret_cast<primitive*>(ptr);												\
}																								\
template<typename Archive>																		\
void save(Archive& ar, remote::detail::param<primitive*> const& p, unsigned int const)			\
{																								\
	ptr_type const* ptr = reinterpret_cast<ptr_type const*>(p.m_ptr);							\
	ar << make_nvp("param", ptr);																\
}																								\
template<typename Archive>																		\
void load(Archive& ar, remote::detail::param<primitive*>& p, unsigned int const)				\
{																								\
	ptr_type* ptr;																				\
	ar >> make_nvp("param", ptr);																\
	p.m_ptr = reinterpret_cast<primitive*>(ptr);												\
}																								\
template<typename Archive>																		\
void save(Archive& ar, remote::detail::param<primitive const*> const& p, unsigned int const)	\
{																								\
	ptr_type const* ptr = reinterpret_cast<ptr_type const*>(p.m_ptr);							\
	ar << make_nvp("param", ptr);																\
}																								\
template<typename Archive>																		\
void load(Archive& ar, remote::detail::param<primitive const*>& p, unsigned int const)			\
{																								\
	ptr_type* ptr;																				\
	ar >> make_nvp("param", ptr);																\
	p.m_ptr = reinterpret_cast<primitive*>(ptr);												\
}																								\
template<typename Archive>																		\
void save(Archive& ar, remote::detail::param<shared_ptr<primitive> > const& p, unsigned int const)	\
{																								\
	shared_ptr<ptr_type const> ptr = reinterpret_pointer_cast<ptr_type const>(p.m_ptr);			\
	ar << make_nvp("param", ptr);																\
}																								\
template<typename Archive>																		\
void load(Archive& ar, remote::detail::param<shared_ptr<primitive> >& p, unsigned int const)	\
{																								\
	shared_ptr<ptr_type> ptr;																	\
	ar >> make_nvp("param", ptr);																\
	p.m_ptr = reinterpret_pointer_cast<primitive>(ptr);											\
}																								\
template<typename Archive>																		\
void save(Archive& ar, remote::detail::param<shared_ptr<primitive const> > const& p, unsigned int const)	\
{																								\
	shared_ptr<ptr_type const> ptr = reinterpret_pointer_cast<ptr_type const>(p.m_ptr);			\
	ar << make_nvp("param", ptr);																\
}																								\
template<typename Archive>																		\
void load(Archive& ar, remote::detail::param<shared_ptr<primitive const> >& p, unsigned int const)	\
{																								\
	shared_ptr<ptr_type> ptr;																	\
	ar >> make_nvp("param", ptr);																\
	p.m_ptr = reinterpret_pointer_cast<primitive>(ptr);											\
}																								\
template<typename Archive>																		\
void save(Archive& ar, remote::detail::param<weak_ptr<primitive> > const& p, unsigned int const)\
{																								\
	shared_ptr<ptr_type const> ptr = reinterpret_pointer_cast<ptr_type const>(p.m_ptr.lock());	\
	ar << make_nvp("param", ptr);																\
}																								\
template<typename Archive>																		\
void load(Archive& ar, remote::detail::param<weak_ptr<primitive> >& p, unsigned int const)		\
{																								\
	shared_ptr<ptr_type> ptr;																	\
	ar >> make_nvp("param", ptr);																\
	p.m_ptr = reinterpret_pointer_cast<primitive>(ptr);											\
}																								\
template<typename Archive>																		\
void save(Archive& ar, remote::detail::param<weak_ptr<primitive const> > const& p, unsigned int const)	\
{																								\
	shared_ptr<ptr_type const> ptr = reinterpret_pointer_cast<ptr_type const>(p.m_ptr.lock());	\
	ar << make_nvp("param", ptr);																\
}																								\
template<typename Archive>																		\
void load(Archive& ar, remote::detail::param<weak_ptr<primitive const> >& p, unsigned int const)\
{																								\
	shared_ptr<ptr_type> ptr;																	\
	ar >> make_nvp("param", ptr);																\
	p.m_ptr = reinterpret_pointer_cast<primitive>(ptr);											\
}																								\
}																								\
}

REMOTE_SERIALIZE_PRIMITIVE(bool, bool_ptr)
REMOTE_SERIALIZE_PRIMITIVE(char, char_ptr)
REMOTE_SERIALIZE_PRIMITIVE(int, int_ptr)
REMOTE_SERIALIZE_PRIMITIVE(long, long_ptr)
REMOTE_SERIALIZE_PRIMITIVE(unsigned char, uchar_ptr)
REMOTE_SERIALIZE_PRIMITIVE(unsigned int, uint_ptr)
REMOTE_SERIALIZE_PRIMITIVE(unsigned long, ulong_ptr)
REMOTE_SERIALIZE_PRIMITIVE(float, float_ptr)
REMOTE_SERIALIZE_PRIMITIVE(double, double_ptr)
REMOTE_SERIALIZE_PRIMITIVE(std::string, string_ptr)

#endif
