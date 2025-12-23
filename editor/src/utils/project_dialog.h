//
// Created by okker on 21/12/2025.
//

#pragma once
#include <string>

namespace editor
{
    enum class ProjectAction {
        Open,
        CreateNew,
        Cancel
    };

    ProjectAction ShowProjectStartDialog();
    std::string PromptProjectName();


} // editor