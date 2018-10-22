#include "ipfilter.h"
#include <sstream>
#include <cassert>
#include <algorithm>

namespace ipfilter
{

IPAddress::IPAddress(std::string ip)
{
    std::string byteValue;
    std::istringstream ipStream(ip);
    auto position = 0;
    while (std::getline(ipStream, byteValue, '.') )
    {
        assert(position >= 0 && position < 4);
        m_bytes[position++] = std::stoi(byteValue);
    }
}

std::string IPAddress::address() const
{
    return std::to_string(static_cast<int>(m_bytes[0])) + '.' +
            std::to_string(static_cast<int>(m_bytes[1])) + '.' +
            std::to_string(static_cast<int>(m_bytes[2])) + '.' +
            std::to_string(static_cast<int>(m_bytes[3]));
}

bool operator<(const IPAddress& lhs, const IPAddress& rhs)
{
    return lhs.m_bytes < rhs.m_bytes;
}

void IPFilter::addAddress(const std::string& str)
{
    IPAddress address(str);
    m_addresses[address]++;
    ++m_size;
}

IPFilter::AddressVectorType IPFilter::addresses() const
{
    AddressVectorType result;
    result.reserve(m_addresses.size() );

    for(auto itr = m_addresses.rbegin(); itr != m_addresses.rend(); ++itr)
    {
        decltype(IPAddressPairType::second) count = 0;
        while(count < itr->second)
        {
            result.emplace_back(itr->first.address() );
            ++count;
        }
    }

    return result;
}

IPFilter::AddressVectorType IPFilter::filter_any(unsigned char value)
{
    AddressVectorType result;
    std::for_each(m_addresses.crbegin(), m_addresses.crend(),
                  [&result, value](const auto& addressPair){
        if(addressPair.first.m_bytes[0] == value || addressPair.first.m_bytes[1] == value ||
                addressPair.first.m_bytes[2] == value || addressPair.first.m_bytes[3] == value)
        {
            decltype(IPAddressPairType::second) count = 0;
            while(count < addressPair.second)
            {
                result.emplace_back(addressPair.first.address() );
                ++count;
            }
        }
    });
    return result;
}

IPFilter::AddressVectorType IPFilter::filterByAddresses(const IPAddress& minAddress, const IPAddress& maxAddress)
{
    AddressVectorType result;

    if(maxAddress < minAddress)
        return result;

    auto litr = m_addresses.lower_bound(minAddress);
    auto ritr = m_addresses.upper_bound(maxAddress);

    if(litr != ritr)
    {
        --ritr;
        --litr;
        while(ritr != litr)
        {
            decltype(IPAddressPairType::second) count = 0;
            while(count < ritr->second)
            {
                result.emplace_back(ritr->first.address() );
                ++count;
            }
            --ritr;
        }
    }
    return result;
}

}
