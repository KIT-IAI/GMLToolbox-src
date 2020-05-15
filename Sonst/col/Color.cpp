#include "Color.hpp"

namespace col
{
    std::ostream& operator<<(std::ostream& out, const Color& color)
    {
        out << color.m_r << ' '
            << color.m_g << ' '
            << color.m_b << ' '
            << color.m_a;

        return out;
    }

    bool operator==(const Color& color1, const Color& color2)
    {
        return color1.m_r == color2.m_r &&
            color1.m_g == color2.m_g &&
            color1.m_b == color2.m_b &&
            color1.m_a == color2.m_a;
    }

}