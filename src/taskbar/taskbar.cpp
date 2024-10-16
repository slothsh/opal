#include <cstdlib>
#include <memory>

#include <fmt/core.h>
#include "compositor.hpp"
#include "user_config.hpp"

int main() {
    COMPOSITOR = std::make_unique<Compositor>();
    COMPOSITOR->init();
    COMPOSITOR->start();
    COMPOSITOR->shutdown();
    return EXIT_SUCCESS;
}
