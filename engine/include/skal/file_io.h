//
// Created by okker on 20/12/2025.
//

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace skal
{
    class FileIO {
    public:
        std::string ReadTextFile(const std::string& path);
        bool WriteTextFile(const std::string& path, const std::string& content);

        std::vector<uint8_t> ReadBinaryFile(const std::string& path);
        bool WriteBinaryFile(const std::string& path, const std::vector<uint8_t>& data);

        bool Exists(const std::string& path);
        std::vector<std::string> ListFiles(const std::string& directory, const std::string& extension);

        // Platform-specific user data location
        std::string GetUserDataDirectory();
    };
}
