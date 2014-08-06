#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__
#include <sstream>
#include <string>
#include <vector>
inline std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
inline std::vector<std::string> split(const std::string &s, std::vector<char> delims) {
    std::vector<std::string> elems;
    std::string item;
    for (char letter : s) {
        char currentChar = letter;
        bool foundDelim = false;
        for (auto delim : delims) {
            if (currentChar == delim) {
                foundDelim = true;
            }
        }
        if (foundDelim) {
            elems.push_back(item);
            item = "";
        } else {
            item += currentChar;
        }
    }
    if (item.size() != 0) {
        elems.push_back(item);
    }
    return elems;
}
inline std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
inline std::string join(const std::vector<std::string> &strings, char delim) {
    std::string result = "";
    for (auto elem : strings) {
        result += elem + delim;
    }
    result = result.substr(0, result.size() - 1);
    return result;
}
template<typename T>
std::vector<T> splice(const std::vector<T>& elements, int start = 0, int end = -1) {
    int oldlen = elements.size();
    int newlen = 0;

    if (end == -1 || end >= oldlen) {
        newlen = oldlen - start;
    } else {
        newlen = end - start;
    }

    std::vector<T> result(newlen);

    for (int i = 0; i < newlen; i++) {
        result[i] = elements[start + i];
    }
    return result;
}

#endif
