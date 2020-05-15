#pragma once

namespace col
{
    template <typename T>
    class Vector2X
    {
    public:
        Vector2X() : m_x(), m_y() {}
        Vector2X(T x, T y) : m_x(x), m_y(y) {}

        void setX(T x) { m_x = x; }
        void setY(T y) { m_y = y; }

        T getX() const { return m_x; }
        T getY() const { return m_y; }

    private:
        T m_x;
        T m_y;
    };

    typedef Vector2X<double> Vector2;
}