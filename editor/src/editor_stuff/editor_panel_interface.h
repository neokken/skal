//
// Created by okker on 10/01/2026.
//

#pragma once

namespace editor
{
    class IEditorPanel
    {
    public:
        virtual ~IEditorPanel() = default;

        virtual void Render() = 0;
        [[nodiscard]] virtual const char *GetName() const = 0;
        [[nodiscard]] virtual bool IsOpen() const = 0;
        virtual void SetOpen(bool open) = 0;
    };


} // editor
