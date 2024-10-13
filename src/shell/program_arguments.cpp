#include <ranges>
#include <span>
#include "macros.hpp"
#include "program_arguments.hpp"

auto ProgramArguments::parse(int argc, char** argv) -> ProgramArguments {
        ProgramArguments parsed_arguments{};

        auto const arguments = std::span(argv, argc)
            | std::views::transform([](auto arg) { return std::string_view{arg}; })
            | std::views::drop(1);

        for (auto const& arg : arguments) {
            if (arg == "--help" || arg == "-h") {
                parsed_arguments.show_help = true;
            } else if (arg == "--version" || arg == "-v") {
                parsed_arguments.show_version = true;
            } else {
                parsed_arguments.unknown.emplace_back(arg);
            }
        }

        return parsed_arguments;
}

auto ProgramArguments::help() -> void {
    OPAL_TODO("not implemented");
}
