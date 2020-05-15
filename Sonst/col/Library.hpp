#pragma once

namespace col
{
    template <typename T>
    class Library
    {
    public:
        typedef std::vector<T> Items;

        void add(const T& item) { m_items.push_back(item); }

        const Items& getItems() const { return m_items; }

    private:
        Items m_items;
    };
}