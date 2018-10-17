#ifndef IPFILTER_H
#define IPFILTER_H

#include <string>
#include <map>
#include <vector>
#include <array>
#include <functional>

namespace ipfilter
{

class IPAddress
{
public:
    //throw std::invalid_argument, std::out_of_range
    IPAddress(std::string ip);

    std::string address() const {return m_ipStr;}

    // 186.204.34.46 -> 186
    unsigned short first() const {return m_ipParts[0];}
    // 186.204.34.46 -> 204
    unsigned short second() const {return m_ipParts[1];}
    // 186.204.34.46 -> 34
    unsigned short third() const {return m_ipParts[2];}
    // 186.204.34.46 -> 46
    unsigned short fourth() const {return m_ipParts[3];}
private:
    std::string m_ipStr;
    std::array<unsigned short, 4> m_ipParts;
};

bool operator<(const IPAddress& lhs, const IPAddress& rhs);

class IPFilter
{
    using AddressesVectorType = std::vector<std::string>;
    using IPAddressPairType = std::map<IPAddress, unsigned int>::value_type;
public:
    void addAddress(const std::string& str);
    AddressesVectorType addresses() const;
    AddressesVectorType filter_any(unsigned short value);
    AddressesVectorType filter(unsigned short firstFilter);
    AddressesVectorType filter(unsigned short firstFilter, unsigned short secondFilter);

    auto size() const {return m_size;}
private:
    std::map<IPAddress, unsigned int> m_addresses; //{адрес, количество}
    unsigned int m_size = 0; //размер
};

}

#endif
