#include "FL/Fl.H"
#include "EditorWindow.h"


int main(int argc, char** argv) {

    float minW = 800;
    float minH = 600;

    EditorWindow* window = new EditorWindow(minW, minH, "test");

    window->end();
    window->size_range(minW, minH);
    window->show(argc, argv);
    return Fl::run();
}