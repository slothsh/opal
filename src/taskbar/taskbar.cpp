#include <memory>

#include <fmt/core.h>
#include "taskbar.hpp"
#include "compositor.hpp"
#include "renderer_egl.hpp"

int main() {
    RENDERER = std::make_unique<EglRenderer>();
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
