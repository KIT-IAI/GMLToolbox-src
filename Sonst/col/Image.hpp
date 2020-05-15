#pragma once

#include <boost/cstdint.hpp>
#include <boost/filesystem.hpp>

#include "ColFwd.hpp"

/// Global namespace for (col)lada export
namespace col
{
    /// Reference to an image file.
    class Image : public boost::noncopyable
    {
    public:
        Image(const std::string& id) : m_id(id) {}

        const boost::filesystem::path& getPath() const { return m_path; }
        void setPath(const boost::filesystem::path& path) { m_path = path; }

        const std::string& getId() const { return m_id; }

        static void deleteMe(Image* image) { delete image; }

    private:
        ~Image() {}

        std::string m_id;

        /// The (relative) path to an image file.
        boost::filesystem::path m_path;
    };
}