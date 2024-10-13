#ifndef OPAL_MACROS_HPP
#define OPAL_MACROS_HPP

#include <cassert>
#include <exception>

// Logging
#define OPAL_TODO(msg) (assert(0 && msg))
#define OPAL_ASSERT(assertion) (assert(assertion))
#define OPAL_THROW(msg) throw std::runtime_error((msg))


#endif // @END of OPAL_MACROS_HPP
