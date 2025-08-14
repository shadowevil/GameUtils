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
    struct Point
    {
        static_assert(std::is_arithmetic_v<T>, "Point<T> requires an arithmetic type.");

        T X;
        T Y;

        constexpr Point() : X(0), Y(0) {}
        constexpr Point(T x, T y) : X(x), Y(y) {}

        template<typename U>
        constexpr Point(const Point<U>& other) : X(static_cast<T>(other.X)), Y(static_cast<T>(other.Y)) {}

        constexpr bool operator==(const Point& other) const { return X == other.X && Y == other.Y; }
        constexpr bool operator!=(const Point& other) const { return !(*this == other); }
        constexpr bool operator<(const Point& other)  const { return (X < other.X) || (X == other.X && Y < other.Y); }
        constexpr bool operator>(const Point& other)  const { return (X > other.X) || (X == other.X && Y > other.Y); }
        constexpr bool operator<=(const Point& other) const { return !(*this > other); }
        constexpr bool operator>=(const Point& other) const { return !(*this < other); }

        template<typename U>
        constexpr Point<std::common_type_t<T, U>> operator+(const Point<U>& other) const
        {
            using R = std::common_type_t<T, U>;
            return { static_cast<R>(X) + other.X, static_cast<R>(Y) + other.Y };
        }

        template<typename U>
        constexpr Point<std::common_type_t<T, U>> operator-(const Point<U>& other) const
        {
            using R = std::common_type_t<T, U>;
            return { static_cast<R>(X) - other.X, static_cast<R>(Y) - other.Y };
        }

        template<typename U>
        constexpr Point<std::common_type_t<T, U>> operator*(U scalar) const
        {
            using R = std::common_type_t<T, U>;
            return { static_cast<R>(X) * scalar, static_cast<R>(Y) * scalar };
        }

        template<typename U>
        constexpr Point<std::common_type_t<T, U>> operator/(U scalar) const
        {
            using R = std::common_type_t<T, U>;
            return { static_cast<R>(X) / scalar, static_cast<R>(Y) / scalar };
        }

        template<typename U>
        constexpr Point& operator+=(const Point<U>& other)
        {
            X = static_cast<T>(X + other.X);
            Y = static_cast<T>(Y + other.Y);
            return *this;
        }

        template<typename U>
        constexpr Point& operator-=(const Point<U>& other)
        {
            X = static_cast<T>(X - other.X);
            Y = static_cast<T>(Y - other.Y);
            return *this;
        }

        template<typename U>
        constexpr Point& operator*=(U scalar)
        {
            X = static_cast<T>(X * scalar);
            Y = static_cast<T>(Y * scalar);
            return *this;
        }

        template<typename U>
        constexpr Point& operator/=(U scalar)
        {
            X = static_cast<T>(X / scalar);
            Y = static_cast<T>(Y / scalar);
            return *this;
        }

        template<typename U>
        constexpr explicit operator Point<U>() const { return Point<U>(static_cast<U>(X), static_cast<U>(Y)); }

        std::string ToString() const { return std::to_string(X) + ", " + std::to_string(Y); }

        // raylib interop
        constexpr explicit operator raylib::Vector2() const { return { static_cast<float>(X), static_cast<float>(Y) }; }
    };
}