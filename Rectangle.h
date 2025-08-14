#pragma once
#include <type_traits>
namespace raylib {
#include "raylib/raylib.h"
#include "raylib/raymath.h"
}
#include <string>
#include "Size.h"
#include "Point.h"

namespace utils
{
    template<typename T>
    struct Rectangle
    {
        // data
        Point<T> Position;
        Size<T>  Size;

        // refs (aliases)
        T& X;
        T& Y;
        T& Width;
        T& Height;

    private:
        struct EmptyTag {};
        bool m_empty;

    public:
        // factories
        static constexpr Rectangle<T> Zero() { return { 0, 0, 0, 0 }; }
        static constexpr Rectangle<T> FromSize(const utils::Size<T>& size) { return { 0, 0, size.Width, size.Height }; }
        static constexpr Rectangle<T> Empty() { return Rectangle<T>(EmptyTag{}); }
        constexpr bool IsEmpty() const noexcept { return m_empty; }

        // ctors
        constexpr Rectangle() noexcept
            : Position(), Size(), X(Position.X), Y(Position.Y), Width(Size.Width), Height(Size.Height), m_empty(false) {
        }

        constexpr Rectangle(T x, T y) noexcept
            : Position(x, y), Size(0, 0), X(Position.X), Y(Position.Y), Width(Size.Width), Height(Size.Height), m_empty(false) {
        }

        constexpr Rectangle(T x, T y, T width, T height) noexcept
            : Position(x, y), Size(width, height), X(Position.X), Y(Position.Y), Width(Size.Width), Height(Size.Height), m_empty(false) {
        }

        constexpr Rectangle(const Point<T>& pos, const utils::Size<T>& size) noexcept
            : Position(pos), Size(size), X(Position.X), Y(Position.Y), Width(Size.Width), Height(Size.Height), m_empty(false) {
        }

        constexpr Rectangle(const Rectangle& other)
            : Position(other.Position), Size(other.Size),
            X(Position.X), Y(Position.Y), Width(Size.Width), Height(Size.Height),
            m_empty(other.m_empty) {
        }

        template<typename U>
        constexpr Rectangle(const Rectangle<U>& other) noexcept
            : Position(static_cast<T>(other.Position.X), static_cast<T>(other.Position.Y)),
            Size(static_cast<T>(other.Size.Width), static_cast<T>(other.Size.Height)),
            X(Position.X), Y(Position.Y), Width(Size.Width), Height(Size.Height),
            m_empty(other.IsEmpty()) {
        }

        // assignment
        constexpr Rectangle& operator=(const Rectangle& other)
        {
            if (this != &other)
            {
                Position = other.Position;
                Size = other.Size;
                m_empty = other.m_empty;
                // refs already alias Position/Size
            }
            return *this;
        }

        constexpr Rectangle(Rectangle&& other) noexcept
            : Position(std::move(other.Position)), Size(std::move(other.Size)),
            X(Position.X), Y(Position.Y), Width(Size.Width), Height(Size.Height),
            m_empty(other.m_empty) {
        }

        constexpr Rectangle& operator=(Rectangle&& other) noexcept
        {
            if (this != &other)
            {
                Position = std::move(other.Position);
                Size = std::move(other.Size);
                m_empty = other.m_empty;
            }
            return *this;
        }

        constexpr Rectangle& operator=(const raylib::Rectangle& other)
        {
            Position.X = static_cast<T>(other.x);
            Position.Y = static_cast<T>(other.y);
            Size.Width = static_cast<T>(other.width);
            Size.Height = static_cast<T>(other.height);
            m_empty = false;
            return *this;
        }

        // comparisons
        constexpr bool operator==(const Rectangle& other) const { return Position == other.Position && Size == other.Size && m_empty == other.m_empty; }
        constexpr bool operator!=(const Rectangle& other) const { return !(*this == other); }

        // arithmetic
        constexpr Rectangle operator+(const Rectangle& other) const { Rectangle r{ Position + other.Position, Size + other.Size }; r.m_empty = m_empty && other.m_empty; return r; }
        constexpr Rectangle operator-(const Rectangle& other) const { Rectangle r{ Position - other.Position, Size - other.Size }; r.m_empty = m_empty && other.m_empty; return r; }
        constexpr Rectangle operator*(T scalar)            const { Rectangle r{ Position * scalar, Size * scalar }; r.m_empty = m_empty; return r; }
        constexpr Rectangle operator/(T scalar)            const { Rectangle r{ Position / scalar, Size / scalar }; r.m_empty = m_empty; return r; }

        constexpr Rectangle& operator+=(const Rectangle& other) { Position += other.Position; Size += other.Size; m_empty = m_empty && other.m_empty; return *this; }
        constexpr Rectangle& operator-=(const Rectangle& other) { Position -= other.Position; Size -= other.Size; m_empty = m_empty && other.m_empty; return *this; }
        constexpr Rectangle& operator*=(T scalar) { Position *= scalar; Size *= scalar; return *this; }
        constexpr Rectangle& operator/=(T scalar) { Position /= scalar; Size /= scalar; return *this; }

        template<typename U>
        constexpr Rectangle<std::common_type_t<T, U>> operator+(const Thickness<U>& t) const
        {
            using R = std::common_type_t<T, U>;
            Rectangle<R> r{
                { static_cast<R>(Position.X) - static_cast<R>(t.Left),
                  static_cast<R>(Position.Y) - static_cast<R>(t.Top) },
                { static_cast<R>(Size.Width) + static_cast<R>(t.Left) + static_cast<R>(t.Right),
                  static_cast<R>(Size.Height) + static_cast<R>(t.Top) + static_cast<R>(t.Bottom) }
            };
            return r;
        }

        template<typename U>
        constexpr Rectangle<std::common_type_t<T, U>> operator-(const Thickness<U>& t) const
        {
            using R = std::common_type_t<T, U>;
            Rectangle<R> r{
                { static_cast<R>(Position.X) + static_cast<R>(t.Left),
                  static_cast<R>(Position.Y) + static_cast<R>(t.Top) },
                { static_cast<R>(Size.Width) - (static_cast<R>(t.Left) + static_cast<R>(t.Right)),
                  static_cast<R>(Size.Height) - (static_cast<R>(t.Top) + static_cast<R>(t.Bottom)) }
            };
            return r;
        }

        template<typename U>
        constexpr Rectangle& operator+=(const Thickness<U>& t)
        {
            using R = std::common_type_t<T, U>;
            Position.X = static_cast<T>(static_cast<R>(Position.X) - t.Left);
            Position.Y = static_cast<T>(static_cast<R>(Position.Y) - t.Top);
            Size.Width = static_cast<T>(static_cast<R>(Size.Width) + t.Left + t.Right);
            Size.Height = static_cast<T>(static_cast<R>(Size.Height) + t.Top + t.Bottom);
            return *this;
        }

        template<typename U>
        constexpr Rectangle& operator-=(const Thickness<U>& t)
        {
            using R = std::common_type_t<T, U>;
            Position.X = static_cast<T>(static_cast<R>(Position.X) + t.Left);
            Position.Y = static_cast<T>(static_cast<R>(Position.Y) + t.Top);
            Size.Width = static_cast<T>(static_cast<R>(Size.Width) - (t.Left + t.Right));
            Size.Height = static_cast<T>(static_cast<R>(Size.Height) - (t.Top + t.Bottom));
            return *this;
        }

        template<typename U>
        constexpr Rectangle<std::common_type_t<T, U>> operator*(const Thickness<U>& t) const
        {
            using R = std::common_type_t<T, U>;
            Rectangle<R> r{
                { static_cast<R>(Position.X) * static_cast<R>(t.Left),
                  static_cast<R>(Position.Y) * static_cast<R>(t.Top) },
                { static_cast<R>(Size.Width) * static_cast<R>(t.Right),
                  static_cast<R>(Size.Height) * static_cast<R>(t.Bottom) }
            };
            return r;
        }

        template<typename U>
        constexpr Rectangle<std::common_type_t<T, U>> operator/(const Thickness<U>& t) const
        {
            using R = std::common_type_t<T, U>;
            Rectangle<R> r{
                { static_cast<R>(Position.X) / static_cast<R>(t.Left),
                  static_cast<R>(Position.Y) / static_cast<R>(t.Top) },
                { static_cast<R>(Size.Width) / static_cast<R>(t.Right),
                  static_cast<R>(Size.Height) / static_cast<R>(t.Bottom) }
            };
            return r;
        }

        template<typename U>
        constexpr Rectangle& operator*=(const Thickness<U>& t)
        {
            using R = std::common_type_t<T, U>;
            Position.X = static_cast<T>(static_cast<R>(Position.X) * static_cast<R>(t.Left));
            Position.Y = static_cast<T>(static_cast<R>(Position.Y) * static_cast<R>(t.Top));
            Size.Width = static_cast<T>(static_cast<R>(Size.Width) * static_cast<R>(t.Right));
            Size.Height = static_cast<T>(static_cast<R>(Size.Height) * static_cast<R>(t.Bottom));
            return *this;
        }

        template<typename U>
        constexpr Rectangle& operator/=(const Thickness<U>& t)
        {
            using R = std::common_type_t<T, U>;
            Position.X = static_cast<T>(static_cast<R>(Position.X) / static_cast<R>(t.Left));
            Position.Y = static_cast<T>(static_cast<R>(Position.Y) / static_cast<R>(t.Top));
            Size.Width = static_cast<T>(static_cast<R>(Size.Width) / static_cast<R>(t.Right));
            Size.Height = static_cast<T>(static_cast<R>(Size.Height) / static_cast<R>(t.Bottom));
            return *this;
        }

        // conversions
        constexpr explicit operator raylib::Rectangle() const
        {
            return { static_cast<float>(Position.X), static_cast<float>(Position.Y),
                     static_cast<float>(Size.Width),  static_cast<float>(Size.Height) };
        }

        // queries
        constexpr bool Contains(raylib::Vector2 point) const { return Contains(Point<float>{ point.x, point.y }); }

        template<typename U>
        constexpr bool Contains(const utils::Point<U>& point) const
        {
            T px = static_cast<T>(point.X);
            T py = static_cast<T>(point.Y);
            return px >= X && px <= (X + Width) && py >= Y && py <= (Y + Height);
        }

        std::string ToString() const
        {
            return std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Width) + ", " + std::to_string(Height);
        }

    private:
        // empty-tag ctor
        constexpr Rectangle(EmptyTag) noexcept
            : Position(), Size(), X(Position.X), Y(Position.Y), Width(Size.Width), Height(Size.Height), m_empty(true) {
        }
    };
}