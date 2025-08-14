#pragma once
#include <type_traits>
namespace raylib {
#include "raylib/raylib.h"
#include "raylib/raymath.h"
}
#include <string>

namespace utils
{
    template<typename T>
    struct Size
    {
        static_assert(std::is_arithmetic_v<T>, "Size<T> requires an arithmetic type.");

        T Width;
        T Height;

        constexpr Size() : Width(0), Height(0) {}
        constexpr Size(T width, T height) : Width(width), Height(height) {}

        constexpr bool operator==(const Size& other) const { return Width == other.Width && Height == other.Height; }
        constexpr bool operator!=(const Size& other) const { return !(*this == other); }
        constexpr bool operator<(const Size& other)  const { return (Width < other.Width) || (Width == other.Width && Height < other.Height); }
        constexpr bool operator>(const Size& other)  const { return (Width > other.Width) || (Width == other.Width && Height > other.Height); }
        constexpr bool operator<=(const Size& other) const { return !(*this > other); }
        constexpr bool operator>=(const Size& other) const { return !(*this < other); }

        template<typename U>
        constexpr Size<std::common_type_t<T, U>> operator+(const Size<U>& other) const
        {
            using R = std::common_type_t<T, U>;
            return { static_cast<R>(Width) + other.Width, static_cast<R>(Height) + other.Height };
        }

        template<typename U>
        constexpr Size<std::common_type_t<T, U>> operator-(const Size<U>& other) const
        {
            using R = std::common_type_t<T, U>;
            return { static_cast<R>(Width) - other.Width, static_cast<R>(Height) - other.Height };
        }

        template<typename U>
        constexpr Size<std::common_type_t<T, U>> operator*(U scalar) const
        {
            using R = std::common_type_t<T, U>;
            return { static_cast<R>(Width) * scalar, static_cast<R>(Height) * scalar };
        }

        template<typename U>
        constexpr Size<std::common_type_t<T, U>> operator/(U scalar) const
        {
            using R = std::common_type_t<T, U>;
            return { static_cast<R>(Width) / scalar, static_cast<R>(Height) / scalar };
        }

        template<typename U>
        constexpr Size& operator+=(const Size<U>& other)
        {
            Width = static_cast<T>(Width + other.Width);
            Height = static_cast<T>(Height + other.Height);
            return *this;
        }

        template<typename U>
        constexpr Size& operator-=(const Size<U>& other)
        {
            Width = static_cast<T>(Width - other.Width);
            Height = static_cast<T>(Height - other.Height);
            return *this;
        }

        template<typename U>
        constexpr Size& operator*=(U scalar)
        {
            Width = static_cast<T>(Width * scalar);
            Height = static_cast<T>(Height * scalar);
            return *this;
        }

        template<typename U>
        constexpr Size& operator/=(U scalar)
        {
            Width = static_cast<T>(Width / scalar);
            Height = static_cast<T>(Height / scalar);
            return *this;
        }

        template<typename U>
        constexpr explicit operator Size<U>() const { return Size<U>(static_cast<U>(Width), static_cast<U>(Height)); }

        std::string ToString() const { return std::to_string(Width) + ", " + std::to_string(Height); }

        // raylib interop
        constexpr explicit operator raylib::Vector2() const { return { static_cast<float>(Width), static_cast<float>(Height) }; }
    };
}