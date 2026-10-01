# Assets Folder

The current UI is drawn entirely with Dear ImGui's procedural drawing
API (buttons, tables, progress bars, colored text, etc.), so no image
or font assets are required for the application to run.

This folder is kept as a place to drop custom assets if you want to
extend the app, for example:

- Custom `.ttf` fonts - load them in `Application::initialize()`
  (see `src/gui/Application.cpp`) via `io.Fonts->AddFontFromFileTTF(...)`.
- Icon/texture images for move buttons (Rock/Paper/Scissors icons) -
  load with `stb_image` (bundled inside Dear ImGui's dependencies) and
  upload as an OpenGL texture, then draw with `ImGui::Image(...)`.
- A window/taskbar icon - set via `glfwSetWindowIcon()` in
  `Application::initialize()`.

The build copies this folder next to the compiled executable
automatically, so any files you add here will be available at runtime
via a relative `assets/...` path.
