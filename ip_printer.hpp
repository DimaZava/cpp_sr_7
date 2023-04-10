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
#include <iostream>
#include <complex>

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

template <typename T>
void print_ip_common(T ip)
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

template <typename T, typename std::enable_if<std::is_same<int8_t, T>::value, int>::type = 0>
void print_ip(T ip)
{
    std::cout << "Calling int8_t" << std::endl;
    print_ip_common(ip);
}

template <typename T, typename std::enable_if<std::is_same<int16_t, T>::value, int>::type = 0>
void print_ip(T ip)
{
    std::cout << "Calling int16_t" << std::endl;
    print_ip_common(ip);
}

template <typename T, typename std::enable_if<std::is_same<int32_t, T>::value, int>::type = 0>
void print_ip(T ip)
{
    std::cout << "Calling int32_t" << std::endl;
    print_ip_common(ip);
}

template <typename T, typename std::enable_if<std::is_same<int64_t, T>::value, int>::type = 0>
void print_ip(T ip)
{
    std::cout << "Calling int64_t" << std::endl;
    print_ip_common(ip);
}

#endif /* ip_printer_hpp */
