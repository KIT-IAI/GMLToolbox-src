#include <algorithm>

#include "NCName.hpp"

namespace col
{
    const std::wstring ALPHABETIC_RANGE = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::wstring NUMERIC_RANGE = L"0123456789";
    const std::wstring SPECIAL_RANGE = L"_.-";

    bool isValidAlpabetic(wchar_t c)
    {
        return ALPHABETIC_RANGE.find(c) != std::wstring::npos;
    }

    bool isValidNumeric(wchar_t c)
    {
        return NUMERIC_RANGE.find(c) != std::wstring::npos;
    }

    bool isValidSpecial(wchar_t c)
    {
        return SPECIAL_RANGE.find(c) != std::wstring::npos;
    }

    bool isValid(wchar_t c)
    {
        return isValidSpecial(c) || isValidNumeric(c) || isValidAlpabetic(c);
    }

    bool isNotValid(wchar_t c)
    {
        return !isValid(c);
    }

    std::string makeValidNCName(const std::string& name)
    {
        // First letter: alpha, _
        // Following letters: alnum, _

        if (name.empty())
        {
            return "_";
        }

        std::wstring temp;

        try
        {
            temp = boost::locale::conv::utf_to_utf<wchar_t>(name);
        }
        catch (const std::exception& e)
        {
            return "_";        	
        }

        if (!isValidAlpabetic(temp[0]))
        {
            temp[0] = '_';
        }

        if (temp.size() > 1)
        {
            std::replace_if(temp.begin() + 1, temp.end(), isNotValid, '_');
        }

        try
        {
            return boost::locale::conv::utf_to_utf<char>(temp);
        }
        catch (const std::exception& e)
        {
            return "_";        	
        } 
    }
}
