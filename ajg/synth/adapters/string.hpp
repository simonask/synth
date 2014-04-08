//  (C) Copyright 2014 Alvaro J. Genial (http://alva.ro)
//  Use, modification and distribution are subject to the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef AJG_SYNTH_ADAPTERS_STRING_HPP_INCLUDED
#define AJG_SYNTH_ADAPTERS_STRING_HPP_INCLUDED

#include <string>

#include <ajg/synth/adapters/adapter.hpp>

namespace ajg {
namespace synth {

//
// specialization for std::basic_string
////////////////////////////////////////////////////////////////////////////////////////////////////

template < class Traits
         , class StringTraits
         , class Allocator
         >
struct adapter<Traits, std::basic_string
        <typename Traits::char_type, StringTraits, Allocator> >
    : public abstract_adapter<Traits> {

    typedef std::basic_string< typename Traits::char_type
                             , StringTraits
                             , Allocator
                             >  basic_string_type;
    AJG_SYNTH_ADAPTER(basic_string_type);

  public:

    number_type  count() const { return traits_type::to_number(adapted_); }
    boolean_type test()  const { return !adapted_.empty(); }
    void input (istream_type& in)        { in  >> adapted_; }
    void output(ostream_type& out) const { out << adapted_; }

    const_iterator begin() const { return const_iterator(adapted_.begin()); }
    const_iterator end()   const { return const_iterator(adapted_.end()); }

    /*const*/ adapted_type/*&*/ adapted_;
};

//
// specializations for [const] char_type* and [const] char_type[N].
////////////////////////////////////////////////////////////////////////////////////////////////////

template <class Traits>
struct adapter<Traits, typename Traits::char_type const*>
    : public adapter<Traits, std::basic_string<typename Traits::char_type> > {
    adapter(typename Traits::char_type const *const value) :
        adapter<Traits, std::basic_string<typename Traits::char_type> >(value) {}
};

template <class Traits, std::size_t N>
struct adapter<Traits, typename Traits::char_type const[N]>
    : public adapter<Traits, std::basic_string<typename Traits::char_type> > {
    adapter(typename Traits::char_type const value[N]) :
        adapter<Traits, std::basic_string<typename Traits::char_type> >(
            std::basic_string<typename Traits::char_type>(value, N ? N - 1 : 0)) {}
};

template <class Traits>
struct adapter<Traits, typename Traits::char_type*>
    : public adapter<Traits, std::basic_string<typename Traits::char_type> > {
    adapter(typename Traits::char_type *const value) :
        adapter<Traits, std::basic_string<typename Traits::char_type> >(value) {}
};

template <class Traits, std::size_t N>
struct adapter<Traits, typename Traits::char_type[N]>
    : public adapter<Traits, std::basic_string<typename Traits::char_type> > {
    adapter(typename Traits::char_type const value[N]) :
        adapter<Traits, std::basic_string<typename Traits::char_type> >(
            std::basic_string<typename Traits::char_type>(value, N ? N - 1 : 0)) {}
};

#if defined(BOOST_MSVC)
    // MSVC seems to need these more specific specializations,
    // otherwise string literals specialize as char arrays.
    template <class Traits, std::size_t N>
    struct adapter<Traits, char const[N]>
        : public adapter<Traits, std::basic_string<char> > {
        adapter(char const value[N]) :
            adapter<Traits, std::basic_string<char> >(
                std::basic_string<char>(value, N ? N - 1 : 0)) {}
    };

    template <class Traits, std::size_t N>
    struct adapter<Traits, wchar_t const[N]>
        : public adapter<Traits, std::basic_string<wchar_t> > {
        adapter(wchar_t const value[N]) :
            adapter<Traits, std::basic_string<wchar_t> >(
                std::basic_string<wchar_t>(value, N ? N - 1 : 0)) {}
    };

#endif

}} // namespace ajg::synth

#endif // AJG_SYNTH_ADAPTERS_STRING_HPP_INCLUDED

