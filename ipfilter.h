#ifndef IPFILTER_H
#define IPFILTER_H

#include <string>
#include <set>
#include <vector>

namespace ipfilter
{

class IPAddress
{
public:
    IPAddress(std::string ip);

    std::string address() const {return m_ipStr;}

    // 186.204.34.46 -> 186
    unsigned short first() const;
    // 186.204.34.46 -> 204
    unsigned short second() const;
    // 186.204.34.46 -> 34
    unsigned short third() const;
    // 186.204.34.46 -> 46
    unsigned short fourth() const;
private:
    std::string m_ipStr;
    int m_ip = 0;
};

bool operator<(const IPAddress& lhs, const IPAddress& rhs);

class IPFilter
{
    using AddressesVector = std::vector<std::string>;
public:
    void addAddress(const std::string& str);
    AddressesVector filter_any(unsigned short value);
    AddressesVector filter(unsigned short firstFilter);
    AddressesVector filter(unsigned short firstFilter, unsigned short secondFilter);
private:
    std::set<IPAddress> m_addresses;
};

}

#endif
