#include <chrono>
#include <csignal>
#include <thread>
#include <chrono>

#include <fmt/core.h>

#include "shell.hpp"
#include "signal.hpp"
#include "program_arguments.hpp"

namespace stdc = std::chrono;

Result taskbar_main() {
    bool should_exit = false;

    while (!should_exit) {}

    return SUCCESS;
}

Result shell_main([[maybe_unused]] ProgramArguments const& arguments) {
    bool should_exit = false;

    while (!should_exit) {
        auto const signal = SignalHandler::SIGNAL.load();

        if (signal != -1) {
            switch (signal) {
                case SIGHUP:
                case SIGINT:
                case SIGQUIT: {
                    fmt::print("exiting...\n");
                    should_exit = true;
                } break;

                default: {} break;
            }
        }

        if (!should_exit) std::this_thread::sleep_for(stdc::milliseconds(10));
    }

    return SUCCESS;
}

int main(int argc, char** argv) {
    auto const arguments = ProgramArguments::parse(argc, argv);
    std::signal(SIGINT, SignalHandler::handle_default);
    std::signal(SIGHUP, SignalHandler::handle_default);
    std::signal(SIGQUIT, SignalHandler::handle_default);
    shell_main(arguments);
    return SUCCESS;
}
