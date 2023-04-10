#include <string>
#include "ip_printer.hpp"

#define UNUSED(variable) (void)variable

int main(int argc, char const *argv[])
{
    print_ip( int8_t{-1} ); // 255
    print_ip( int16_t{0} ); // 0.0
    print_ip( int32_t{2130706433} ); // 127.0.0.1
    print_ip( int64_t{8875824491850138409} );// 123.45.67.89.101.112.131.41
    UNUSED(argc);
    UNUSED(argv);
    return 0;
}
