#include <gtest/gtest.h>
#include "ipfilter.h"
#include "split.h"

TEST(IP_ADDRESS_TEST, constructors_tests)
{
    const std::string ip("12.113.200.127");

    //ctor
    ipfilter::IPAddress address(ip);
    ASSERT_EQ(address.address(), ip);

    //copy ctor
    ipfilter::IPAddress address2(address);
    ASSERT_EQ(address.address(), address2.address() );

    //move ctor
    ipfilter::IPAddress address3(std::move(address) );
    ASSERT_EQ(address3.address(), ip);

    //exceptions
    ASSERT_THROW(ipfilter::IPAddress("aa.aa.aa.aa"), std::invalid_argument);
    ASSERT_THROW(ipfilter::IPAddress(std::to_string(2147483648) ), std::out_of_range);
}

TEST(IP_ADDRESS_TEST, operator_less_test)
{
    const std::string ip1("12.113.200.127");
    const std::string ip2("13.113.200.127");
    const std::string ip3("13.114.200.127");
    const std::string ip4("13.114.556.127");
    const std::string ip5("13.114.556.128");

    ASSERT_TRUE(!(ip1<ip1) );
    ASSERT_TRUE( (ip1 < ip2) && (ip2 < ip3) &&
                 (ip3 < ip4) && (ip4<ip5) );
}

TEST(IP_FILTER_TEST, sort_test)
{
    ipfilter::IPFilter ipFilter;

    ipFilter.addAddress("1.70.44.170");
    ipFilter.addAddress("222.173.235.246");
    ipFilter.addAddress("1.1.234.8");
    ipFilter.addAddress("222.130.177.64");
    ipFilter.addAddress("222.82.198.61");
    ipFilter.addAddress("1.29.168.152");

    ASSERT_EQ(ipFilter.size(), 6);

    const auto sortAddresses = ipFilter.addresses();

    ASSERT_EQ(sortAddresses[0], "222.173.235.246");
    ASSERT_EQ(sortAddresses[1], "222.130.177.64");
    ASSERT_EQ(sortAddresses[2], "222.82.198.61");
    ASSERT_EQ(sortAddresses[3], "1.70.44.170");
    ASSERT_EQ(sortAddresses[4], "1.29.168.152");
    ASSERT_EQ(sortAddresses[5], "1.1.234.8");
}

TEST(IP_FILTER_TEST, sort_with_duplicates_test)
{
    ipfilter::IPFilter ipFilter;

    ipFilter.addAddress("185.46.86.131");
    ipFilter.addAddress("46.223.254.56");
    ipFilter.addAddress("68.46.218.208");
    ipFilter.addAddress("185.46.86.131");
    ipFilter.addAddress("186.46.222.194");
    ipFilter.addAddress("46.223.254.56");

    ASSERT_EQ(ipFilter.size(), 6);

    const auto sortAddresses = ipFilter.addresses();

    ASSERT_EQ(sortAddresses[0], "186.46.222.194");
    ASSERT_EQ(sortAddresses[1], "185.46.86.131");
    ASSERT_EQ(sortAddresses[2], "185.46.86.131");
    ASSERT_EQ(sortAddresses[3], "68.46.218.208");
    ASSERT_EQ(sortAddresses[4], "46.223.254.56");
    ASSERT_EQ(sortAddresses[5], "46.223.254.56");
}

TEST(IP_FILTER_TEST, filter_any_test)
{
    ipfilter::IPFilter ipFilter;

    ipFilter.addAddress("185.46.86.131");
    ipFilter.addAddress("46.223.254.56");
    ipFilter.addAddress("68.46.218.208");
    ipFilter.addAddress("185.46.86.131");
    ipFilter.addAddress("186.46.222.194");
    ipFilter.addAddress("185.38.85.204");
    ipFilter.addAddress("46.223.254.56");
    ipFilter.addAddress("20.251.197.23");
    ipFilter.addAddress("185.46.86.132");
    ipFilter.addAddress("186.204.34.46");
    ipFilter.addAddress("185.46.85.78");
    ipFilter.addAddress("185.50.86.22");
    ipFilter.addAddress("185.46.87.231");

    auto filterAddresses = ipFilter.filter_any(46);

    ASSERT_EQ(filterAddresses.size(), 10);

    ASSERT_EQ(filterAddresses[0], "186.204.34.46");
    ASSERT_EQ(filterAddresses[1], "186.46.222.194");
    ASSERT_EQ(filterAddresses[2], "185.46.87.231");
    ASSERT_EQ(filterAddresses[3], "185.46.86.132");
    ASSERT_EQ(filterAddresses[4], "185.46.86.131");
    ASSERT_EQ(filterAddresses[5], "185.46.86.131");
    ASSERT_EQ(filterAddresses[6], "185.46.85.78");
    ASSERT_EQ(filterAddresses[7], "68.46.218.208");
    ASSERT_EQ(filterAddresses[8], "46.223.254.56");
    ASSERT_EQ(filterAddresses[9], "46.223.254.56");

}

TEST(IP_FILTER_TEST, filter_by_first_byte_test)
{
    ipfilter::IPFilter ipFilter;

    ipFilter.addAddress("185.46.86.131");
    ipFilter.addAddress("46.223.254.56");
    ipFilter.addAddress("68.46.218.208");
    ipFilter.addAddress("185.46.86.131");
    ipFilter.addAddress("186.46.222.194");
    ipFilter.addAddress("185.38.85.204");
    ipFilter.addAddress("46.223.254.56");
    ipFilter.addAddress("20.251.197.23");
    ipFilter.addAddress("185.46.86.132");
    ipFilter.addAddress("186.204.34.46");
    ipFilter.addAddress("185.46.85.78");
    ipFilter.addAddress("185.50.86.22");
    ipFilter.addAddress("185.46.87.231");

    //
    const auto filteredVector = ipFilter.filter(185);

    ASSERT_EQ(filteredVector.size(), 7);
    ASSERT_EQ(filteredVector[0], "185.50.86.22");
    ASSERT_EQ(filteredVector[1], "185.46.87.231");
    ASSERT_EQ(filteredVector[2], "185.46.86.132");
    ASSERT_EQ(filteredVector[3], "185.46.86.131");
    ASSERT_EQ(filteredVector[4], "185.46.86.131");
    ASSERT_EQ(filteredVector[5], "185.46.85.78");
    ASSERT_EQ(filteredVector[6], "185.38.85.204");

    //
    const auto emptyFilteredVector = ipFilter.filter(1);

    ASSERT_TRUE(emptyFilteredVector.empty() );
}

TEST(IP_FILTER_TEST, filter_by_first_second_byte_test)
{
    ipfilter::IPFilter ipFilter;

    ipFilter.addAddress("185.46.86.131");
    ipFilter.addAddress("46.223.254.56");
    ipFilter.addAddress("68.46.218.208");
    ipFilter.addAddress("185.46.86.131");
    ipFilter.addAddress("186.46.222.194");
    ipFilter.addAddress("185.38.85.204");
    ipFilter.addAddress("46.223.254.56");
    ipFilter.addAddress("20.251.197.23");
    ipFilter.addAddress("185.46.86.132");
    ipFilter.addAddress("186.204.34.46");
    ipFilter.addAddress("185.46.85.78");
    ipFilter.addAddress("185.50.86.22");
    ipFilter.addAddress("185.46.87.231");

    //
    const auto filteredVector = ipFilter.filter(185, 46);

    ASSERT_EQ(filteredVector.size(), 5);
    ASSERT_EQ(filteredVector[0], "185.46.87.231");
    ASSERT_EQ(filteredVector[1], "185.46.86.132");
    ASSERT_EQ(filteredVector[2], "185.46.86.131");
    ASSERT_EQ(filteredVector[3], "185.46.86.131");
    ASSERT_EQ(filteredVector[4], "185.46.85.78");

    //
    const auto emptyFilteredVector1 = ipFilter.filter(1, 46);
    ASSERT_TRUE(emptyFilteredVector1.empty() );

    const auto emptyFilteredVector2 = ipFilter.filter(185, 2);
    ASSERT_TRUE(emptyFilteredVector2.empty() );
}

TEST(IP_FILTER_TEST, filter_by_first_second_third_byte_test)
{
    ipfilter::IPFilter ipFilter;

    ipFilter.addAddress("185.46.86.131");
    ipFilter.addAddress("46.223.254.56");
    ipFilter.addAddress("68.46.218.208");
    ipFilter.addAddress("185.46.86.131");
    ipFilter.addAddress("186.46.222.194");
    ipFilter.addAddress("185.38.85.204");
    ipFilter.addAddress("46.223.254.56");
    ipFilter.addAddress("20.251.197.23");
    ipFilter.addAddress("185.46.86.132");
    ipFilter.addAddress("186.204.34.46");
    ipFilter.addAddress("185.46.85.78");
    ipFilter.addAddress("185.50.86.22");
    ipFilter.addAddress("185.46.87.231");

    //
    const auto filteredVector = ipFilter.filter(185, 46, 86);

    ASSERT_EQ(filteredVector.size(), 3);
    ASSERT_EQ(filteredVector[0], "185.46.86.132");
    ASSERT_EQ(filteredVector[1], "185.46.86.131");
    ASSERT_EQ(filteredVector[2], "185.46.86.131");

    //
    const auto emptyFilteredVector1 = ipFilter.filter(1, 46, 54);
    ASSERT_TRUE(emptyFilteredVector1.empty() );

    const auto emptyFilteredVector2 = ipFilter.filter(185, 2, 54);
    ASSERT_TRUE(emptyFilteredVector2.empty() );

    const auto emptyFilteredVector3 = ipFilter.filter(18, 23, 23);
    ASSERT_TRUE(emptyFilteredVector3.empty() );
}

TEST(IP_FILTER_TEST, filter_by_first_second_third_fourth_byte_test)
{
    ipfilter::IPFilter ipFilter;

    ipFilter.addAddress("185.46.86.131");
    ipFilter.addAddress("46.223.254.56");
    ipFilter.addAddress("68.46.218.208");
    ipFilter.addAddress("185.46.86.131");
    ipFilter.addAddress("186.46.222.194");
    ipFilter.addAddress("185.38.85.204");
    ipFilter.addAddress("46.223.254.56");
    ipFilter.addAddress("20.251.197.23");
    ipFilter.addAddress("185.46.86.132");
    ipFilter.addAddress("186.204.34.46");
    ipFilter.addAddress("185.46.85.78");
    ipFilter.addAddress("185.50.86.22");
    ipFilter.addAddress("185.46.87.231");

    //
    const auto filteredVector = ipFilter.filter(185, 46, 86, 131);

    ASSERT_EQ(filteredVector.size(), 2);
    ASSERT_EQ(filteredVector[0], "185.46.86.131");
    ASSERT_EQ(filteredVector[1], "185.46.86.131");

    //
    const auto emptyFilteredVector1 = ipFilter.filter(1, 46, 54, 44);
    ASSERT_TRUE(emptyFilteredVector1.empty() );


    const auto filteredVector2 = ipFilter.filter(185, 46, 86, 132);

    ASSERT_EQ(filteredVector2.size(), 1);
    ASSERT_EQ(filteredVector2[0], "185.46.86.132");
}

//В случае пустого фильтра будет возвращен пустой вектор
TEST(IP_FILTER_TEST, empty_filter_test)
{
    ipfilter::IPFilter ipFilter;

    ipFilter.addAddress("185.46.86.131");
    ipFilter.addAddress("46.223.254.56");
    ipFilter.addAddress("68.46.218.208");
    ipFilter.addAddress("185.46.86.131");
    ipFilter.addAddress("186.46.222.194");

    const auto result = ipFilter.filter();
    ASSERT_TRUE(result.empty() );
}

TEST(IP_FILTER_TEST, filter_by_range_test)
{
    ipfilter::IPFilter ipFilter;

    ipFilter.addAddress("68.46.13.5");
    ipFilter.addAddress("185.46.86.131");
    ipFilter.addAddress("46.223.254.56");
    ipFilter.addAddress("68.46.218.208");
    ipFilter.addAddress("192.1.56.98");
    ipFilter.addAddress("185.46.86.131");
    ipFilter.addAddress("186.46.222.194");
    ipFilter.addAddress("58.1.1.1");
    ipFilter.addAddress("46.223.254.56");

    ASSERT_EQ(ipFilter.size(), 9);

    const auto sortAddresses = ipFilter.addresses();

    ASSERT_EQ(sortAddresses[0], "192.1.56.98");
    ASSERT_EQ(sortAddresses[1], "186.46.222.194");
    ASSERT_EQ(sortAddresses[2], "185.46.86.131");
    ASSERT_EQ(sortAddresses[3], "185.46.86.131");
    ASSERT_EQ(sortAddresses[4], "68.46.218.208");
    ASSERT_EQ(sortAddresses[5], "68.46.13.5");
    ASSERT_EQ(sortAddresses[6], "58.1.1.1");
    ASSERT_EQ(sortAddresses[7], "46.223.254.56");
    ASSERT_EQ(sortAddresses[8], "46.223.254.56");

    auto v1 = ipFilter.filterByAddresses(ipfilter::IPAddress("127.0.0.1"), ipfilter::IPAddress("127.0.0.2") );
    ASSERT_TRUE(v1.empty() );

    auto v2 = ipFilter.filterByAddresses(ipfilter::IPAddress("1.0.0.0"), ipfilter::IPAddress("200.0.0.0") );
    ASSERT_EQ(v2.size(), ipFilter.size() );
    ASSERT_EQ(v2[0], "192.1.56.98");
    ASSERT_EQ(v2[1], "186.46.222.194");
    ASSERT_EQ(v2[2], "185.46.86.131");
    ASSERT_EQ(v2[3], "185.46.86.131");
    ASSERT_EQ(v2[4], "68.46.218.208");
    ASSERT_EQ(v2[5], "68.46.13.5");
    ASSERT_EQ(v2[6], "58.1.1.1");
    ASSERT_EQ(v2[7], "46.223.254.56");
    ASSERT_EQ(v2[8], "46.223.254.56");

    auto v3 = ipFilter.filterByAddresses(ipfilter::IPAddress("68.46.13.5"), ipfilter::IPAddress("68.46.13.5") );
    ASSERT_EQ(v3.size(), 1);
    ASSERT_EQ(v3[0], "68.46.13.5");

    auto v4 = ipFilter.filterByAddresses(ipfilter::IPAddress("192.0.0.1"), ipfilter::IPAddress("127.0.0.2") );
    ASSERT_TRUE(v4.empty() );

    auto v5 = ipFilter.filterByAddresses(ipfilter::IPAddress("185.0.0.0"), ipfilter::IPAddress("185.255.255.255") );
    ASSERT_EQ(v5.size(), 2);
    ASSERT_EQ(v5[0], "185.46.86.131");
    ASSERT_EQ(v5[1], "185.46.86.131");

    auto v6 = ipFilter.filterByAddresses(ipfilter::IPAddress("58.1.1.1"), ipfilter::IPAddress("185.46.86.131") );
    ASSERT_EQ(v6.size(), 5);
    ASSERT_EQ(v6[0], "185.46.86.131");
    ASSERT_EQ(v6[1], "185.46.86.131");
    ASSERT_EQ(v6[2], "68.46.218.208");
    ASSERT_EQ(v6[3], "68.46.13.5");
    ASSERT_EQ(v6[4], "58.1.1.1");
}

TEST(SPLIT_TEST, split_test)
{
    auto resultVector = split("", '.');
    ASSERT_EQ(resultVector.size(), 1);
    ASSERT_EQ(resultVector[0], "");

    resultVector = split("11", '.');
    ASSERT_EQ(resultVector.size(), 1);
    ASSERT_EQ(resultVector[0], "11");

    resultVector = split("..", '.');
    ASSERT_EQ(resultVector.size(), 3);
    ASSERT_EQ(resultVector[0], "");
    ASSERT_EQ(resultVector[1], "");
    ASSERT_EQ(resultVector[2], "");

    resultVector = split("11.", '.');
    ASSERT_EQ(resultVector.size(), 2);
    ASSERT_EQ(resultVector[0], "11");
    ASSERT_EQ(resultVector[1], "");

    resultVector = split(".11", '.');
    ASSERT_EQ(resultVector.size(), 2);
    ASSERT_EQ(resultVector[0], "");
    ASSERT_EQ(resultVector[1], "11");

    resultVector = split("11.22", '.');
    ASSERT_EQ(resultVector.size(), 2);
    ASSERT_EQ(resultVector[0], "11");
    ASSERT_EQ(resultVector[1], "22");
}
