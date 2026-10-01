// main.cpp
// Entry point: creates the Application, initializes the window/ImGui
// context, runs the main loop, and shuts everything down cleanly.
#include "gui/Application.h"

#include <cstdlib>
#include <iostream>

int main() {
    rps::gui::Application app;

    if (!app.initialize()) {
        std::cerr << "Failed to initialize the application." << std::endl;
        return EXIT_FAILURE;
    }

    app.run();
    app.shutdown();

    return EXIT_SUCCESS;
}
