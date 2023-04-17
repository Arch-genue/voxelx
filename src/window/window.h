#pragma once

class GLFWwindow;

class Window {
public:
    static int width;
    static int height;

    static GLFWwindow * window;
    static int init(int width, int height, const char * title);
    static void _glInit();
    static void _glClear();
    static void exit();

    static void setCursorMode(int mode);
    static void toggleFullscreen();
    static bool isShouldClose();
    static void setShouldClose(bool flag);
    static bool swapBuffers(); 
};