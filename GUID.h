#pragma once
#include <array>
#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <stdexcept>

namespace utils
{
    class GUID
    {
    public:
        static GUID Generate()
        {
            GUID guid;
            FillSecureRandom(reinterpret_cast<uint8_t*>(&guid), sizeof(GUID));
            // Set version (4)
            guid.Data3 = (guid.Data3 & 0x0FFF) | 0x4000;
            // Set variant (10xxxxxx)
            guid.Data4[0] = (guid.Data4[0] & 0x3F) | 0x80;
            return guid;
        }

        bool operator==(const GUID& other) const
        {
            return Data1 == other.Data1 &&
                Data2 == other.Data2 &&
                Data3 == other.Data3 &&
                std::equal(Data4.begin(), Data4.end(), other.Data4.begin());
        }

        std::string ToString() const
        {
            std::ostringstream ss;
            ss << std::hex << std::setfill('0') << std::nouppercase
                << std::setw(8) << Data1 << "-"
                << std::setw(4) << Data2 << "-"
                << std::setw(4) << Data3 << "-"
                << std::setw(2) << static_cast<int>(Data4[0])
                << std::setw(2) << static_cast<int>(Data4[1]) << "-"
                << std::setw(2) << static_cast<int>(Data4[2])
                << std::setw(2) << static_cast<int>(Data4[3])
                << std::setw(2) << static_cast<int>(Data4[4])
                << std::setw(2) << static_cast<int>(Data4[5])
                << std::setw(2) << static_cast<int>(Data4[6])
                << std::setw(2) << static_cast<int>(Data4[7]);
            return ss.str();
        }

        uint32_t ToUInt32() const
        {
            constexpr uint32_t FNV_OFFSET = 2166136261u;
            constexpr uint32_t FNV_PRIME = 16777619u;

            uint32_t hash = FNV_OFFSET;
            const uint8_t* bytes = reinterpret_cast<const uint8_t*>(this);
            for (size_t i = 0; i < sizeof(*this); ++i)
            {
                hash ^= bytes[i];
                hash *= FNV_PRIME;
            }
            return hash;
        }

        uint64_t ToUInt64() const
        {
            constexpr uint64_t FNV_OFFSET = 1469598103934665603ull;
            constexpr uint64_t FNV_PRIME = 1099511628211ull;

            uint64_t hash = FNV_OFFSET;
            const uint8_t* bytes = reinterpret_cast<const uint8_t*>(this);
            for (size_t i = 0; i < sizeof(*this); ++i)
            {
                hash ^= bytes[i];
                hash *= FNV_PRIME;
            }
            return hash;
        }

    private:
        GUID() = default;

        static void FillSecureRandom(uint8_t* buffer, size_t size)
        {
            std::random_device rd;
            for (size_t i = 0; i < size; ++i)
                buffer[i] = static_cast<uint8_t>(rd());
        }

        uint32_t Data1{};
        uint16_t Data2{};
        uint16_t Data3{};
        std::array<uint8_t, 8> Data4{};
    };
}

namespace std
{
    template <>
    struct hash<utils::GUID>
    {
        size_t operator()(const utils::GUID& guid) const noexcept
        {
            return std::hash<std::string>{}(guid.ToString());
        }
    };
}