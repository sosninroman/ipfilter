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
        m_ipParts[position++] = std::stoi(byteValue);
    }
}

bool operator<(const IPAddress& lhs, const IPAddress& rhs)
{
    if(lhs.first() < rhs.first() )
        return true;
    else if(lhs.first() == rhs.first() && lhs.second() < rhs.second() )
        return true;
    else if(lhs.first() == rhs.first() && lhs.second() == rhs.second() &&
            lhs.third() < rhs.third() )
        return true;
    else if(lhs.first() == rhs.first() && lhs.second() == rhs.second() &&
            lhs.third() == rhs.third() && lhs.fourth() < rhs.fourth() )
        return true;
    return false;
}

void IPFilter::addAddress(const std::string& str)
{
    IPAddress address(str);
    m_addresses[address]++;
    ++m_size;
}

IPFilter::AddressesVectorType IPFilter::addresses() const
{
    AddressesVectorType result;
    result.reserve(m_addresses.size() );

    //for(const auto& addressPair : m_addresses)
    for(auto itr = m_addresses.rbegin(); itr != m_addresses.rend(); ++itr)
    {
        decltype(IPAddressPairType::second) count = 0;
        //while(count < addressPair.second)
        while(count < itr->second)
        {
            //result.emplace_back(addressPair.first.address() );
            result.emplace_back(itr->first.address() );
            ++count;
        }
    }

    return result;
}

IPFilter::AddressesVectorType IPFilter::filter_any(unsigned short value)
{
    AddressesVectorType result;
    //std::for_each(m_addresses.begin(), m_addresses.end(),
    std::for_each(m_addresses.rbegin(), m_addresses.rend(),
                  [&result, value](const auto& address){
        if(address.first.first() == value || address.first.second() == value ||
                address.first.third() == value || address.first.fourth() == value)
        {
            decltype(IPAddressPairType::second) count = 0;
            while(count < address.second)
            {
                result.emplace_back(address.first.address() );
                ++count;
            }
        }
//            result.emplace_back(address.first.address() );
    });
    return result;
}

}
