// by SM
// Nov. 2023

#include "pch.h"
import ieee754;

int main()
{
    auto print_format = [](double x) {
        auto d{ std::bit_cast<DoubleBitfields>(x) };
        std::println("{} {:011b} {:052b}, {}", +d.sign, +d.exponent, +d.mantissa, x);
        };

    auto compare_ulp_nextafter = [&](std::span<double> values) {
        for (auto x : values)
        {
            print_format(x);
            auto ulp{ get_ulp_bit_cast_lookup_table(x) };
            print_format(ulp);
            auto builtin{ std::nextafter(x, INFINITY) - x };
            print_format(builtin);
            std::println("have the same values: {}", ulp == builtin);
            std::println("");
        }};

    auto ulps{ make_ulp_lookup_table() };

    compare_ulp_nextafter(ulps);

    std::array test_values{
        0.0,
        -0.0,
        -1.25,
        -1.25e-100,
        -1.25e-200,
        -1.25e-285,
        -1.25e-295,
        3.14,
        3.14e+100,
        3.14e+200
    };

    compare_ulp_nextafter(test_values);

    double d1{ 1.25 };
    double d2{ 3.14 };
    double sum{ d1 + d2 };
    print_format(sum);
    print_format(sum + get_ulp_bit_cast_lookup_table(sum));
    print_format(sum - get_ulp_bit_cast_lookup_table(sum));
    print_format(std::nextafter(sum, INFINITY));
    print_format(std::nextafter(sum, -INFINITY));
    print_format(add_upward(d1, d2));
    print_format(add_downward(d1, d2));
    /*
    Output:
    0 10000000001 0001100011110101110000101000111101011100001010010000, 4.390000000000001
    0 10000000001 0001100011110101110000101000111101011100001010010001, 4.3900000000000015
    0 10000000001 0001100011110101110000101000111101011100001010001111, 4.39
    0 10000000001 0001100011110101110000101000111101011100001010010001, 4.3900000000000015
    0 10000000001 0001100011110101110000101000111101011100001010001111, 4.39
    0 10000000001 0001100011110101110000101000111101011100001010010000, 4.390000000000001
    0 10000000001 0001100011110101110000101000111101011100001010010000, 4.390000000000001
    */
}