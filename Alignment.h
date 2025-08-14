#pragma once
#include "Point.h"
#include "Size.h"
#include "Thickness.h"

namespace utils
{
    enum class HorizontalAlignment { Left, Center, Right };
    enum class VerticalAlignment { Top, Center, Bottom };

    template <typename T>
    inline Point<T> CalculateAlignment(const Point<T>& parentPosition, const Size<T>& parentSize, const Size<T>& elementSize,
        const Padding<T>& parentPadding, HorizontalAlignment hAlign, VerticalAlignment vAlign)
    {
        T contentX = parentPosition.X + parentPadding.Left;
        T contentY = parentPosition.Y + parentPadding.Top;
        T contentWidth = parentSize.Width - parentPadding.Horizontal();
        T contentHeight = parentSize.Height - parentPadding.Vertical();

        T x = contentX, y = contentY;

        switch (hAlign)
        {
        case HorizontalAlignment::Left:   x = contentX; break;
        case HorizontalAlignment::Center: x = contentX + (contentWidth - elementSize.Width) / 2; break;
        case HorizontalAlignment::Right:  x = contentX + (contentWidth - elementSize.Width); break;
        }

        switch (vAlign)
        {
        case VerticalAlignment::Top:    y = contentY; break;
        case VerticalAlignment::Center: y = contentY + (contentHeight - elementSize.Height) / 2; break;
        case VerticalAlignment::Bottom: y = contentY + (contentHeight - elementSize.Height); break;
        }

        return { x, y };
    }
}