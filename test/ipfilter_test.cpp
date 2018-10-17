#include <gtest/gtest.h>
#include "ipfilter.h"

using namespace std::string_literals;

TEST(IP_ADDRESS_TEST, constructors_tests)
{
    const std::string ip("12.113.555.127");

    //ctor
    ipfilter::IPAddress address(ip);
    ASSERT_EQ(address.address(), ip);
    ASSERT_EQ(address.first(), 12);
    ASSERT_EQ(address.second(), 113);
    ASSERT_EQ(address.third(), 555);
    ASSERT_EQ(address.fourth(), 127);

    //copy ctor
    ipfilter::IPAddress address2(address);
    ASSERT_EQ(address.first(), address2.first() );
    ASSERT_EQ(address.second(), address2.second() );
    ASSERT_EQ(address.third(), address2.third() );
    ASSERT_EQ(address.fourth(), address2.fourth() );

    //move ctor
    ipfilter::IPAddress address3(std::move(address) );
    ASSERT_EQ(address3.address(), ip);
    ASSERT_EQ(address3.first(), 12);
    ASSERT_EQ(address3.second(), 113);
    ASSERT_EQ(address3.third(), 555);
    ASSERT_EQ(address3.fourth(), 127);

    //exceptions
    ASSERT_THROW(ipfilter::IPAddress("aa.aa.aa.aa"), std::invalid_argument);
    ASSERT_THROW(ipfilter::IPAddress(std::to_string(2147483648) ), std::out_of_range);
}

TEST(IP_ADDRESS_TEST, operator_less_test)
{
    const std::string ip1("12.113.555.127");
    const std::string ip2("13.113.555.127");
    const std::string ip3("13.114.555.127");
    const std::string ip4("13.114.556.127");
    const std::string ip5("13.114.556.128");

    ASSERT_TRUE(!(ip1<ip1) );
    ASSERT_TRUE( (ip1 < ip2) && (ip2 < ip3) &&
                 (ip3 < ip4) && (ip4<ip5) );
}

TEST(IP_FILTER_TEST, sort_test)
{
    ipfilter::IPFilter ipFilter;

    ipFilter.addAddress("1.70.44.170"s);
    ipFilter.addAddress("222.173.235.246"s);
    ipFilter.addAddress("1.1.234.8"s);
    ipFilter.addAddress("222.130.177.64"s);
    ipFilter.addAddress("222.82.198.61"s);
    ipFilter.addAddress("1.29.168.152"s);

    const auto sortAddresses = ipFilter.addresses();

    ASSERT_EQ(sortAddresses[0], "222.173.235.246"s);
    ASSERT_EQ(sortAddresses[1], "222.130.177.64"s);
    ASSERT_EQ(sortAddresses[2], "222.82.198.61"s);
    ASSERT_EQ(sortAddresses[3], "1.70.44.170"s);
    ASSERT_EQ(sortAddresses[4], "1.29.168.152"s);
    ASSERT_EQ(sortAddresses[5], "1.1.234.8"s);
}

TEST(IP_FILTER_TEST, filter_any_test)
{
    /// 186.204.34.46
    /// 186.46.222.194
    /// 185.50.86.22
    /// 185.46.87.231
    /// 185.46.86.132
    /// 185.46.86.131
    /// 185.46.86.131
    /// 185.46.85.78
    /// 185.38.85.204
    /// 68.46.218.208
    /// 46.223.254.56
    /// 46.223.254.56
    /// 20.251.197.23
    ipfilter::IPFilter ipFilter;

    ipFilter.addAddress("185.46.86.131"s);
    ipFilter.addAddress("46.223.254.56"s);
    ipFilter.addAddress("68.46.218.208"s);
    ipFilter.addAddress("185.46.86.131"s);
    ipFilter.addAddress("186.46.222.194"s);
    ipFilter.addAddress("185.38.85.204"s);
    ipFilter.addAddress("46.223.254.56"s);
    ipFilter.addAddress("20.251.197.23"s);
    ipFilter.addAddress("185.46.86.132"s);
    ipFilter.addAddress("186.204.34.46"s);
    ipFilter.addAddress("185.46.85.78"s);
    ipFilter.addAddress("185.50.86.22"s);
    ipFilter.addAddress("185.46.87.231"s);

    auto filterAddresses = ipFilter.filter_any(46);

    ASSERT_EQ(filterAddresses.size(), 10);

    ASSERT_EQ(filterAddresses[0], "186.204.34.46"s);
    ASSERT_EQ(filterAddresses[1], "186.46.222.194"s);
    ASSERT_EQ(filterAddresses[2], "185.46.87.231"s);
    ASSERT_EQ(filterAddresses[3], "185.46.86.132"s);
    ASSERT_EQ(filterAddresses[4], "185.46.86.131"s);
    ASSERT_EQ(filterAddresses[5], "185.46.86.131"s);
    ASSERT_EQ(filterAddresses[6], "185.46.85.78"s);
    ASSERT_EQ(filterAddresses[7], "68.46.218.208"s);
    ASSERT_EQ(filterAddresses[8], "46.223.254.56"s);
    ASSERT_EQ(filterAddresses[9], "46.223.254.56"s);

}
