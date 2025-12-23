//
// Created by okker on 21/12/2025.
//

#pragma once
#include <cstdio>
#include <random>
#include <string>
#include <fmt/format.h>

namespace skal
{
    template<typename Tag>
    struct TypedUUID
    {
        uint64_t high;
        uint64_t low;

        TypedUUID() : high(0), low(0)
        {
        }

        TypedUUID(const uint64_t h, const uint64_t l) : high(h), low(l)
        {
        }

        bool operator==(const TypedUUID &other) const { return high == other.high && low == other.low; }

        bool operator!=(const TypedUUID &other) const { return !(*this == other); }

        [[nodiscard]] bool is_null() const { return high == 0 && low == 0; }

        [[nodiscard]] std::string to_string() const
        {
            return fmt::format("{:08x}-{:04x}-{:04x}-{:04x}-{:012x}",
                               static_cast<uint32_t>(high >> 32),
                               static_cast<uint16_t>((high >> 16) & 0xFFFF),
                               static_cast<uint16_t>(high & 0xFFFF),
                               static_cast<uint16_t>((low >> 48) & 0xFFFF),
                               low & 0xFFFFFFFFFFFFULL);
        }

        static TypedUUID from_string(const std::string &str)
        {
            if (str.size() != 36)
            {
                throw std::runtime_error("Invalid UUID format");
            }

            auto parse_hex = [&](size_t pos, size_t len) -> uint64_t
            {
                return std::stoull(str.substr(pos, len), nullptr, 16);
            };

            uint64_t high = (parse_hex(0, 8) << 32) |
                            (parse_hex(9, 4) << 16) |
                            parse_hex(14, 4);

            uint64_t low = (parse_hex(19, 4) << 48) |
                           parse_hex(24, 12);

            return TypedUUID(high, low);
        }

        static TypedUUID null()
        {
            return {};
        }
    };


    class UUIDGenerator
    {
    public:
        template<typename UUIDType>
        static UUIDType generate()
        {
            thread_local std::mt19937_64 gen(std::random_device{}());
            thread_local std::uniform_int_distribution<uint64_t> dis;

            auto high = dis(gen);
            auto low = dis(gen);

            // Set UUID v4 bits
            // uuid are weird.
            high = (high & 0xFFFFFFFFFFFF0FFFULL) | 0x0000000000004000ULL;
            low = (low & 0x3FFFFFFFFFFFFFFFULL) | 0x8000000000000000ULL;

            return UUIDType{high, low};
        }
    };
}


template<typename Tag>
struct std::hash<skal::TypedUUID<Tag> > // NOLINT(*-dcl58-cpp)
{
    size_t operator()(const skal::TypedUUID<Tag> &uuid) const
    {
        return std::hash<uint64_t>{}(uuid.high) ^ std::hash<uint64_t>{}(uuid.low);
    }
};
