#pragma once

#include <ostream>

namespace col
{
    /// Color with r, g, b, a components
    /// Floating point colors, values 0 to 1
    class Color
    {
    public:
        Color()
            : m_r(), m_g(), m_b(), m_a()
        {

        }
        Color(double r, double g, double b, double a = 1.0)
            : m_r(r), m_g(g), m_b(b), m_a(a)
        {}

        void set(double r, double g, double b)
        {
            m_r = r;
            m_g = g;
            m_b = b;
        }
        void set(double r, double g, double b, double a)
        {
            m_r = r;
            m_g = g;
            m_b = b;
            m_a = a;
        }

        double getR() const { return m_r; }
        double getG() const { return m_g; }
        double getB() const { return m_b; }
        double getA() const { return m_a; }

        friend std::ostream& operator<<(std::ostream& out, const Color& color);
        friend bool operator==(const Color& color1, const Color& color2);

    private:
        double m_r;
        double m_g;
        double m_b;
        double m_a;
    };

    bool operator==(const Color& color1, const Color& color2);
}