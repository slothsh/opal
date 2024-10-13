#include <fmt/core.h>
#include <wayland-egl-core.h>
#include <wayland-client-protocol.h>

#include "renderer.hpp"

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::init([[maybe_unused]] wl_display* display) {}

void Renderer::shutdown() {}

void* Renderer::display() {
    return nullptr;
}

void* Renderer::surface() {
    return nullptr;
}

void Renderer::setSurface([[maybe_unused]] void* surface) {}

void* Renderer::config() {
    return nullptr;
}


void* Renderer::getPlatformDisplay() {
    return nullptr;
}

void* Renderer::createPlatformWindowSurface([[maybe_unused]] wl_egl_window* window) {
    return nullptr;
}

void* Renderer::context() {
    return nullptr;
}
