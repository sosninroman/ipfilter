#include "ipfilter.h"
#include <sstream>
#include <cassert>
#include <algorithm>

namespace ipfilter
{

namespace internal
{

void addressesRange::next()
{
    assert(m_currentIterator != m_addressesMap->crend() );
    if(m_counter < m_currentIterator->second-1)
    {
        ++m_counter;
    }
    else
    {
        ++m_currentIterator;
        m_counter = 0;
    }
}

std::size_t addressesRange::size()
{
    size_t result = m_addressesMap->size();

    ranges::for_each(*m_addressesMap,
                  [&result](const std::pair<const IPAddress,unsigned int>& val){result += val.second-1;});

    return result;
}

}//internal

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

    internal::addressesRange rng(&m_addresses);
    result.reserve(rng.size() );
    ranges::for_each(rng, [&result](const IPAddress& addr){
            result.push_back(addr.address());
        });

    return result;
}

IPFilter::AddressVectorType IPFilter::filter_any(unsigned char value)
{
    AddressVectorType result;

    ranges::for_each(
                internal::addressesRange(&m_addresses) |
                ranges::view::filter(
                    [value](const IPAddress& addr)
                    {
                        return addr.m_bytes[0] == value || addr.m_bytes[1] == value ||
                                addr.m_bytes[2] == value || addr.m_bytes[3] == value;
                    }),
    [&result](const IPAddress& addr){result.push_back(addr.address() );}
    );

    return result;
}

IPFilter::AddressVectorType IPFilter::filterByAddresses(const IPAddress& minAddress, const IPAddress& maxAddress)
{
    AddressVectorType result;

    if(maxAddress < minAddress)
        return result;

    auto ritr = m_addresses.upper_bound(maxAddress);

    ranges::for_each(
                internal::addressesRange(&m_addresses) |
                ranges::view::filter(
                         [minAddr = minAddress, ritr, allFromRightSide = (ritr == m_addresses.end() )]
                         (const IPAddress& addr)
                        {
                            return (allFromRightSide || addr < ritr->first) && !(addr < minAddr);
                        }),
    [&result](const IPAddress& addr){result.emplace_back(addr.address() );}
    );

    return result;
}

}
