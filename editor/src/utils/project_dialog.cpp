//
// Created by okker on 21/12/2025.
//

#include "project_dialog.h"

#include <iostream>


namespace editor
{
    ProjectAction ShowProjectStartDialog()
    {
        // Temporary: console choice
        std::cout << "Skal Editor\n";
        std::cout << "1. Open Project\n";
        std::cout << "2. Create New Project\n";
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;

        switch(choice) {
            case 1: return ProjectAction::Open;
            case 2: return ProjectAction::CreateNew;
            default: return ProjectAction::Cancel;
        }
    }

    std::string PromptProjectName()
    {
        // MessageBox doesn't support text input
        // Fall back to console or use a proper dialog
        std::cout << "Project name: ";
        std::string name;
        std::cin >> name;
        return name;
    }
} // editor