//  (C) Copyright 2014 Alvaro J. Genial (http://alva.ro)
//  Use, modification and distribution are subject to the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef AJG_SYNTH_ENGINES_NULL_OPTIONS_HPP_INCLUDED
#define AJG_SYNTH_ENGINES_NULL_OPTIONS_HPP_INCLUDED

#include <ajg/synth/engines/base_options.hpp>

namespace ajg {
namespace synth {
namespace engines {
namespace null {

template <class Value>
struct options : base_options<Value> {};

}}}} // namespace ajg::synth::engines::null

#endif // AJG_SYNTH_ENGINES_NULL_OPTIONS_HPP_INCLUDED
