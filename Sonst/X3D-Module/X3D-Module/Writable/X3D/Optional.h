#pragma once

#include <cassert>

namespace X3D
{
	template <typename T>
	class Optional
	{
	public:
		Optional()
			: m_hasValue(false)
		{

		}

		Optional(const T& val)
			: m_val(val), m_hasValue(true)
		{
		}

		const T& operator*() const
		{
			assert(m_hasValue);
			return m_val;
		}

		const T* operator->() const
		{
			assert(m_hasValue);
			return &m_val;
		}

		T* operator->()
		{
			assert(m_hasValue);
			return &m_val;
		}

		bool operator!() const
		{
			return !m_hasValue;
		}

		operator bool() const
		{

			return m_hasValue;
		}

		Optional<T>& operator=(const T& val)
		{
			m_hasValue = true;
			m_val = val;

			return *this;
		}

	private:
		bool m_hasValue;
		T m_val;
	};
}
