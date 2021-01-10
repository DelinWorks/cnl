
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_CONVERT_OPERATOR_H)
#define CNL_IMPL_SCALED_CONVERT_OPERATOR_H

#include "../../fraction.h"
#include "../../limits.h"
#include "../num_traits/fixed_width_scale.h"
#include "../num_traits/scale.h"
#include "../operators/native_tag.h"
#include "../power_value.h"
#include "../scaled_integer/declaration.h"
#include "is_same_tag_family.h"
#include "power.h"

/// compositional numeric library
namespace cnl {
    // integer -> floating
    template<int DestExponent, int SrcExponent, int Radix, _impl::floating_point Dest, _impl::integer Src>
    struct convert_operator<power<DestExponent, Radix>, power<SrcExponent, Radix>, Dest, Src> {
        CNL_NODISCARD constexpr Dest operator()(Src const& from) const
        {
            return Dest(from) * _impl::power_value<Dest, SrcExponent - DestExponent, Radix>();
        }
    };

    // floating -> integer
    template<int DestExponent, int SrcExponent, int Radix, _impl::integer Result, _impl::floating_point Input>
    struct convert_operator<power<DestExponent, Radix>, power<SrcExponent, Radix>, Result, Input> {
        CNL_NODISCARD constexpr Result operator()(Input const& from) const
        {
            return static_cast<Result>(
                    from * _impl::power_value<Input, SrcExponent - DestExponent, Radix>());
        }
    };

    // integer -> integer
    template<int DestExponent, int SrcExponent, int Radix, _impl::integer Result, _impl::integer Input>
    struct convert_operator<
            power<DestExponent, Radix>, power<SrcExponent, Radix>, Result, Input> {
        CNL_NODISCARD constexpr Result operator()(Input const& from) const
        {
            // when converting *from* scaled_integer
            return static_cast<Result>(_impl::scale<SrcExponent - DestExponent, Radix>(
                    _impl::from_value<Result>(from)));
        }
    };

    // shims between equivalent tags
    template<int DestExponent, int DestRadix, typename Result, typename Input>
    struct convert_operator<power<DestExponent, DestRadix>, _impl::native_tag, Result, Input>
        : convert_operator<power<DestExponent, DestRadix>, power<0, DestRadix>, Result, Input> {
    };

    template<int SrcExponent, int SrcRadix, typename Result, typename Input>
    struct convert_operator<_impl::native_tag, power<SrcExponent, SrcRadix>, Result, Input>
        : convert_operator<power<0, SrcRadix>, power<SrcExponent, SrcRadix>, Result, Input> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // conversion from fraction

    namespace _impl {
        template<typename Number>
        CNL_NODISCARD constexpr Number not_scaled_integer(Number const& number)
        {
            return number;
        }

        template<typename Rep, int Exponent, int Radix>
        CNL_NODISCARD constexpr Rep not_scaled_integer(
                scaled_integer<Rep, power<Exponent, Radix>> const& f)
        {
            return _impl::to_rep(f);
        }

        template<typename Number>
        struct exponent : constant<0> {
        };

        template<typename Rep, int Exponent, int Radix>
        struct exponent<scaled_integer<Rep, power<Exponent, Radix>>> : constant<Exponent> {
        };

        template<class Quotient, class Dividend, class Divisor>
        struct exponent_shift
            : std::integral_constant<
                      int, _impl::exponent<Dividend>::value - _impl::exponent<Divisor>::value
                                   - _impl::exponent<Quotient>::value> {
        };
    }

    template<
            int DestExponent, int Radix, typename Dest, typename SrcNumerator,
            typename SrcDenominator>
    struct convert_operator<
            cnl::power<DestExponent, Radix>, cnl::power<0, Radix>, Dest,
            cnl::fraction<SrcNumerator, SrcDenominator>> {
        CNL_NODISCARD constexpr Dest operator()(
                cnl::fraction<SrcNumerator, SrcDenominator> const& from) const
        {
            static_assert(_impl::exponent<Dest>::value == 0, "TODO");

            return static_cast<Dest>(
                    _impl::fixed_width_scale<
                            _impl::exponent<SrcNumerator>::value
                                    - _impl::exponent<SrcDenominator>::value - DestExponent,
                            Radix>(static_cast<Dest>(_impl::not_scaled_integer(from.numerator)))
                    / _impl::not_scaled_integer(from.denominator));
        }
    };
}

#endif  // CNL_IMPL_SCALED_CONVERT_OPERATOR_H
