//
// Created by okker on 22/12/2025.
//

#pragma once
#include <string>

namespace skal
{
    struct RootTag
    {
        bool _;
    };

    struct MarkedForDeleteTag
    {
       bool _;
    };

    struct NameComponent
    {
        std::string name{"empty"};
    };



}