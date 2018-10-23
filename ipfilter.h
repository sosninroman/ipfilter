#ifndef IPFILTER_H
#define IPFILTER_H

#include <string>
#include <map>
#include <vector>
#include <array>
#include <functional>
#include <type_traits>

namespace ipfilter
{

class IPFilter;

class IPAddress
{
public:
    IPAddress():m_bytes({0,0,0,0}){}
    //throw std::invalid_argument, std::out_of_range
    explicit IPAddress(std::string ip);

    std::string address() const;

    friend bool operator<(const IPAddress&, const IPAddress&);
    friend class IPFilter;
private:
    std::array<unsigned char, 4> m_bytes;
};

class IPFilter
{
    using AddressVectorType = std::vector<std::string>;
    using IPAddressPairType = std::map<IPAddress, unsigned int>::value_type;
public:
    void addAddress(const std::string& str);

    AddressVectorType addresses() const;

    AddressVectorType filter_any(unsigned char value);

    template<class... Types>
    AddressVectorType filter(Types... args);

    //Возвращает те адреса (из имеющихся в пуле), которые лежат между заданными адресами
    AddressVectorType filterByAddresses(const IPAddress& minAddress, const IPAddress& maxAddress);

    auto size() const {return m_size;}
private:
    std::map<IPAddress, unsigned int> m_addresses; //{адрес, количество}
    size_t m_size = 0; //размер

    template<size_t MaskSize, class... Types>
    AddressVectorType _filter(std::array<unsigned char, MaskSize>& addressMask, unsigned char byte, Types... args);

    template<size_t MaskSize>
    AddressVectorType _filter(std::array<unsigned char, MaskSize>& addressMask);

    template<size_t MaskSize>
    auto getAddresses(const std::array<unsigned char, MaskSize>& addressMask);
};

template<class... Types>
IPFilter::AddressVectorType IPFilter::filter(Types... args)
{
    static_assert(sizeof...(args) <= 4, "IPFilter::filter(Types... args) arguments error: too much arguments!");

    if(!sizeof...(args))
        return AddressVectorType();

    std::array<unsigned char, sizeof...(args)> addressMask;
    return _filter(addressMask, args...);
}

template<size_t MaskSize, class... Types>
IPFilter::AddressVectorType IPFilter::_filter(std::array<unsigned char, MaskSize>& addressMask, unsigned char byte, Types... args)
{
    addressMask[MaskSize - sizeof...(args) - 1] = byte;
    return _filter(addressMask, args...);
}

template<size_t MaskSize>
IPFilter::AddressVectorType IPFilter::_filter(std::array<unsigned char, MaskSize>& addressMask)
{
    IPAddress minAddress;
    IPAddress maxAddress;
    std::tie(minAddress, maxAddress) = getAddresses(addressMask);

    return filterByAddresses(minAddress, maxAddress);
}

template<size_t MaskSize>
auto IPFilter::getAddresses(const std::array<unsigned char, MaskSize>& addressMask)
{
    IPAddress minAddress;
    IPAddress maxAddress;

    for(decltype(MaskSize) i = 0; i < MaskSize; ++i)
    {
        minAddress.m_bytes[i] = addressMask[i];
        maxAddress.m_bytes[i] = addressMask[i];
    }

    for(decltype(MaskSize) i = 3; i > MaskSize - 1; --i)
    {
        maxAddress.m_bytes[i] = 255;
    }

    return std::make_tuple(IPAddress(minAddress), IPAddress(maxAddress) );
}

}

#endif
