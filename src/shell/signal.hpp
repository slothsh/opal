#ifndef OPAL_SIGNAL_HPP
#define OPAL_SIGNAL_HPP

#include <atomic>

struct SignalHandler {
    static void handle_default(int signal);
    static void clear();
    static inline std::atomic<int> SIGNAL = -1;
};

#endif // @END of OPAL_SIGNAL_HPP
