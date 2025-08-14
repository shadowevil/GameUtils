#pragma once
#include <type_traits>
#include <string>

namespace utils
{
    template<typename T>
    struct Thickness
    {
        static_assert(std::is_arithmetic_v<T>, "Thickness<T> requires an arithmetic type.");

        T Left;
        T Top;
        T Right;
        T Bottom;

        constexpr Thickness() noexcept : Left(0), Top(0), Right(0), Bottom(0) {}
        constexpr Thickness(T uniform) noexcept : Left(uniform), Top(uniform), Right(uniform), Bottom(uniform) {}
        constexpr Thickness(T horizontal, T vertical) noexcept : Left(horizontal), Top(vertical), Right(horizontal), Bottom(vertical) {}
        constexpr Thickness(T left, T top, T right, T bottom) noexcept : Left(left), Top(top), Right(right), Bottom(bottom) {}

        constexpr Thickness(const Thickness&) noexcept = default;
        constexpr Thickness& operator=(const Thickness&) noexcept = default;
        constexpr Thickness(Thickness&&) noexcept = default;
        constexpr Thickness& operator=(Thickness&&) noexcept = default;

        constexpr bool operator==(const Thickness& o) const { return Left == o.Left && Top == o.Top && Right == o.Right && Bottom == o.Bottom; }
        constexpr bool operator!=(const Thickness& o) const { return !(*this == o); }

        template<typename U>
        constexpr Thickness<std::common_type_t<T, U>> operator+(const Thickness<U>& other) const
        {
            using R = std::common_type_t<T, U>;
            return { static_cast<R>(Left) + other.Left,
                     static_cast<R>(Top) + other.Top,
                     static_cast<R>(Right) + other.Right,
                     static_cast<R>(Bottom) + other.Bottom };
        }

        template<typename U>
        constexpr Thickness<std::common_type_t<T, U>> operator-(const Thickness<U>& other) const
        {
            using R = std::common_type_t<T, U>;
            return { static_cast<R>(Left) - other.Left,
                     static_cast<R>(Top) - other.Top,
                     static_cast<R>(Right) - other.Right,
                     static_cast<R>(Bottom) - other.Bottom };
        }

        template<typename U>
        constexpr Thickness<std::common_type_t<T, U>> operator*(U scalar) const
        {
            using R = std::common_type_t<T, U>;
            return { static_cast<R>(Left) * scalar,
                     static_cast<R>(Top) * scalar,
                     static_cast<R>(Right) * scalar,
                     static_cast<R>(Bottom) * scalar };
        }

        template<typename U>
        constexpr Thickness<std::common_type_t<T, U>> operator/(U scalar) const
        {
            using R = std::common_type_t<T, U>;
            return { static_cast<R>(Left) / scalar,
                     static_cast<R>(Top) / scalar,
                     static_cast<R>(Right) / scalar,
                     static_cast<R>(Bottom) / scalar };
        }

        template<typename U>
        constexpr Thickness& operator+=(const Thickness<U>& other)
        {
            Left = static_cast<T>(Left + other.Left);
            Top = static_cast<T>(Top + other.Top);
            Right = static_cast<T>(Right + other.Right);
            Bottom = static_cast<T>(Bottom + other.Bottom);
            return *this;
        }

        template<typename U>
        constexpr Thickness& operator-=(const Thickness<U>& other)
        {
            Left = static_cast<T>(Left - other.Left);
            Top = static_cast<T>(Top - other.Top);
            Right = static_cast<T>(Right - other.Right);
            Bottom = static_cast<T>(Bottom - other.Bottom);
            return *this;
        }

        template<typename U>
        constexpr Thickness& operator*=(U scalar)
        {
            Left = static_cast<T>(Left * scalar);
            Top = static_cast<T>(Top * scalar);
            Right = static_cast<T>(Right * scalar);
            Bottom = static_cast<T>(Bottom * scalar);
            return *this;
        }

        template<typename U>
        constexpr Thickness& operator/=(U scalar)
        {
            Left = static_cast<T>(Left / scalar);
            Top = static_cast<T>(Top / scalar);
            Right = static_cast<T>(Right / scalar);
            Bottom = static_cast<T>(Bottom / scalar);
            return *this;
        }

        template<typename U>
        constexpr explicit operator Thickness<U>() const
        {
            return { static_cast<U>(Left), static_cast<U>(Top), static_cast<U>(Right), static_cast<U>(Bottom) };
        }

        T Horizontal() const { return Left + Right; }
        T Vertical()   const { return Top + Bottom; }

        std::string ToString() const
        {
            return std::to_string(Left) + ", " + std::to_string(Top) + ", " +
                std::to_string(Right) + ", " + std::to_string(Bottom);
        }
    };

    // Aliases
    template<typename T> using Padding = Thickness<T>;
    template<typename T> using Margin = Thickness<T>;
}