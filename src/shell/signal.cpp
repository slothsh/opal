#include "signal.hpp"
#include <atomic>
#include <fmt/core.h>

void SignalHandler::handle_default(int signal) {
    SignalHandler::SIGNAL.store(signal);
}

void SignalHandler::clear() {
    SignalHandler::SIGNAL.store(-1);
}
