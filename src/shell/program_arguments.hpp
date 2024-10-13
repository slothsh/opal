#ifndef OPAL_PROGRAM_ARGUMENTS_HPP
#define OPAL_PROGRAM_ARGUMENTS_HPP

#include <string_view>
#include <vector>
#include <fmt/core.h>

struct ProgramArguments {
    static ProgramArguments parse(int argc, char** argv);
    static void help();

    bool show_help = false;
    bool show_version = false;
    std::vector<std::string_view> unknown;
};

#endif // @END of OPAL_PROGRAM_ARGUMENTS_HPP
