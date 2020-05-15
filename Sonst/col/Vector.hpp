#pragma once

namespace col
{
    /// 3-dimensional vector.
	template <typename T>
	class Vector
	{
	public:
		Vector()
		{
			m_x = T();
			m_y = T();
			m_z = T();
		}

		Vector(const T& x, const T& y, const T& z)
		{
			m_x = x;
			m_y = y;
			m_z = z;
		}

        T length() const
        {
            return std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
        }

        Vector<T> normalize()
        {
            T l = length();

            if (l != T())
            {
                return Vector<T>(m_x / l, m_y / l, m_z / l);
            }
            else
            {
                return *this;
            }
        }

		T getX() const { return m_x; }
		T getY() const { return m_y; }
		T getZ() const { return m_z; }

        template <typename T>
        friend Vector<T> operator+(const Vector<T>& v1, const Vector<T>& v2);
        template <typename T>
        friend Vector<T> operator-(const Vector<T>& v1, const Vector<T>& v2);        
        template <typename T>
        friend Vector<T> operator*(const Vector<T>& v, T s); 

        Vector<T>& operator+=(const Vector<T>& v)
        {
            m_x += v.m_x;
            m_y += v.m_y;
            m_z += v.m_z;

            return *this;
        }

	private:
		T m_x;
        T m_y;
        T m_z;
	};

    template <typename T>
    inline Vector<T> operator+(const Vector<T>& v1, const Vector<T>& v2)
    {
        T x = v1.m_x + v2.m_x;
        T y = v1.m_y + v2.m_y;
        T z = v1.m_z + v2.m_z;

        return Vector<T>(x, y, z);
    }

    template <typename T>
    inline Vector<T> operator-(const Vector<T>& v1, const Vector<T>& v2)
    {
        T x = v1.m_x - v2.m_x;
        T y = v1.m_y - v2.m_y;
        T z = v1.m_z - v2.m_z;

        return Vector<T>(x, y, z);
    }
    
    template <typename T>
    inline Vector<T> operator*(const Vector<T>& v, T s)
    {
        T x = v.m_x * s;
        T y = v.m_y * s;
        T z = v.m_z * s;

        return Vector<T>(x, y, z);
    }

	typedef Vector<double> Vector3;
}