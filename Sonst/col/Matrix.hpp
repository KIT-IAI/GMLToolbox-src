#pragma once

namespace col
{
    /// Matrix
    template <typename T, size_t W, size_t H>
    class Matrix
    {
    public:
        Matrix()
        {
            for (size_t x(0); x < W; ++x)
            {
                for (size_t y(0); y < H; ++y)
                {
                    m_data[x][y] = T();
                }
            }
        }

        void set(size_t x, size_t y, const T& val)
        {
            m_data[x][y] = val;
        }
        
        const T& get(size_t x, size_t y) const
        {
            return m_data[x][y];
        }

        template <typename T, size_t W, size_t H>
        friend Matrix<T, W, H> operator*(const Matrix<T, W, H>& m1, const Matrix<T, W, H>& m2);

        template <typename T, size_t W, size_t H>
        friend std::ostream& operator<<(std::ostream& out, Matrix<T, W, H>& m)
        {
            for (size_t y(0); y < H; ++y)
            {
                for (size_t x(0); x < W; ++x)
                {
                    out << m.get(x, y);

                    if (y < 3 || x < 3)
                    {
                        out << ' ';
                    }
                }
            }

            return out;
        }

    private:
        T m_data[W][H];
    };

    template <typename T, size_t W, size_t H>
    Matrix<T, W, H> operator*(const Matrix<T, W, H>& m1, const Matrix<T, W, H>& m2)
    {
        // TODO: assert W=H, H=W
        Matrix4 r;

        for (size_t x(0); x < W; ++x)
        {
            for (size_t y(0); y < H; ++y)
            {
                for (size_t i(0); i < W; ++i)
                {
                    r.m_data[X][Y] += m1.m_data[i][y] * m2.m_data[x][i];
                }
            }
        }

        return r;
    }

    template <typename T, size_t W, size_t H>
    Matrix<T, W, H> invert(const Matrix<T, W, H>& m)
    {
        
    }

    template <typename T, size_t W, size_t H>
    Matrix<T, W, H> makeIdentity()
    {
        assert(W == H);

        Matrix<T, W, H> temp;

        for (size_t i(0); i < W; ++i)
        {
            // TODO: Make one
            temp.set(i, i, 1.0);
        }

        return temp;
    }

    template <typename T, size_t W, size_t H>
    bool operator!=(const Matrix<T, W, H>& m1, const Matrix<T, W, H>& m2)
    {
        for (size_t x(0); x < W; ++x)
        {
            for (size_t y(0); y < H; ++y)
            {
                if (m1.get(x, y) != m2.get(x, y))
                {
                    return true;
                }
            }
        }

        return false;
    }

    typedef Matrix<double, 4, 4> Matrix4;
}