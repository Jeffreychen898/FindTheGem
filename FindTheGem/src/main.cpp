#include <iostream>

#include "../GameDevLib/Renderer/Rendering.h"
#include "../GameDevLib/Math/Math.h"

int main(void)
{
    unsigned int window_width = 1024;
    unsigned int window_height = 768;

    Rendering::Window window(window_width, window_height, "Find The Gem");

    Rendering::Renderer renderer;
    renderer.Init(window_width, window_height);

    window.AccessRenderer(&renderer);

    while (!window.WindowShouldClose())
    {
        window.StartOfFrame();
        std::cout << "[FPS]: " << window.GetFrameRate() << "\n";

        /* end of the frame */
        window.EndOfFrame();
    }

    window.Terminate();
    return 0;
}