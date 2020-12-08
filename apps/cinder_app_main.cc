#include "game_engine.h"

using minecraft::MinecraftApp;

void prepareSettings(MinecraftApp::Settings* settings) {
  settings->setResizable(false);
  srand(time(0));  // this seeds the `rand()` function, which is used in
                   // GameEngine, so it's useful to do this up-front before
                   // GameEngine is even initialized
}

// This line is a macro that expands into an "int main()" function.
CINDER_APP(MinecraftApp, ci::app::RendererGl, prepareSettings);
