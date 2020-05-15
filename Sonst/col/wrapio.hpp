#pragma once

#include <ostream>

namespace col
{
    inline std::ostream& operator<<(std::ostream& out, Wrap wrap)
    {
        switch (wrap)
        {
        case WRAP:
            out << "WRAP";
            break;
        case CLAMP:
            out << "CLAMP";
            break;
        case MIRROR:
            out << "MIRROR";
            break;
        case NONE:
            out << "NONE";
            break;
        case BORDER:
            out << "BORDER";
            break;
        }

        return out;
    }
}