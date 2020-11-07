#include "visualizer/minecraft.h"

using minecraft::visualizer::MinecraftApp;

void prepareSettings(MinecraftApp::Settings* settings) {
  settings->setResizable(false);
}

// This line is a macro that expands into an "int main()" function.
CINDER_APP(MinecraftApp, ci::app::RendererGl, prepareSettings);
