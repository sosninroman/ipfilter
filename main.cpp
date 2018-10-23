#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "ipfilter.h"
#include "split.h"

int main()
{
    try
    {
        ipfilter::IPFilter ipFilter;

        for(std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            ipFilter.addAddress(v.at(0) );
        }

        auto filterResult = ipFilter.addresses();

        for(const auto& address : filterResult)
        {
            std::cout << address << std::endl;
        }

        // filter by first byte and output
        filterResult = ipFilter.filter(1);

        for(const auto& address : filterResult)
        {
            std::cout << address << std::endl;
        }


        // filter by first and second bytes and output
        filterResult = ipFilter.filter(46, 70);

        for(const auto& address : filterResult)
        {
            std::cout << address << std::endl;
        }

        // filter by any byte and output
        filterResult = ipFilter.filter_any(46);

        for(const auto& address : filterResult)
        {
            std::cout << address << std::endl;
        }

    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
