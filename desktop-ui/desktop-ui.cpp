#include "desktop-ui.hpp"

namespace ruby {
  Video video;
  Audio audio;
  Input input;
}

auto locate(const string& name) -> string {
  // First, check the application directory
  // This allows ares to function in 'portable' mode
  string location = {Path::program(), name};
  if(inode::exists(location)) return location;

  // On macOS, also check the AppBundle Resource path
  #if defined(PLATFORM_MACOS)
    location = {Path::program(), "../Resources/", name};
    if(inode::exists(location)) return location;
  #endif

  // Check the userData directory, this is the default
  // on non-windows platforms for any resouces that did not
  // ship with the executable.
  // On Windows, this allows settings from to be carried over
  // from previous versions (pre-portable)
  location = {Path::userData(), "ares/", name};
  if(inode::exists(location)) return location;

  // On non-windows platforms, after exhausting other options,
  // default to userData, on Windows, default to program dir
  // this ensures Portable mode is the default on Windows platforms.
  #if !defined(PLATFORM_WINDOWS)
    directory::create({Path::userData(), "ares/"});
    return {Path::userData(), "ares/", name};
  #else 
    return {Path::program(), name};
  #endif
}

#include <nall/main.hpp>
auto nall::main(Arguments arguments) -> void {
  Application::setName("ares");
  Application::setScreenSaver(false);

  mia::setHomeLocation([]() -> string {
    if(auto location = settings.paths.home) return location;
    return locate("Systems/");
  });

  mia::setSaveLocation([]() -> string {
    return settings.paths.saves;
  });

  for(auto argument : arguments) {
    if(argument == "--fullscreen") {
      program.startFullScreen = true;
    } else if(file::exists(argument)) {
      program.startGameLoad = argument;
    }
  }

  inputManager.create();
  Emulator::construct();
  settings.load();
  Instances::presentation.construct();
  Instances::settingsWindow.construct();
  Instances::toolsWindow.construct();

  program.create();
  presentation.loadEmulators();
  Application::onMain({&Program::main, &program});
  Application::run();

  settings.save();

  Instances::presentation.destruct();
  Instances::settingsWindow.destruct();
  Instances::toolsWindow.destruct();
}
