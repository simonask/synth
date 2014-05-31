//  (C) Copyright 2014 Alvaro J. Genial (http://alva.ro)
//  Use, modification and distribution are subject to the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef AJG_SYNTH_ENGINES_DJANGO_BUILTIN_TAGS_HPP_INCLUDED
#define AJG_SYNTH_ENGINES_DJANGO_BUILTIN_TAGS_HPP_INCLUDED

#include <map>
#include <string>
#include <locale>
#include <sstream>
#include <iterator>
#include <stdexcept>

#include <boost/cstdint.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

#include <ajg/synth/exceptions.hpp>
#include <ajg/synth/adapters/map.hpp>
#include <ajg/synth/adapters/bool.hpp>
#include <ajg/synth/adapters/string.hpp>
#include <ajg/synth/adapters/vector.hpp>
#include <ajg/synth/adapters/numeric.hpp>
#include <ajg/synth/adapters/utility.hpp>
#include <ajg/synth/adapters/variant.hpp>
#include <ajg/synth/detail/filesystem.hpp>
#include <ajg/synth/detail/text.hpp>
#include <ajg/synth/engines/django/formatter.hpp>

namespace ajg {
namespace synth {
namespace engines {
namespace django {

x::mark_tag const id(9);

template <class Kernel>
struct builtin_tags {
  private:

    typedef Kernel                                                              kernel_type;
    typedef typename kernel_type::id_type                                       id_type;
    typedef typename kernel_type::regex_type                                    regex_type;
    typedef typename kernel_type::match_type                                    match_type;
    typedef typename kernel_type::sub_match_type                                sub_match_type;
    typedef typename kernel_type::engine_type                                   engine_type;
    typedef typename kernel_type::result_type                                   result_type;
    typedef typename kernel_type::state_type                                    state_type;
    typedef typename kernel_type::range_type                                    range_type;
    typedef typename kernel_type::iterator_type                                 iterator_type;

    typedef typename engine_type::context_type                                  context_type;
    typedef typename engine_type::options_type                                  options_type;
    typedef typename engine_type::value_type                                    value_type;

    typedef typename value_type::behavior_type                                  behavior_type;
 // typedef typename value_type::range_type                                     range_type;
    typedef typename value_type::sequence_type                                  sequence_type;
    typedef typename value_type::mapping_type                                   mapping_type;
    typedef typename value_type::traits_type                                    traits_type;

    typedef typename traits_type::boolean_type                                  boolean_type;
    typedef typename traits_type::char_type                                     char_type;
    typedef typename traits_type::size_type                                     size_type;
    typedef typename traits_type::floating_type                                 floating_type;
    typedef typename traits_type::datetime_type                                 datetime_type;
    typedef typename traits_type::path_type                                     path_type;
    typedef typename traits_type::names_type                                    names_type;
    typedef typename traits_type::symbols_type                                  symbols_type;
    typedef typename traits_type::string_type                                   string_type;
    typedef typename traits_type::ostream_type                                  ostream_type;

    typedef detail::text<string_type>                                           text;

  public:

    typedef void (*tag_type)( kernel_type  const& kernel
                            , result_type  const& result
                            , match_type   const& match
                            , context_type&       context
                            , ostream_type&       ostream
                            );

  private:

    typedef std::map<id_type, tag_type>                                         tags_type;
    typedef std::basic_ostringstream<char_type>                                 string_stream_type;
    typedef formatter<options_type>                                             formatter_type;
    typedef typename context_type::blocks_type                                  blocks_type;
    typedef typename options_type::renderer_type                                renderer_type;
    typedef typename options_type::renderers_type                               renderers_type;
    typedef typename options_type::entry_type                                   entry_type;
    typedef typename options_type::entries_type                                 entries_type;
    typedef typename options_type::segment_type                                 segment_type;
    typedef typename options_type::segments_type                                segments_type;
    typedef typename options_type::arguments_type                               arguments_type;
    typedef typename arguments_type::second_type::value_type                    named_argument_type;

  public:

    inline void initialize(kernel_type& kernel) {
        kernel.tag
            = add(kernel, autoescape_tag::syntax(kernel),        autoescape_tag::render)
            | add(kernel, block_tag::syntax(kernel),             block_tag::render)
            | add(kernel, comment_tag::syntax(kernel),           comment_tag::render)
            | add(kernel, csrf_token_tag::syntax(kernel),        csrf_token_tag::render)
            | add(kernel, cycle_tag::syntax(kernel),             cycle_tag::render)
            | add(kernel, cycle_as_tag::syntax(kernel),          cycle_as_tag::render)
            | add(kernel, cycle_as_silent_tag::syntax(kernel),   cycle_as_silent_tag::render)
            | add(kernel, debug_tag::syntax(kernel),             debug_tag::render)
            | add(kernel, extends_tag::syntax(kernel),           extends_tag::render)
            | add(kernel, filter_tag::syntax(kernel),            filter_tag::render)
            | add(kernel, firstof_tag::syntax(kernel),           firstof_tag::render)
            | add(kernel, for_tag::syntax(kernel),               for_tag::render)
            | add(kernel, for_empty_tag::syntax(kernel),         for_empty_tag::render)
            | add(kernel, if_tag::syntax(kernel),                if_tag::render)
            | add(kernel, ifchanged_tag::syntax(kernel),         ifchanged_tag::render)
            | add(kernel, ifequal_tag::syntax(kernel),           ifequal_tag::render)
            | add(kernel, ifnotequal_tag::syntax(kernel),        ifnotequal_tag::render)
            | add(kernel, include_tag::syntax(kernel),           include_tag::render)
            | add(kernel, include_with_tag::syntax(kernel),      include_with_tag::render)
            | add(kernel, include_with_only_tag::syntax(kernel), include_with_only_tag::render)
            | add(kernel, load_tag::syntax(kernel),              load_tag::render)
            | add(kernel, load_from_tag::syntax(kernel),         load_from_tag::render)
            | add(kernel, now_tag::syntax(kernel),               now_tag::render)
            | add(kernel, regroup_tag::syntax(kernel),           regroup_tag::render)
            | add(kernel, spaceless_tag::syntax(kernel),         spaceless_tag::render)
            | add(kernel, ssi_tag::syntax(kernel),               ssi_tag::render)
            | add(kernel, templatetag_tag::syntax(kernel),       templatetag_tag::render)
            | add(kernel, url_tag::syntax(kernel),               url_tag::render)
            | add(kernel, url_as_tag::syntax(kernel),            url_as_tag::render)
            | add(kernel, variable_tag::syntax(kernel),          variable_tag::render)
            | add(kernel, verbatim_tag::syntax(kernel),          verbatim_tag::render)
            | add(kernel, widthratio_tag::syntax(kernel),        widthratio_tag::render)
            | add(kernel, with_tag::syntax(kernel),              with_tag::render)
            | add(kernel, library_tag::syntax(kernel),           library_tag::render)
            ;
    }

  private:

    inline regex_type const& add(kernel_type& kernel, regex_type const& regex, tag_type const tag) {
        tags_[regex.regex_id()] = tag;
        return regex;
    }

    tags_type tags_;

  public:

//
// get
////////////////////////////////////////////////////////////////////////////////////////////////////

    inline tag_type get(id_type const id) const {
        typename tags_type::const_iterator it = tags_.find(id);
        return it == tags_.end() ? 0 : it->second;
    }

// TODO[c++11]: Replace with function.
#define TAG(content) kernel.block_open >> *_s >> content >> *_s >> kernel.block_close

//
// autoescape_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct autoescape_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("autoescape") >> kernel.name) >> kernel.block
                >> TAG(kernel.reserved("endautoescape"));
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            string_type const& setting  = match(kernel.name)[id].str();
            match_type  const& block    = match(kernel.block);
            boolean_type const previous = context.autoescape_;

                 if (setting == text::literal("on"))  context.autoescape_ = true;
            else if (setting == text::literal("off")) context.autoescape_ = false;
            else AJG_SYNTH_THROW(std::invalid_argument("setting"));

            kernel.render_block(ostream, result, block, context);
            context.autoescape_ = previous;
        }
    };

//
// block_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct block_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("block") >> kernel.name) >> kernel.block
                >> TAG(kernel.reserved("endblock") >> !kernel.name);
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            match_type  const& block = match(kernel.block);
            match_type  const& open  = match(kernel.name, 0);
            match_type  const& close = match(kernel.name, 1);
            string_type const  name  = open[id].str();

            if (close && name != close[id].str()) {
                std::string const original = text::narrow(name);
                AJG_SYNTH_THROW(std::invalid_argument("mismatched endblock tag for " + original));
            }

            if (context.top_level()) { // The block is being rendered directly.
                kernel.render_block(ostream, result, block, context);
                return;
            } // Else, the block is being derived from instead:

            if (optional<string_type> const& derived = context.get_block(name)) {
                ostream << *derived; // The base template is being rendered with derived blocks.
            }

            string_stream_type ss;
            { // TODO: Either make a context copy or make this strongly exception safe:
                string_type const previous_block = context.base_block_;
                context.base_block_ = name;
                kernel.render_block(ss, result, block, context);
                context.base_block_ = previous_block;
            }
            (*context.blocks_)[name] = ss.str();
        }
    };

//
// comment_tag
//     TODO: Split into comment_short_tag and comment_long_tag.
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct comment_tag {
        static regex_type syntax(kernel_type& kernel) {
            // Short form (without a closing token or newlines) or long form, below.
            return kernel.comment_open >> *(~x::before(kernel.comment_close | _n) >> _) >> kernel.comment_close
                | TAG(kernel.reserved("comment")) >> kernel.block >> TAG(kernel.reserved("endcomment"));
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            // Do nothing.
        }
    };

//
// csrf_token_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct csrf_token_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("csrf_token"));
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {

            if (optional<value_type> const& token = detail::find(text::literal("csrf_token"), context)) {
                string_type const& s = text::escape_entities(token->to_string());

                if (s != text::literal("NOTPROVIDED")) {
                    ostream << "<div style='display:none'>";
                    ostream << "<input type='hidden' name='csrfmiddlewaretoken' value='" << s << "' />";
                    ostream << "</div>";
                }
            }
        }
    };

//
// cycle_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct cycle_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("cycle") >> kernel.values >>
                !(kernel.keyword("as") >> kernel.name >> !(s1 = kernel.keyword("silent")))) >> kernel.block;
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            match_type  const& vals     = match(kernel.values);
            match_type  const& block    = match(kernel.block);
            match_type  const& name     = match(kernel.name);
            size_type   const  position = match.position();
            size_type   const  total    = vals.nested_results().size();
            size_type   const  current  = detail::find(position, context.cycles_).get_value_or(0);

            match_type const& val   = *detail::advance_to(vals.nested_results().begin(), current);
            value_type const  value = kernel.evaluate(result, val, context);
            context.cycles_[position] = (current + 1) % total;

            if (!name) { // e.g. cycle foo
                ostream << value;
                kernel.render_block(ostream, result, block, context);
            }
            else { // e.g. cycle foo as bar {silent}
                boolean_type const silent = match[s1].matched;
                if (!silent) ostream << value;

                context_type context_copy = context;
                context_copy[name[id].str()] = value;
                kernel.render_block(ostream, result, block, context_copy);
            }
        }
    };

//
// cycle_as_tag
//     NOTE: This is a no-op, since the as clause is already handled by cycle_tag.
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct cycle_as_tag {
        static regex_type syntax(kernel_type& kernel) { return kernel.nothing; }
        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {}
    };

//
// cycle_as_silent_tag
//     NOTE: This is a no-op, since the as ... silent clause is already handled by cycle_tag.
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct cycle_as_silent_tag {
        static regex_type syntax(kernel_type& kernel) { return kernel.nothing; }
        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {}
    };

//
// debug_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct debug_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("debug"));
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            ostream << "<h1>Context:</h1>" << std::endl;

            BOOST_FOREACH(typename context_type::value_type const& p, context) {
                ostream << "    " << value_type(p.first).escape()
                    << " = "  << value_type(p.second).escape() << "<br />" << std::endl;
            }
        }
    };

//
// extends_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct extends_tag {
        static regex_type syntax(kernel_type& kernel) {
            // TODO: Handle non-string literals bases, which should be treated not as paths but as
            //       template objects.
            return TAG(kernel.reserved("extends") >> kernel.string_literal) >> kernel.block;
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            match_type const& body = match(kernel.block);
            path_type  const  path = kernel.extract_path(match(kernel.string_literal));

            ostream_type null_stream(0);
            blocks_type blocks;
            context.blocks_ = &blocks;

            // First, render the base template as if it were stand-alone, so that block.super is
            // available to the derived template; non-block content is discarded.
            kernel.render_path(null_stream, result, path, context/*_copy*/);

            // context_copy = context; // Discard non-block modifications.
            // context_copy.blocks_ = &blocks;

            // Second, render any blocks in the derived template, while making the base template's
            // versions available to the derivee as block.super; non-block content is discarded.
            kernel.render_block(null_stream, result, body, context/*_copy*/);

            // context_copy = context; // Discard non-block modifications.
            // context_copy.blocks_ = &blocks;

            // Third, render the base template again with any (possibly) overridden blocks.
            // TODO: Parse and generate the result once and reuse it or have render_path do caching.
            kernel.render_path(ostream, result, path, context/*_copy*/);

            context.blocks_ = 0;
        }
    };

//
// firstof_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct firstof_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("firstof") >> kernel.values >> !kernel.string_literal);
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            match_type const& vals = match(kernel.values);

            BOOST_FOREACH(match_type const& val, kernel.select_nested(vals, kernel.value)) {
                try {
                    if (value_type const value = kernel.evaluate(result, val, context)) {
                        ostream << value;
                        break;
                    }
                }
                catch (missing_variable const&) {
                    // Missing variables count as 'False' ones.
                }
            }

            // Use the fallback, if there is one.
            if (match_type const& fallback = match(kernel.string_literal)) {
                ostream << kernel.extract_string(fallback);
            }
        }
    };

//
// filter_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct filter_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("filter") >> kernel.filters) >> kernel.block
                >> TAG(kernel.reserved("endfilter"));
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            string_stream_type ss;
            kernel.render_block(ss, result, match(kernel.block), context);
            ostream << kernel.apply_filters(ss.str(), result, match(kernel.filters), context);
        }
    };

//
// for_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct for_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("for") >> kernel.variable_names >> kernel.keyword("in")
                              >> kernel.value >> !(s1 = kernel.keyword("reversed"))) >> kernel.block
              >> !(TAG(kernel.reserved("empty")) >> kernel.block)
                >> TAG(kernel.reserved("endfor"));
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            match_type   const& vars     = match(kernel.variable_names);
            match_type   const& for_     = match(kernel.block, 0);
            match_type   const& empty    = match(kernel.block, 1);
            boolean_type const  reversed = match[s1].matched;
            value_type          value    = kernel.evaluate(result, match(kernel.value), context);

            if (reversed) {
                value = value.reverse();
            }
        #if AJG_SYNTH_IS_COMPILER_GCC
            else {
                // FIXME: This extra copy shouldn't be necessary but it fixes the odd invalid reads in Travis.
                value = value.to_sequence();
            }
        #endif

            typename value_type::const_iterator it(value.begin()), end(value.end());
            typename options_type::names_type const& variables = kernel.extract_names(vars);

            if (it == end) {
                if (empty) { // for ... empty ... endfor case.
                    kernel.render_block(ostream, result, empty, context);
                }
                return;
            }

            context_type context_copy = context;
            size_type const n = variables.size();
            BOOST_ASSERT(n > 0);

            for (; it != end; ++it) {
                if (n == 1) { // e.g. for x in ...
                    context_copy[variables[0]] = /* XXX: boost::ref */(*it);
                }
                else {
                    size_type i = 0;
                    BOOST_FOREACH(value_type const& var, *it) { // e.g. for x, y, z in ...
                        if (i >= n) break;
                        context_copy[variables[i++]] = /* XXX: boost::ref */(var);
                    }

                    while (i < n) { // Overwrite the remaining vars in the context.
                        context_copy[variables[i++]] = result.options().default_value;
                    }
                }

                kernel.render_block(ostream, result, for_, context_copy);
            }
        }
    };

//
// for_empty_tag
//     NOTE: This is a no-op, since the empty clause is already handled by for_tag.
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct for_empty_tag {
        static regex_type syntax(kernel_type& kernel) { return kernel.nothing; }
        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {}
    };

//
// if_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct if_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("if") >> kernel.value) >> kernel.block
              >> !(TAG(kernel.reserved("else")) >> kernel.block)
                >> TAG(kernel.reserved("endif"));
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            match_type   const& if_   = match(kernel.block, 0);
            match_type   const& else_ = match(kernel.block, 1);
            boolean_type const  cond_ = kernel.evaluate(result, match(kernel.value), context);

                 if (cond_) kernel.render_block(ostream, result, if_,   context);
            else if (else_) kernel.render_block(ostream, result, else_, context);
        }
    };

//
// ifchanged_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct ifchanged_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("ifchanged") >> !kernel.values) >> kernel.block
              >> !(TAG(kernel.reserved("else"))     >> kernel.block)
              >>   TAG(kernel.reserved("endifchanged"));
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            match_type const& if_   = match(kernel.block, 0);
            match_type const& else_ = match(kernel.block, 1);

            size_type const position = match.position();
            optional<value_type> const value = detail::find(position, context.changes_);

            if (match_type const& vals = match(kernel.values)) { // Compare variables.
                // NOTE: The key is a string (rather than e.g. an int) presumably in case variables are repeated.
                mapping_type values;

                BOOST_FOREACH(match_type const& val, kernel.select_nested(vals, kernel.value)) {
                    string_type const s = text::strip(val.str());
                    values[s] = kernel.evaluate(result, val, context);
                }

                if (value && value->template as<mapping_type>() == values) {
                    if (else_) {
                        kernel.render_block(ostream, result, else_, context);
                    }
                }
                else {
                    context.changes_[position] = values;
                    kernel.render_block(ostream, result, if_, context);
                }
            }
            else { // No variables, compare contents.
                string_stream_type ss;
                kernel.render_block(ss, result, if_, context);
                string_type const s = ss.str();

                if (value && value->template as<string_type>() == s) {
                    if (else_) {
                        kernel.render_block(ostream, result, else_, context);
                    }
                }
                else {
                    context.changes_[position] = s;
                    ostream << s;
                }
            }
        }
    };

//
// ifequal_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct ifequal_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("ifequal") >> kernel.value >> kernel.value) >> kernel.block
              >> !(TAG(kernel.reserved("else")) >> kernel.block)
              >>   TAG(kernel.reserved("endifequal"));
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            match_type   const& left  = match(kernel.value, 0);
            match_type   const& right = match(kernel.value, 1);
            match_type   const& if_   = match(kernel.block, 0);
            match_type   const& else_ = match(kernel.block, 1);
            boolean_type const  cond_ = kernel.evaluate(result, left, context) ==
                                        kernel.evaluate(result, right, context);

                 if (cond_) kernel.render_block(ostream, result, if_,   context);
            else if (else_) kernel.render_block(ostream, result, else_, context);
        }
    };

//
// ifnotequal_tag
// TODO: DRY this and ifequal_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct ifnotequal_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("ifnotequal") >> kernel.value >> kernel.value) >> kernel.block
              >> !(TAG(kernel.reserved("else")) >> kernel.block)
              >>   TAG(kernel.reserved("endifnotequal"));
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            match_type   const& left  = match(kernel.value, 0);
            match_type   const& right = match(kernel.value, 1);
            match_type   const& if_   = match(kernel.block, 0);
            match_type   const& else_ = match(kernel.block, 1);
            boolean_type const  cond_ = kernel.evaluate(result, left, context) !=
                                        kernel.evaluate(result, right, context);

                 if (cond_) kernel.render_block(ostream, result, if_,   context);
            else if (else_) kernel.render_block(ostream, result, else_, context);
        }
    };

//
// include_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct include_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("include") >> kernel.string_literal >> *_s >>
                !(kernel.keyword("with") >> kernel.arguments >> !(s1 = kernel.keyword("only"))));
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            path_type const path = kernel.extract_path(match(kernel.string_literal));

            if (match_type const& args = match(kernel.arguments)) {
                boolean_type const only = match[s1].matched;

                arguments_type const& arguments = kernel.evaluate_arguments(result, args, context);
                if (!arguments.first.empty()) {
                    AJG_SYNTH_THROW(std::invalid_argument("positional argument"));
                }

                context_type context_copy = only ? context_type() : context;
                BOOST_FOREACH(named_argument_type const& argument, arguments.second) {
                    context_copy[argument.first] = argument.second;
                }
                kernel.render_path(ostream, result, path, context_copy);
            }
            else {
                kernel.render_path(ostream, result, path, context);
            }
        }
    };

//
// include_with_tag
//     NOTE: This is a no-op, since the with clause is already handled by include_tag.
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct include_with_tag {
        static regex_type syntax(kernel_type& kernel) { return kernel.nothing; }
        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {}
    };

//
// include_with_only_tag
//     NOTE: This is a no-op, since the with ... only clause is already handled by include_tag.
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct include_with_only_tag {
        static regex_type syntax(kernel_type& kernel) { return kernel.nothing; }
        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {}
    };

//
// library_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct library_tag {
        static regex_type syntax(kernel_type& kernel) {
            typename x::function<on_arg_>::type const on_arg = {{}};
            typename x::function<on_continue_>::type const on_continue = {{}};
            typename x::function<on_polyadic1_tag_>::type const on_polyadic1_tag = {{}};
            typename x::function<on_polyadic2_tag_>::type const on_polyadic2_tag = {{}};

            kernel.polyadic_tag = x::keep((TAG((s1 = kernel.unreserved_name) >> *x::keep(kernel.argument[ on_arg(kernel._state, _) ])))[x::check(on_polyadic1_tag(x::ref(kernel), kernel._result, kernel._state, kernel._range, s1, _))]) >> *(x::nil[ x::check(on_continue(kernel._state)) ] >>
                  kernel.block >> x::keep((TAG((s2 = kernel.unreserved_name) >> *x::keep(kernel.argument[ on_arg(kernel._state, _) ])))[x::check(on_polyadic2_tag(x::ref(kernel), kernel._result, kernel._state, kernel._range, s2, _))]));

            return kernel.polyadic_tag;
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
         // string_type    const& name      = match(kernel.unreserved_name)[id].str();
            size_type      const  position  = match.position(1);
            match_type     const& args      = match; // (kernel.arguments);
            arguments_type const  arguments = kernel.evaluate_arguments(result, args, context);

            if (boost::optional<renderer_type> const& renderer = kernel.get_state(result).get_renderer(position)) {
                BOOST_ASSERT(!renderer->empty());
                (*renderer)(arguments, ostream, context, &match);
            }
            else {
                AJG_SYNTH_THROW(std::logic_error("missing renderer"));
            }
        }

      private:

        static void render_block( size_type      const  index
                                , kernel_type    const& kernel
                                , result_type    const& result
                                , arguments_type const& arguments
                                , ostream_type&         ostream
                                , context_type&         context
                                , void const*           m
                                ) {
            BOOST_ASSERT(m);
            match_type const& match = *static_cast<match_type const*>(m);
            match_type const& block = match(kernel.block, index);
            kernel.render_block(ostream, result, block, context);
        }

        struct on_arg_ {
            typedef void result_type;
            void operator()(state_type& state, sub_match_type const& sub_match) const {
                state.library_tag_args_.push_back(text::strip_right(sub_match.str()));
            }
        };

        struct on_continue_ {
            typedef boolean_type result_type;
            boolean_type operator()(state_type& state) const {
                return /*!state.library_tag_entries_.empty(); //*/ state.library_tag_continue_;
            }
        };

        struct on_polyadic1_tag_ {
            typedef boolean_type result_type;
            boolean_type operator()( kernel_type& kernel
                                    , typename kernel_type::result_type& result
                                    , state_type& state
                                    , range_type const& range
                                    , sub_match_type const& n
                                    , sub_match_type const& c
                                    ) const {
                string_type const name     = text::strip_right(n.str());
                string_type const contents = text::strip_right(c.str());
                std::vector<string_type> const args = state.library_tag_args_;
                state.library_tag_args_.clear();


                if (!state.library_tag_entries_.empty() && detail::contains(name, state.library_tag_entries_.top().tag.second)) {
                    return false;
                }
                else if (boost::optional<typename options_type::tag_type> const& tag = state.get_tag(name)) {
                    size_type const position = std::distance(range.first, n.first);
                    std::vector<string_type> pieces;
                    pieces.push_back(contents);
                    pieces.push_back(name);
                    BOOST_FOREACH(string_type const& arg, args) {
                        pieces.push_back(arg);
                    }

                    renderer_type const renderer = boost::bind(render_block, 0, boost::ref(kernel), boost::ref(result), _1, _2, _3, _4);
                    segments_type segments(1, segment_type(pieces, renderer));

                    if (tag->second.empty()) {
                        state.set_renderer(position, tag->first(segments));
                        state.library_tag_continue_ = false;
                    }
                    else {
                        entry_type entry = { position, *tag, segments };
                        state.library_tag_entries_.push(entry);
                        state.library_tag_continue_ = true;
                    }

                    return true;
                }
                else {
                    return false;
                    // AJG_SYNTH_THROW(missing_tag(text::narrow(name)));
                }
            };
        };

        struct on_polyadic2_tag_ {
            typedef boolean_type result_type;
            boolean_type operator()( kernel_type& kernel
                                   , typename kernel_type::result_type& result
                                   , state_type& state
                                   , range_type const& range
                                   , sub_match_type const& n
                                   , sub_match_type const& c
                                   ) const {
                string_type const name     = text::strip_right(n.str());
                string_type const contents = text::strip_right(c.str());
                std::vector<string_type> const args = state.library_tag_args_;
                state.library_tag_args_.clear();

                BOOST_ASSERT(!state.library_tag_entries_.empty());
                entry_type& entry = state.library_tag_entries_.top();
                size_type const position = entry.position;
                BOOST_ASSERT(!entry.tag.second.empty());

                if (!detail::contains(name, entry.tag.second)) {
                    return false;
                }

                std::vector<string_type> pieces;
                pieces.push_back(contents);
                pieces.push_back(name);
                BOOST_FOREACH(string_type const& arg, args) {
                    pieces.push_back(arg);
                }

                renderer_type const renderer = boost::bind(render_block, entry.segments.size(), boost::ref(kernel), boost::ref(result), _1, _2, _3, _4);
                entry.segments.push_back(segment_type(pieces, renderer));

                if (text::begins_with(name, text::literal("end"))) {
                    state.set_renderer(position, entry.tag.first(entry.segments));
                    state.library_tag_continue_ = false;
                    state.library_tag_entries_.pop();
                }
                else {
                    state.library_tag_continue_ = true;
                }

                return true;
            };
        };
    };

//
// load_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct load_tag {
        static regex_type syntax(kernel_type& kernel) {
            typename x::function<loader>::type const load = {{}};
            return x::keep(TAG(kernel.reserved("load") >> x::keep(s1 = kernel.packages))[load(kernel._state, s1)]);
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {}

      private:

        struct loader {
            typedef void result_type;
            void operator()(state_type& state, sub_match_type const& packages) const {
                BOOST_FOREACH(string_type const& library, text::space(packages.str())) {
                    state.load_library(library);
                }
            }
        };
    };

//
// load_from_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct load_from_tag {
        static regex_type syntax(kernel_type& kernel) {
            typename x::function<loader>::type const load = {{}};
            return x::keep(TAG(kernel.reserved("load") >> x::keep(s1 = kernel.names) >> kernel.keyword("from") >> x::keep(s2 = kernel.package))[load(kernel._state, s1, s2)]);
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {}

      private:

        struct loader {
            typedef void result_type;
            void operator()(state_type& state, sub_match_type const& names, sub_match_type const& package) const {
                string_type const& library    = text::strip_right(package);
                names_type  const& components = text::space(names.str());
                state.load_library(library, components);
            }
        };
    };

//
// now_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct now_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("now") >> kernel.string_literal);
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            string_type const format = kernel.extract_string(match(kernel.string_literal));
            ostream << formatter_type::format_datetime(result.options(), format, traits_type::utc_datetime());
        }
    };

//
// regroup_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct regroup_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("regroup") >> kernel.value
                          >> kernel.keyword("by") >> kernel.package
                          >> kernel.keyword("as") >> kernel.name) >> kernel.block;
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            match_type  const& expr  = match(kernel.value);
            string_type const& attrs = match(kernel.package)[id].str();
            string_type const& name  = match(kernel.name)[id].str();
            match_type  const& block = match(kernel.block);

            value_type        values;
            entries_type entries;
            try {
                values = kernel.evaluate(result, expr, context);
            }
            // Fail silently in these cases:
            catch (missing_variable  const&) { goto done; }
            catch (missing_attribute const&) { goto done; }
            entries = regroup(values, attrs);

          done:
            context_type context_copy = context;
            context_copy[name] = entries;
            kernel.render_block(ostream, result, block, context_copy);
        }

        typedef std::map<string_type, value_type>   entry_type;
        typedef std::vector<entry_type>             entries_type;

        inline static entries_type regroup(value_type const& values, string_type const& attrs) {
            static string_type const grouper_name = text::literal("grouper");
            static string_type const list_name    = text::literal("list");
            entries_type entries;

            BOOST_FOREACH(typename value_type::group_type const& group, values.group_by(attrs)) {
                value_type const key   = group.first;
                value_type const value = group.second;
                entry_type entry;
                entry[grouper_name] = key;
                entry[list_name]    = value;
                entries.push_back(entry);
            }

            return entries;
        }
    };

//
// spaceless_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct spaceless_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("spaceless")) >> kernel.block
                >> TAG(kernel.reserved("endspaceless"));
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            static typename kernel_type::string_regex_type const tag = '<' >> *~(as_xpr('>')) >> '>';
            static typename kernel_type::string_regex_type const gap = (s1 = tag) >> +_s >> (s2 = tag);

            string_stream_type ss;
            std::ostreambuf_iterator<char_type> it(ostream);

            match_type const& body = match(kernel.block);
            kernel.render_block(ss, result, body, context);
            // TODO: Use bidirectional_input_stream to feed directly to regex_replace.
            string_type const string = ss.str();
            x::regex_replace(it, string.begin(), string.end(), gap, text::literal("$1$2"));
        }
    };

//
// ssi_tag:
//     TODO: Take into account `ALLOWED_INCLUDE_ROOTS`.
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct ssi_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("ssi") >> kernel.value >> !(s1 = kernel.keyword("parsed")));
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            value_type   const value  = kernel.evaluate(result, match(kernel.value), context);
            path_type    const path   = traits_type::to_path(value.to_string());
            boolean_type const parsed = match[s1].matched;

            if (!detail::is_absolute(path)) {
                AJG_SYNTH_THROW(std::invalid_argument("relative path"));
            }

            if (parsed) {
                kernel.render_path(ostream, result, path, context);
            }
            else {
                string_type line;
                std::string const path_ = text::narrow(path);
                std::basic_ifstream<char_type> file(path_.c_str());
                while (std::getline(file, line)) ostream << line << kernel.newline;
            }
        }
    };

//
// templatetag_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct templatetag_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("templatetag") >> kernel.name);
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            string_type const name = match(kernel.name)[id].str();

            if (optional<string_type> const& marker = detail::find(name, kernel.markers)) {
                ostream << *marker;
            }
            else {
                ostream << result.options().default_value;
            }
        }
    };

//
// url_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct url_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("url") >> kernel.value >> kernel.arguments);
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            match_type const& expr = match(kernel.value);
            match_type const& args = match(kernel.arguments);

            value_type     const view      = kernel.evaluate(result, expr, context);
            arguments_type const arguments = kernel.evaluate_arguments(result, args, context);

            if (optional<string_type> const& url = kernel.get_view_url(result, view, arguments, context)) {
                ostream << *url;
            }
            else {
                AJG_SYNTH_THROW(std::runtime_error("view not found"));
            }
        }
    };

//
// url_as_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct url_as_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("url") >> kernel.value >> kernel.arguments
                        >> kernel.keyword("as") >> kernel.name) >> kernel.block;
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            match_type  const& expr  = match(kernel.value);
            match_type  const& args  = match(kernel.arguments);
            match_type  const& block = match(kernel.block);
            string_type const& name  = match(kernel.name)[id].str();

            value_type     const view      = kernel.evaluate(result, expr, context);
            arguments_type const arguments = kernel.evaluate_arguments(result, args, context);
            string_type    const url       = kernel.get_view_url(result, view, arguments, context)
                                                   .get_value_or(string_type());

            context_type context_copy = context;
            context_copy[name] = url;
            kernel.render_block(ostream, result, block, context_copy);
        }
    };

//
// variable_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct variable_tag {
        static regex_type syntax(kernel_type& kernel) {
            return kernel.variable_open >> *_s >> kernel.value >> kernel.variable_close;
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            value_type const& value = kernel.evaluate(result, match(kernel.value), context);
            boolean_type const safe = !context.autoescape_ || value.safe();
            safe ? ostream << value : ostream << value.escape();
        }
    };

//
// verbatim_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct verbatim_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("verbatim")) >> kernel.block
                >> TAG(kernel.reserved("endverbatim"));
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            ostream << match(kernel.block).str();
        }
    };

//
// widthratio_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct widthratio_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("widthratio") >> kernel.value >> kernel.value >> kernel.value);
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            match_type const& value = match(kernel.value, 0);
            match_type const& limit = match(kernel.value, 1);
            match_type const& width = match(kernel.value, 2);

            floating_type const ratio
                = kernel.evaluate(result, value, context).to_floating()
                / kernel.evaluate(result, limit, context).to_floating()
                * kernel.evaluate(result, width, context).to_floating();

            ostream << round(ratio);
        }

      private:

        template <class T>
        inline static T round(T const r) {
            return (r > 0.0) ? (std::floor)(r + 0.5) : (std::ceil)(r - 0.5);
        }
    };

//
// with_tag
////////////////////////////////////////////////////////////////////////////////////////////////////

    struct with_tag {
        static regex_type syntax(kernel_type& kernel) {
            return TAG(kernel.reserved("with") >> kernel.value >> kernel.keyword("as") >> kernel.name) >> kernel.block
                >> TAG(kernel.reserved("endwith"));
        }

        static void render( kernel_type  const& kernel
                          , result_type  const& result
                          , match_type   const& match
                          , context_type&       context
                          , ostream_type&       ostream
                          ) {
            match_type  const& value = match(kernel.value);
            match_type  const& body  = match(kernel.block);
            string_type const& name  = match(kernel.name)[id].str();

            context_type context_copy = context;
            context_copy[name] = kernel.evaluate(result, value, context);
            kernel.render_block(ostream, result, body, context_copy);
        }
    };

#undef TAG

}; // builtin_tags

}}}} // namespace ajg::synth::engines::django

#endif // AJG_SYNTH_ENGINES_DJANGO_BUILTIN_TAGS_HPP_INCLUDED
