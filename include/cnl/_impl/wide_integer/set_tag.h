
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_SET_TAG_H)
#define CNL_IMPL_WIDE_INTEGER_SET_TAG_H

#include "../num_traits/set_tag.h"
#include "../type_traits/enable_if.h"
#include "../wide_tag/is_wide_tag.h"
#include "definition.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<typename NumberRep, class NumberTag, tag Tag>
    struct set_tag<
            _impl::wrapper<NumberRep, NumberTag>, Tag,
            _impl::enable_if_t<_impl::is_wide_tag<Tag>::value>>
        : std::type_identity<_impl::wrapper<typename Tag::rep, Tag>> {
    };
}

#endif  // CNL_IMPL_WIDE_INTEGER_SET_TAG_H
