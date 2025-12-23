//
// Created by okker on 21/12/2025.
//

// ReSharper disable CppMemberFunctionMayBeStatic
#include "skal/file_io.h"
#include "skal/util/log.h"

#include <filesystem>
#include <fstream>



std::string skal::FileIO::ReadTextFile(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        skal::Log::Error("File {} was not found!", path);
        return {};
    }

    file.seekg(0, std::ios::end);
    const size_t size = file.tellg();
    std::string buffer(size, '\0');
    file.seekg(0);
    file.read(buffer.data(), static_cast<std::streamsize>(size));
    return buffer;
}

bool skal::FileIO::WriteTextFile(const std::string &path, const std::string &content)
{
    std::ofstream file(path);
    if (!file.is_open())
    {
        skal::Log::Error("Failed to open file for writing: {}", path);
        return false;
    }
    file << content;
    file.close();
    return true;
}

std::vector<uint8_t> skal::FileIO::ReadBinaryFile(const std::string &path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        skal::Log::Error("File {} was not found!", path);
        return {};
    }
    const std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);

    if (file.read(reinterpret_cast<std::istream::char_type *>(buffer.data()), size)) // should be safe since both char == uint8
        return buffer;
    assert(false);
    return {};
}

bool skal::FileIO::WriteBinaryFile(const std::string &path, const std::vector<uint8_t> &data)
{
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        skal::Log::Error("Failed to open file for writing: {}", path);
        return false;
    }

    file.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));

    if (!file.good()) {
        skal::Log::Error("Failed to write to file: {}", path);
        return false;
    }

    return true;
}

bool skal::FileIO::Exists(const std::string &path)
{
    std::ifstream f(path);
    const auto good = f.good();
    f.close();
    return good;
}

std::vector<std::string> skal::FileIO::ListFiles(const std::string &directory, const std::string &extension)
{
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            const auto& path = entry.path();
            if (extension.empty() || path.extension() == extension) {
                files.push_back(path.string());
            }
        }
    }
    return files;
}

