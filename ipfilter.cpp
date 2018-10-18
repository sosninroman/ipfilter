#include "ipfilter.h"
#include <sstream>
#include <cassert>
#include <algorithm>

namespace ipfilter
{

IPAddress::IPAddress(std::string ip):
    m_ipStr(std::move(ip) )
{
    std::string byteValue;
    std::istringstream ipStream(m_ipStr);
    auto position = 0;
    while (std::getline(ipStream, byteValue, '.') )
    {
        assert(position >= 0 && position < 4);
        m_bytes[position++] = std::stoi(byteValue);
    }
}

bool operator<(const IPAddress& lhs, const IPAddress& rhs)
{
    if(lhs.firstByte() < rhs.firstByte() )
        return true;
    else if(lhs.firstByte() == rhs.firstByte() && lhs.secondByte() < rhs.secondByte() )
        return true;
    else if(lhs.firstByte() == rhs.firstByte() && lhs.secondByte() == rhs.secondByte() &&
            lhs.thirdByte() < rhs.thirdByte() )
        return true;
    else if(lhs.firstByte() == rhs.firstByte() && lhs.secondByte() == rhs.secondByte() &&
            lhs.thirdByte() == rhs.thirdByte() && lhs.fourthByte() < rhs.fourthByte() )
        return true;
    return false;
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

IPFilter::AddressVectorType IPFilter::filter_any(unsigned short value)
{
    AddressVectorType result;
    std::for_each(m_addresses.crbegin(), m_addresses.crend(),
                  [&result, value](const auto& addressPair){
        if(addressPair.first.firstByte() == value || addressPair.first.secondByte() == value ||
                addressPair.first.thirdByte() == value || addressPair.first.fourthByte() == value)
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

IPFilter::AddressVectorType IPFilter::filter(unsigned short firstByteFilter)
{
    const auto filterPrefix = std::to_string(firstByteFilter);

    auto minAddress = IPAddress(filterPrefix + std::string(".0.0.0") );
    auto maxAddress = IPAddress(filterPrefix + std::string(".255.255.255") );

    return filter(minAddress, maxAddress);
}

IPFilter::AddressVectorType IPFilter::filter(unsigned short firstByteFilter, unsigned short secondByteFilter)
{

    const auto filterPrefix =
            std::to_string(firstByteFilter).append({'.'}).append(std::to_string(secondByteFilter) );

    auto minAddress = IPAddress(filterPrefix + std::string(".0.0") );
    auto maxAddress = IPAddress(filterPrefix + std::string(".255.255") );

    return filter(minAddress, maxAddress);
}

IPFilter::AddressVectorType IPFilter::filter(const IPAddress& minAddress, const IPAddress& maxAddress)
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
