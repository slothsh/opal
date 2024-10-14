#include <memory>

#include <fmt/core.h>
#include "taskbar.hpp"
#include "compositor.hpp"
#include "renderer.hpp"

int main() {
    RENDERER = std::make_unique<Renderer>();
    COMPOSITOR = std::make_unique<Compositor>();
    COMPOSITOR->init();

    COMPOSITOR->draw();

    while (COMPOSITOR->online()) {
        // TODO:
    }

    RENDERER->shutdown();
    COMPOSITOR->shutdown();
    return SUCCESS;
}
