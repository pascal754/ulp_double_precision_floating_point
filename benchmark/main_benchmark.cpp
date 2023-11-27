// by SM
// Nov. 2023

#include "pch.h"

import ieee754;

constexpr double value{ 3.14 };
//constexpr double value{ 1e-308 };
//constexpr double value{ 1e+308 };
//constexpr double value{ 0.0000459 };

void ulp_bit_cast(benchmark::State& s) {
    while (s.KeepRunning())
        get_ulp_bit_cast(value);
}
BENCHMARK(ulp_bit_cast);

void ulp_bit_cast_lookup_table(benchmark::State& s) {
    while (s.KeepRunning())
        get_ulp_bit_cast_lookup_table(value);
}
BENCHMARK(ulp_bit_cast_lookup_table);

void add_ulp(benchmark::State& s) {
    while (s.KeepRunning())
        auto x{ value + get_ulp_bit_cast_lookup_table(value) };
}
BENCHMARK(add_ulp);

void nextafter(benchmark::State& s) {
    while (s.KeepRunning())
        std::nextafter(value, INFINITY);
}
BENCHMARK(nextafter);

void test_add_upward(benchmark::State& s) {
    while (s.KeepRunning())
        add_upward(value, value);
}
BENCHMARK(test_add_upward);

void test_add_downward(benchmark::State& s) {
    while (s.KeepRunning())
        add_downward(value, value);
}
BENCHMARK(test_add_downward);

void test_fesetround(benchmark::State& s) {
    while (s.KeepRunning())
        std::fesetround(FE_TONEAREST);
}
BENCHMARK(test_fesetround);

BENCHMARK_MAIN();
