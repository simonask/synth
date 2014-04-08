//  (C) Copyright 2014 Alvaro J. Genial (http://alva.ro)
//  Use, modification and distribution are subject to the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef AJG_SYNTH_VALUE_FACADE_HPP_INCLUDED
#define AJG_SYNTH_VALUE_FACADE_HPP_INCLUDED

#include <cstddef>
#include <iterator>

#include <boost/none_t.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <ajg/synth/detail.hpp>
#include <ajg/synth/value_traits.hpp>
#include <ajg/synth/adapters/numeric.hpp>
#include <ajg/synth/adapters/abstract.hpp>

namespace ajg {
namespace synth {

template <class Char, class Adapted>
struct adapter;

using detail::integer_type;

//
// value_facade
////////////////////////////////////////////////////////////////////////////////////////////////////

template < class Char
         , class Value
         , class Traits = default_value_traits<Char, Value>
         >
struct value_facade {
  public:

    typedef value_facade             this_type;
    typedef Value                    value_type;
    typedef Traits                   traits_type;
    typedef abstract_adapter<Traits> abstract_type;

    typedef typename traits_type::none_type     none_type;
    typedef typename traits_type::boolean_type  boolean_type;
    typedef typename traits_type::size_type     size_type;
    typedef typename traits_type::char_type     char_type;
    typedef typename traits_type::range_type    range_type;
    typedef typename traits_type::string_type   string_type;
    typedef typename traits_type::number_type   number_type;
    typedef typename traits_type::datetime_type datetime_type;
    typedef typename traits_type::duration_type duration_type;
    typedef typename traits_type::istream_type  istream_type;
    typedef typename traits_type::ostream_type  ostream_type;

    typedef typename traits_type::iterator       iterator;
    typedef typename traits_type::const_iterator const_iterator;

  protected:

    // An uninitialized value
    value_facade() : adapter_() {}

    template <class T>
    value_facade(T const& t, typename boost::disable_if<boost::is_same<T, value_type> >::type* = 0)
        : adapter_(new synth::adapter<Traits, T>(t)) {}

    template <class T, class U>
    value_facade(T const& t, U const& u, typename boost::disable_if<boost::is_same<T, value_type> >::type* = 0)
        : adapter_(new synth::adapter<Traits, T>(t, u)) {}

    template <class T, class U, class V>
    value_facade(T const& t, U const& u, V const& v, typename boost::disable_if<boost::is_same<T, value_type> >::type* = 0)
        : adapter_(new synth::adapter<Traits, T>(t, u, v)) {}

  public:

    inline boolean_type initialized()  const { return boolean_type(adapter_); }
    inline void         uninitialize()       { adapter_.reset(); }

    template <class T> inline boolean_type is() const { return traits_type::adapter_traits::template is<T>(*this); }
    template <class T> inline T const&     as() const { return traits_type::adapter_traits::template as<T>(*this); }
    template <class T> inline T            to() const { return traits_type::adapter_traits::template to<T>(*this); }

  public: // TODO: Should only be visible to {default_}value_traits.


    // TODO: Figure out if type comparisons are reliable, otherwise defer to the adapters themselves or using adapter().as<...> != 0
    inline boolean_type typed_like (value_type const& that) const { return this->type() == that.type(); }
    inline boolean_type typed_equal(value_type const& that) const { return this->adapter().equal_adapted(that.adapter()); }
    inline boolean_type typed_less (value_type const& that) const { return this->adapter().less_adapted(that.adapter()); }
    template <class T>
    inline T const& typed_as() const { return this->adapter().template get_adapted<T>(); }

  public:

    inline std::type_info const& type() const { return this->adapter().type(); }

    // TODO: Defer all these to traits.

    inline boolean_type is_none()    const { return this->template is<none_type>(); }
    inline boolean_type is_boolean() const { return this->template is<boolean_type>(); }
    // TODO: Allow adapters to say they're adapting a numeric type even without deriving from abstract_numeric_adapter.
    inline boolean_type is_numeric() const { return this->adapter().template get<abstract_numeric_adapter<traits_type> >() != 0; }
    inline boolean_type is_string()  const { return this->template is<string_type>(); }
    inline boolean_type is_number()  const { return this->template is<number_type>(); }

    inline boolean_type  to_boolean()  const { return this->adapter().test(); }
    inline number_type   to_number()   const { return this->adapter().count(); }
    inline datetime_type to_datetime() const { return this->adapter().to_datetime(); }
    inline string_type   to_string()   const { return this->adapter().to_string(); }
    inline size_type     to_size()     const { return traits_type::to_size(*this); }

    inline size_type empty()  const { return this->size() == 0; }             // TODO: Defer to adapter.
    inline size_type size()   const { return std::distance(begin(), end()); } // TODO: Defer to adapter.

    inline value_type front() const { return *this->begin(); }                          // TODO: Defer to adapter.
    inline value_type back()  const { return *this->at(-1); } // return *--this->end(); // TODO: Defer to adapter.

    inline const_iterator       at   (value_type const& index) const { return detail::at(*this, static_cast<integer_type>(index.to_number())); } // TODO: Defer to adapter.
    inline const_iterator       find (value_type const& value) const { return this->adapter().find(value); }
    inline optional<value_type> index(value_type const& key)   const { return this->adapter().index(key); }

    // Even the non-const versions are immutable and are provided simply as a convenience.
    inline iterator begin() { return const_cast<this_type const*>(this)->begin(); }
    inline iterator end()   { return const_cast<this_type const*>(this)->end(); }

    inline const_iterator begin() const { return this->adapter().begin(); }
    inline const_iterator end()   const {
        try {
            return this->adapter().end();
        }
        // TODO: Investigate why this is needed.
        catch (bad_method const& method) {
            return method.name == "end" ? const_iterator() : throw;
        }
    }

    inline boolean_type is_iterable() const { // TODO: Defer to adapter.
        try {
            return this->size(), true; // Will throw if item cannot be iterated over.
        }
        catch (bad_method const& method) {
            return method.name == "size" ? false : throw;
        }
    }

    inline range_type slice( optional<integer_type> const lower = none
                           , optional<integer_type> const upper = none
                           ) const { // TODO: Defer to adapter.
        return detail::slice(*this, lower, upper);
    }

    inline boolean_type contains(value_type const& that) const { return traits_type::adapter_traits::contains(*this, that); }
    inline boolean_type equal   (value_type const& that) const { return traits_type::adapter_traits::equal(*this, that); }
    inline boolean_type less    (value_type const& that) const { return traits_type::adapter_traits::less(*this, that); }

    inline operator boolean_type()                          const { return this->to_boolean(); }
    inline boolean_type operator!()                         const { return !this->to_boolean(); }
    inline boolean_type operator==(value_type const& that)  const { return this->equal(that); }
    inline boolean_type operator!=(value_type const& that)  const { return !this->equal(that); }
    inline boolean_type operator< (value_type const& that)  const { return this->less(that); }
    inline boolean_type operator<=(value_type const& that)  const { return !that.less(*this); }
    inline boolean_type operator> (value_type const& that)  const { return that.less(*this); }
    inline boolean_type operator>=(value_type const& that)  const { return !this->less(*this); }
    inline value_type   operator[](value_type const& index) const { return *this->at(index); }

  protected:

    inline const abstract_type& adapter() const {
        if (!adapter_) {
            AJG_SYNTH_THROW(std::logic_error("uninitialized value"));
        }
        return *adapter_;
    }

  private:

    template <class V>                   friend struct value_iterator;
    template <class C, class V, class T> friend struct value_facade;

    friend ostream_type& operator <<(ostream_type& output, value_type const& value) {
        /* if (!value.initialized()) {
            return output << traits_type::literal("<uninitialized>");
        }
        else */ {
            return value.adapter().output(output), output;
        }
    }

    friend istream_type& operator >>(istream_type& input, value_type& value) {
        return value.adapter().input(input), input;
    }

  private:

    boost::shared_ptr<abstract_type const> adapter_;
};

// TODO[c++11]: Replace with variadic templates.
#define AJG_SYNTH_VALUE_CONSTRUCTORS(name, base, rest) \
    name() : base() rest /* TODO: Remove this, and change all map[] access with insert().*/ \
    template <class T>                   name(T const& t) : base(t) rest \
    template <class T, class U>          name(T const& t, U const& u) : base(t, u) rest \
    template <class T, class U, class V> name(T const& t, U const& u, V const& v) : base(t, u, v) rest

}} // namespace ajg::synth

#endif // AJG_SYNTH_VALUE_FACADE_HPP_INCLUDED
