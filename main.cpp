#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "ipfilter.h"
#include "split.h"

int main()
{
    ipfilter::IPFilter ipFilter;

    ipFilter.addAddress("185.46.86.131");
    ipFilter.addAddress("46.223.254.56");
    ipFilter.addAddress("68.46.218.208");
    ipFilter.addAddress("185.46.86.131");
    ipFilter.addAddress("186.46.222.194");
    ipFilter.addAddress("185.38.85.204");
    ipFilter.addAddress("46.223.254.56");
    ipFilter.addAddress("20.251.197.23");
    ipFilter.addAddress("185.46.86.132");
    ipFilter.addAddress("186.204.34.46");
    ipFilter.addAddress("185.46.85.78");
    ipFilter.addAddress("185.50.86.22");
    ipFilter.addAddress("185.46.87.231");

    //
    const auto filteredVector = ipFilter.filter(185);

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
