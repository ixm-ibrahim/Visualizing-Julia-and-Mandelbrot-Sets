
#include "Window.h"

int main()
{
    {
        Window window = Window(1000,1000);
        
        window.RenderLoop();
    }
    
    //_CrtDumpMemoryLeaks();
    //std::cin.get();

    return 0;
}
