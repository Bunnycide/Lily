#include <iostream>
#include <Log/Log.h>

#include <Window/Linux/Window.h>
#include <Device/Device.h>

#include <Resources/Buffer/VertexBuffer.h>

void drawFrame(long dealtTime);

void drawFrame(long dealtTime){

}

int main() {

    // Create Window
    Window window;
    window.createWindow(1280, 800);

    // Set draw callback
    window.pDrawCallBack = drawFrame;

    Device device(window);

    // Start main loop
    window.mainLoop();

    return 0;
}
