#include "StringUtils.h"
#include <string>
#include <cmath>

namespace StringUtils {

    std::string Slice(const std::string& str, ssize_t start, ssize_t end) noexcept {
        // converts start & end to their positive equivalents if negative
        int len = str.length();
        int substr_start = (start < 0) ? (start + len) : start;
        int substr_end = (end <= 0) ? (end + len) : end;

        // returns empty string immediately if making an actual substring is impossible
        if (substr_start >= len || substr_end < 0 || substr_start > substr_end || str == "") {
            return "";
        }
        // out-of-bounds indices set to either the beginning or end of string as appropriate
        if (substr_start < 0) {
            substr_start = 0;
        }
        if (substr_end > len) {
            substr_end = len;
        }

        return str.substr(substr_start, substr_end - substr_start);
    }

    std::string Capitalize(const std::string& str) noexcept {
        // empty string is immediately returned
        if (str == "") {
            return "";
        }

        int len = str.length();

        // first char of new string is capitalized if possible
        std::string new_str = "";
        new_str += toupper(str[0]);

        // rest of string is lowercased and added to new string
        for (int i = 1; i < len; ++i) {
            new_str += tolower(str[i]);
        }

        return new_str;
    }

    std::string Upper(const std::string& str) noexcept {
        int len = str.length();
        std::string new_str = "";

        // entire string is capitalized and added to new string char by char
        for (int i = 0; i < len; ++i) {
            new_str += toupper(str[i]);
        }

        return new_str;
    }

    std::string Lower(const std::string& str) noexcept {
        int len = str.length();
        std::string new_str = "";

        // entire string is lowercased and added to new string char by char
        for (int i = 0; i < len; ++i) {
            new_str += tolower(str[i]);
        }

        return new_str;
    }

    std::string LStrip(const std::string& str) noexcept {
        int len = str.length();
        int i = 0;

        // index is set to first non-whitespace character
        while (isspace(str[i])) {
            ++i;
        }

        return str.substr(i, len - i);
    }

    std::string RStrip(const std::string& str) noexcept {
        int len = str.length();
        int i = len - 1;

        // index is set to last non-whitespace character
        while (isspace(str[i])) {
            --i;
        }

        return str.substr(0, i + 1);
    }

    std::string Strip(const std::string& str) noexcept {
        // LStrip & RStrip are used in succession
        return StringUtils::RStrip(StringUtils::LStrip(str));
    }

    std::string Center(const std::string& str, int width, char fill) noexcept {
            std::string newstring = str;

    if (width < 0)
    return newstring;

    if (width < (str.length()))
    return newstring;

    if (str.length() == 0)
    {
    std::string newstring(width, fill);
    return newstring;
    }

    std::string leftspaces = "";
    std::string rightspaces = "";

    int filled = width - str.length();

    if (filled % 2 == 1)   //if odd
    {
        leftspaces.append(floor(filled/2), fill);
        rightspaces.append(floor(filled/2) +1, fill);
    }

    else
    {
        leftspaces.append((filled/2), fill);
        rightspaces.append((filled/2), fill);
    }

    newstring = leftspaces + newstring + rightspaces;

    return newstring;
    }

    std::string LJust(const std::string& str, int width, char fill) noexcept {
        // new string is set to str, and then the appropriate number of fill chars are added

        std::string new_str = str;
        int length = str.length();

        for (int i = 0; i < (width - length); ++i) {
            new_str += fill;
        }

        return new_str;
    }

    std::string RJust(const std::string& str, int width, char fill) noexcept {
        // appropriate number of fill chars are added to new string, then str is added

        std::string new_str = "";
        int length = str.length();

        for (int i = 0; i < width - length; ++i) {
            new_str += fill;
        }
        new_str += str;

        return new_str;
    }

    std::string Replace(const std::string& str, const std::string& old, const std::string& rep) noexcept {
        std::string new_str = "";
        int str_len = str.length();
        int old_len = old.length();
        int rep_index = str.find(old);          // index of first occurence of substring is found to prepare for the loop
        int i = 0;

        if (old == "") {
            for (; i < str_len; ++i) {
                new_str += rep;
                new_str += str[i];
            }
            new_str += rep;

            return new_str;
        }

        // while loop runs as long as substring is still found in str
        // (str.npos indicates substring wasn't found)
        while (rep_index != str.npos) {

            // chars from old string added to new string up until the replacement index, then replacement string is added
            for (; i < rep_index; ++i) {
                new_str += str[i];
            }
            new_str += rep;

            // replacement index set to next occurence of substring
            rep_index = str.find(old, rep_index + old_len);

            // string counter set to index directly after replacement string
            i += old_len;
        }

        // one last segment of main string still hasn't been added to new string
        // this for loop adds it in
        for (; i < str_len; ++i) {
            new_str += str[i];
        }

        return new_str;
    }

    std::vector< std::string > Split(const std::string& str, const std::string& splt) noexcept {
        std::vector< std::string > splt_str;            // this is the vector that will be returned
        std::string substr = "";                        // this variable will create the strings that will be added to the above vector
        int str_len = str.length();
        int splt_len = splt.length();
        int splt_index = str.find(splt);                // index of first occurence of substring is found to prepare for the loop
        int i = 0;

        // runs if the user wants to split at whitespace
        if (splt == "") {

            // iterates over entire length of string
            for (; i < str_len; ++i) {

                // adds non-whitespace chars to string creation variable
                if (!isspace(str[i])) {
                    substr += str[i];
                }

                // if it isn't empty, string variable is added to vector once whitespace is detected
                // this is because detecting whitespace means that the string variable has ended
                else {
                    if (substr != "") {
                        splt_str.push_back(substr);
                    }
                    substr = "";            // string variable is reset
                }
            }

            // one last segment of main string still hasn't been added
            // this adds it if it isn't whitespace
            if (substr != "") {
                splt_str.push_back(substr);
            }
        }

        // runs if the user wants to split at anything else
        else {
            // while loop runs as long as substring is still found in str
            while (splt_index != str.npos) {
                for (; i < splt_index; ++i) {
                    substr += str[i];
                }
                splt_str.push_back(substr);
                substr = "";

                splt_index = str.find(splt, splt_index + splt_len);
                i += splt_len;
            }

            // one last segment of main string still hasn't been added
            // this adds it
            for (; i < str_len; ++i) {
                substr += str[i];
            }
            splt_str.push_back(substr);
        }

        return splt_str;
    }

    std::string Join(const std::string& str, const std::vector< std::string >& vect) noexcept {
        std::string new_str = "";
        int vect_size = vect.size();

        for (int i = 0; i < vect_size; ++i) {
            new_str += vect[i];
            if (i < (vect_size - 1)) {
                new_str += str;
            }
        }

        return new_str;
    }

    std::string ExpandTabs(const std::string& str, int tabsize) noexcept {
        std::string new_str = "";
        int str_len = str.length();
        int new_cntr = 0;
        int num_spaces;

        if (tabsize == 0) {
            for (int i = 0; i < str_len; ++i) {
                if (str[i] != '\t') {
                    new_str += str[i];
                }
            }

            return new_str;
        }

        for (int i = 0; i < str_len; ++i) {
            if (str[i] == '\t') {
                num_spaces = tabsize - (new_cntr % tabsize);
                for (int j = 0; j < num_spaces; ++j) {
                    new_str += " ";
                    ++new_cntr;
                }
            }
            else if (str[i] == '\n' || str[i] == '\r') {
                new_str += str[i];
                new_cntr = 0;
            }
            else {
                new_str += str[i];
                ++new_cntr;
            }
        }

        return new_str;
    }

    int EditDistance(const std::string& left, const std::string& right, bool ignorecase) noexcept {
        std::string new_left = (ignorecase) ? StringUtils::Lower(left) : left;
        std::string new_right = (ignorecase) ? StringUtils::Lower(right) : right;

        if (new_left == "") {
            return new_right.length();
        }
        else if (new_right == "") {
            return new_left.length();
        }
        else if (new_left[0] == new_right[0]) {
            return StringUtils::EditDistance(StringUtils::Slice(new_left, 1), StringUtils::Slice(new_right, 1));
        }
        else {
            return 1 + std::min(StringUtils::EditDistance(StringUtils::Slice(new_left, 1), new_right),
                std::min(StringUtils::EditDistance(new_left, StringUtils::Slice(new_right, 1)),
                    StringUtils::EditDistance(StringUtils::Slice(new_left, 1), StringUtils::Slice(new_right, 1))));
        }
    }

};