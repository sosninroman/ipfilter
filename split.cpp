#include "split.h"
#include <iostream>
#include <cstdlib>

std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    auto stop = str.find_first_of(d);
    decltype(stop) start = 0;

    while(stop != std::string::npos)
    {
        r.emplace_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.emplace_back(str.substr(start));

    return r;
}
