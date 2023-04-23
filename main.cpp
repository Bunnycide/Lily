#include "MyApp.h"

int main() {

    // Create Window
    Window window;
    window.createWindow(1280, 800);

    Device device(window);

    MyApp app(device, window);
    app.run();

    return 0;
}
