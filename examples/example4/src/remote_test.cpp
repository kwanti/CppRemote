
// note: include "rmt.hpp" again here before defining
// REMOTE_IMPLEMENT_CLASS because we don't want to generate
// implementation for rmt class in this translation unit.
#include "rmt.hpp"

#include <remote/make_tcp_binding.hpp>
#define REMOTE_IMPLEMENT_CLASS remote_test
#include "remote_test.hpp"
