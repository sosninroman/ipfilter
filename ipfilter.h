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
    explicit IPAddress(std::string ip);

    std::string address() const {return m_ipStr;}

    // 186.204.34.46 -> 186
    unsigned short firstByte() const {return m_bytes[0];}
    // 186.204.34.46 -> 204
    unsigned short secondByte() const {return m_bytes[1];}
    // 186.204.34.46 -> 34
    unsigned short thirdByte() const {return m_bytes[2];}
    // 186.204.34.46 -> 46
    unsigned short fourthByte() const {return m_bytes[3];}
private:
    std::string m_ipStr;
    std::array<unsigned short, 4> m_bytes;
};

bool operator<(const IPAddress& lhs, const IPAddress& rhs);

class IPFilter
{
    using AddressVectorType = std::vector<std::string>;
    using IPAddressPairType = std::map<IPAddress, unsigned int>::value_type;
public:
    void addAddress(const std::string& str);

    AddressVectorType addresses() const;

    AddressVectorType filter_any(unsigned short value);

    AddressVectorType filter(unsigned short firstByteFilter);

    AddressVectorType filter(unsigned short firstByteFilter, unsigned short secondByteFilter);

    //Возвращает те адреса (из имеющихся в пуле), которые лежат между заданными адресами
    AddressVectorType filter(const IPAddress& minAddress, const IPAddress& maxAddress);

    auto size() const {return m_size;}
private:
    std::map<IPAddress, unsigned int> m_addresses; //{адрес, количество}
    unsigned int m_size = 0; //размер
};

}

#endif
