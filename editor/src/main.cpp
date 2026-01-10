//
// Created by okker on 20/12/2025.
//

#include <skal/engine.h>
#include "editor.h"


int main(const int argc, char **argv)
{
    // Command line: editor.exe "C:/Projects/MyGame"
    // // full path, or it's appended to the working directory
    const std::string projectPath = argv[1];

    editor::Editor editor;
    editor.Run(projectPath);


    return 0;
}
