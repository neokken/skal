//
// Created by okker on 21/12/2025.
//

#pragma once
#include <string>

#include "skal/project.h"


namespace editor
{
    class EditorContext
    {
    public:
        enum class PlayState { Editing, Playing, Paused };


        // Project management
        static bool CreateProject(const std::string &basePath, const std::string &name);

        bool OpenProject(const std::string &path);


        // engine stuff void Update(float dt);
        // engine stuff void RenderUI();
        // engine stuff void RenderGizmos();

        // Editor state
        const skal::Project& GetCurrentProject() const { return m_currentProject; }
        bool HasProjectOpen() const { return m_projectOpen; }

        void SetPlayState(PlayState state);
        PlayState GetPlayState() const { return m_playState; }

    private:
        skal::Project m_currentProject;
        std::string m_projectPath;
        bool m_projectOpen = false;

        PlayState m_playState = PlayState::Editing;

    };

} // editor
