// by SM
// Nov. 2023

module;

#include "pch.h"

export module ieee754;

// Intel i5 12th Gen, Windows 11, MSVC 17.9.0 Preview 1.0
// little endianness
static_assert(std::endian::native == std::endian::little);

export struct DoubleBitfields
{
    std::uint64_t mantissa : 52; // LSB
    std::uint64_t exponent : 11;
    std::uint64_t sign : 1; // MSB
};
// end of little endianness

static_assert(sizeof(DoubleBitfields) == sizeof(double));

static_assert(std::bit_cast<uint64_t>(std::numeric_limits<double>::denorm_min()) == 1);

export double get_ulp_bit_cast(double x)
{
    if (std::isnan(x) || std::isinf(x))
        return std::nan("");

    auto bits{ std::bit_cast<std::uint64_t>(x) };
    bits >>= 52;
    bits &= 0b0111'1111'1111; // get exponent
    if (bits > 52)
    {
        bits -= 52;
        bits <<= 52;
        return std::bit_cast<double>(bits);
    }

    if (bits == 0 || bits == 1)
    {
        return std::numeric_limits<double>::denorm_min();
    }

    // 2 <= exponent <= 52
    --bits;
    std::uint64_t ulp{ 1 };
    ulp <<= bits;

    return std::bit_cast<double>(ulp);
}

export constexpr std::array<double, 53> make_ulp_lookup_table()
{
    std::array<double, 53> ulps;
    std::uint64_t bits{ 1 };
    ulps[0] = ulps[1] = std::bit_cast<double>(bits);
    for (auto i{ 2u }; i < ulps.size(); ++i)
    {
        bits = 1u;
        bits <<= (i - 1);
        ulps[i] = std::bit_cast<double>(bits);
    }

    return ulps;
}

constexpr static auto ULPS{ make_ulp_lookup_table() };

export double get_ulp_bit_cast_lookup_table(double x)
{
    if (std::isnan(x) || std::isinf(x))
        return std::nan("");

    auto bits{ std::bit_cast<std::uint64_t>(x) };
    bits >>= 52;
    bits &= 0b0111'1111'1111; // get exponent
    if (bits > 52)
    {
        bits -= 52;
        bits <<= 52;
        return std::bit_cast<double>(bits);
    }

    return ULPS[bits];
}

export double add_upward(double x1, double x2)
{
    std::fesetround(FE_UPWARD);
    auto sum{ x1 + x2 };
    std::fesetround(FE_TONEAREST);
    return sum;
}

export double add_downward(double x1, double x2)
{
    std::fesetround(FE_DOWNWARD);
    auto sum{ x1 + x2 };
    std::fesetround(FE_TONEAREST);
    return sum;
}
