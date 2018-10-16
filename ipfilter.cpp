#include "ipfilter.h"
#include <sstream>
#include <cassert>

namespace ipfilter
{

IPAddress::IPAddress(std::string ip):
    m_ipStr(std::move(ip) )
{
    std::string byteValue;
    std::istringstream ipStream(m_ipStr);
    auto shift = 3;
    while (std::getline(ipStream, byteValue, '.') )
    {
        assert(shift >= 0 && shift <= 3);
        m_ip |= atoi(byteValue.c_str() ) << shift;
        --shift;
    }
}

unsigned short IPAddress::first() const
{
    return static_cast<unsigned short>(m_ip >> 3);
}

unsigned short IPAddress::second() const
{
    return static_cast<unsigned short>(m_ip >> 2);
}

unsigned short IPAddress::third() const
{
    return static_cast<unsigned short>(m_ip >> 2);
}

unsigned short IPAddress::fourth() const
{
    return static_cast<unsigned short>(m_ip >> 3);
}

bool operator<(const IPAddress& lhs, const IPAddress& rhs)
{
    return true;
}

void IPFilter::addAddress(const std::string& str)
{
    m_addresses.emplace(str);
}
}
