//
//  ip_printer.cpp
//  main
//
//  Created by Dmitrii Zavadskii on 29.03.2023.
//

#include <stdio.h>
#include <type_traits>
#include <vector>
#include <list>
#include <iostream>
#include <complex>
#include <experimental/type_traits>
#include <numeric>
#include <tuple>
#include <utility>
#include <tuple>
#include <algorithm>

// Generalized arithmetic based types ip print

/// Split arithmetic value to bytes.
/// - Parameter value: Value to split.
template <typename T,
typename std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
std::vector<std::bitset<sizeof(T) * 8>> split_arithmetic_value_to_bytes(T const& value)
{
    const size_t bits_count = sizeof(T) * 8;
    std::vector<std::bitset<bits_count>> bits;
    
    for (size_t i = 0; i < sizeof(T); i++)
    {
        const unsigned long offset = i * 8;
        const unsigned long bit_value = (value >> offset) & ((1 << 8) - 1);
        // (source >> offset) & ((1 << n) - 1)
        // get n continuous bits from source starting at the offset bit.
        bits.push_back(std::bitset<bits_count>{bit_value});
    }
        
    return bits;
}

/// Convert bitset to string.
/// - Parameter bitset: Bitset to convert.
template <typename T>
std::string convert_bitset_to_str(std::bitset<sizeof(T) * 8> bitset)
{
    int result = 0;
    for (int i = 0; i < (int)bitset.size(); ++i)
    {
        result += bitset[i] * std::pow(2, i);
    }
    
    return std::to_string(result);
}

void test()
{
}

/// Print arithmetic "ip" address.
/// - Parameter ip: Address to print.
template <typename T, typename std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
void print_ip_arithmetic(T ip)
{
    auto bytes_vector = split_arithmetic_value_to_bytes(ip);
    std::vector<std::string> ip_string_vector;
    for (auto it = bytes_vector.rbegin(); it != bytes_vector.rend(); ++it)
    {
        auto bitset = *it;
        auto ip_part_string = convert_bitset_to_str<T>(bitset);
        ip_string_vector.push_back(ip_part_string);
    }
    
    if (ip_string_vector.begin() != ip_string_vector.end())
    {
        std::ostringstream output;
        output << *ip_string_vector.begin();
        for (auto it = ip_string_vector.begin() + 1; it != ip_string_vector.end(); ++it)
        {
            output << "." << *it;
        }
        std::cout << output.str() << std::endl;
    }
    
    // auto bytes = reinterpret_cast<const unsigned char>(&ip);
    // auto end = bytes + sizeof(T);
    //
    // std::copy(
    //           std::make_reverse_iterator(end),
    //           std::make_reverse_iterator(bytes + 1),
    //           std::ostream_iterator<unsigned>(std::cout, ".")
    //           );
    // std::cout << unsigned(*bytes) << std::endl;
};

/// SFINAE print_ip for integral values.
/// - Parameter ip: ip address.
template <typename T,
typename std::enable_if_t<std::is_integral<T>::value, bool> = true
>
void print_ip(T ip)
{
    // std::cout << "Calling int8_t" << std::endl;
    print_ip_arithmetic(ip);
}

// Generalized container based types ip print

template <typename T> struct is_container : std::false_type {};

template <typename... Ts>
struct is_container<std::vector<Ts...>> : std::true_type {};
template <typename... Ts>
struct is_container<std::list<Ts...>> : std::true_type {};

/// Print "ip" container. Accepts iteratable entity.
/// - Parameter ip: Container to print.
template<typename T,
typename std::enable_if_t<is_container<T>::value, bool> = true
>
void print_ip_container(T ip)
{
    if (ip.begin() != ip.end())
    {
        std::ostringstream out;
        auto iterator = ip.begin();
        out << *iterator;
        advance(iterator, 1);
        
        for (; iterator != ip.end(); ++iterator)
        {
            out << "." << *iterator;
        }
        std::cout << out.str() << std::endl;
    }
}

/// SFINAE print_ip for vector<int>.
/// - Parameter ip: ip address.
template <typename T,
typename std::enable_if_t<std::is_same<std::vector<int>, T>::value, bool> = true
>
void print_ip(T ip)
{
    // std::cout << "Calling std::vector<int>" << std::endl;
    print_ip_container(ip);
}

/// SFINAE print_ip for list<short>.
/// - Parameter ip: ip address.
template <typename T,
typename std::enable_if_t<std::is_same<std::list<short>, T>::value, bool> = true
>
void print_ip(T ip)
{
    // std::cout << "Calling std::list<short>" << std::endl;
    print_ip_container(ip);
}

// Custom implementations

/// SFINAE print_ip for string.
/// - Parameter ip: ip address.
template <typename T,
typename std::enable_if_t<std::is_same<std::string, T>::value, int> = 0
>
void print_ip(T ip)
{
    // std::cout << "Calling std::string" << std::endl;
    std::cout << ip << std::endl;
}

template <typename> struct is_tuple: std::false_type {};
template <typename ...is_tuple_type> struct is_tuple<std::tuple<is_tuple_type...>>: std::true_type {};

/// Prints tuple. Performs in runtime.
/// - Parameter tuple: Tuple to print.
template<class T, size_t... I>
void print_indexed_sequence(const T& tuple, std::index_sequence<I...>)
{
    // C++ c'mon, why can't I just iterate over the tuple's contents in more obvious way???
    (..., (std::cout << (I == 0 ? "" : ".") << std::get<I>(tuple)));
    std::cout << std::endl;
}

/// Unpack tuple to print it using runtime mechanisms.
/// - Parameter tuple: Tuple to print.
template <typename T,
typename std::enable_if_t<is_tuple<T>::value, bool> = true
>
void print(const T& tuple)
{
    constexpr std::size_t tuple_size = std::tuple_size<T>{};
    const std::integer_sequence indices = std::make_index_sequence<tuple_size>{};
    print_indexed_sequence(tuple, indices);
}

/// SFINAE print_ip for tuples.
/// - Parameter ip: ip address.
template <typename T,
typename std::enable_if_t<is_tuple<T>::value, bool> = true
>
void print_ip(T ip)
{
    // std::cout << "Calling std::tuple" << std::endl;
    print(ip);
}

//template<typename T, typename... O>
//constexpr bool are_same() {
//    bool b = true;
//    int arr[] = { (b = b && std::is_same_v<T, O>, 0)... };
//    return b;
//}
//
//template <
//typename T,
//typename... Ts,
//typename std::enable_if_t<std::conjunction_v<std::is_same<T, Ts>...>>
////typename std::enable_if_t<are_same<T, Ts...>()>
//>
//void print_ip(T x, Ts... xs)
//{
//    print(x, xs...);
//}
