//
//  ip_printer.hpp
//  main
//
//  Created by Dmitrii Zavadskii on 29.03.2023.
//

#ifndef ip_printer_hpp
#define ip_printer_hpp

#include <stdio.h>
#include <type_traits>
#include <vector>
#include <list>
#include <iostream>
#include <complex>
#include <experimental/type_traits>

// Generalized arithmetic based types ip print

template <typename T,
typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
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

template <typename T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
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
};

template <typename T,
typename std::enable_if_t<std::is_same<int8_t, T>::value, bool> = true
>
void print_ip(T ip)
{
    // std::cout << "Calling int8_t" << std::endl;
    print_ip_arithmetic(ip);
}

template <typename T,
typename std::enable_if_t<std::is_same<int16_t, T>::value, bool> = true
>
void print_ip(T ip)
{
    // std::cout << "Calling int16_t" << std::endl;
    print_ip_arithmetic(ip);
}

template <typename T,
typename std::enable_if_t<std::is_same<int32_t, T>::value, bool> = true
>
void print_ip(T ip)
{
    // std::cout << "Calling int32_t" << std::endl;
    print_ip_arithmetic(ip);
}

template <typename T,
typename std::enable_if_t<std::is_same<int64_t, T>::value, bool> = true
>
void print_ip(T ip)
{
    // std::cout << "Calling int64_t" << std::endl;
    print_ip_arithmetic(ip);
}

// Generalized container based types ip print

template<typename T, typename _ = void>
struct is_container : std::false_type {};

template<typename T>
struct is_container<
        T,
        std::void_t<
                typename T::value_type,
                typename T::size_type,
                typename T::allocator_type,
                typename T::iterator,
                typename T::const_iterator,
                decltype(std::declval<T>().size()),
                decltype(std::declval<T>().begin()),
                decltype(std::declval<T>().end()),
                decltype(std::declval<T>().cbegin()),
                decltype(std::declval<T>().cend())>
        > : std::true_type {};


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

template <typename T,
typename std::enable_if_t<std::is_same<std::vector<int>, T>::value, bool> = true
>
void print_ip(T ip)
{
    // std::cout << "Calling std::vector<int>" << std::endl;
    print_ip_container(ip);
}

template <typename T,
typename std::enable_if_t<std::is_same<std::list<short>, T>::value, bool> = true
>
void print_ip(T ip)
{
    // std::cout << "Calling std::list<short>" << std::endl;
    print_ip_container(ip);
}

// Custom implementations

template <typename T,
typename std::enable_if_t<std::is_same<std::string, T>::value, int> = 0
>
void print_ip(T ip)
{
    // std::cout << "Calling std::string" << std::endl;
    std::cout << ip << std::endl;
}

// Most of this awful tuple stuff is stolen from stackoverflow
// since there's absolutely no even some kind of simple way just to print it!!!
// std::get<index> is a template based function, so one should be generated at the build time
// that's why there's no other way than just to make it like this :(
template <typename> struct is_tuple: std::false_type {};
template <typename ...T> struct is_tuple<std::tuple<T...>>: std::true_type {};

template<class TupType, size_t... I>
void print(const TupType& _tup, std::index_sequence<I...>)
{
    (..., (std::cout << (I == 0 ? "" : ".") << std::get<I>(_tup)));
    std::cout << std::endl;
}

template<class... T>
void print (const std::tuple<T...>& _tup)
{
    print(_tup, std::make_index_sequence<sizeof...(T)>());
}

template <typename T,
typename std::enable_if_t<is_tuple<T>::value, bool> = 0
>
void print_ip(T ip)
{
    // std::cout << "Calling std::string" << std::endl;
    print(ip);
}

#endif /* ip_printer_hpp */
