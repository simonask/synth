
//  (C) Copyright 2010 Alvy J. Guty <plus {dot} ajg {at} gmail {dot} com>
//  Use, modification and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#include <map>
#include <stdexcept>

#include <chemical/synthesis/template.hpp>
#include <chemical/synthesis/adapters.hpp>
#include <chemical/synthesis/engines/tmpl.hpp>
#include <chemical/testing.hpp>

typedef char char_t;
namespace cs = chemical::synthesis;
typedef cs::tmpl::engine<> engine_type;
typedef cs::file_template<char_t, engine_type> file_template;
typedef cs::string_template<char_t, engine_type> string_template;

struct context_data {

    context_data() {
        context["foo"] = "A";
        context["bar"] = "B";
        context["qux"] = "C";

        context["true_var"] = true;
        context["false_var"] = false;

        std::map<std::string, std::string> joe, bob, lou;
        joe["name"] = "joe"; joe["age"] = "23";
        bob["name"] = "bob"; bob["age"] = "55";
        lou["name"] = "lou"; lou["age"] = "41";
        friends[0] = joe; friends[1] = bob; friends[2] = lou;

        context["friends"] = friends;
    }

    string_template::context_type context;
    std::map<std::string, std::string> friends[3];
};

typedef chemical::test_group<context_data> group_type;
group_type group_object("tmpl tests");


unit_test(single tag) {
    string_template t("<TMPL_VAR foo>");
    ensure_equals(t.render_to_string(context), "A");
}}

unit_test(invalid tag) {
    ensure_throws(cs::parsing_error,
        string_template("<TMPL_ VAR foo>"));
}}

unit_test(alternative tag) {
    string_template t("<!--TMPL_VAR foo -->");
    ensure_equals(t.render_to_string(context), "A");
}}

unit_test(shortcut variables) {
    string_template t("<TMPL_VAR foo><TMPL_VAR BAR><TMPL_VAR qUx>");
    ensure_equals(t.render_to_string(context), "ABC");
}}

unit_test(qualified variables) {
    string_template t("<TMPL_VAR NAME='foo'><TMPL_VAR NAME='BAR'><TMPL_VAR NAME='qUx'>");
    ensure_equals(t.render_to_string(context), "ABC");
}}

unit_test(default variables) {
    string_template t(
        "<TMPL_VAR foo>\n"
        "<TMPL_VAR foo DEFAULT=B>\n"
        "<TMPL_VAR DEFAULT=B foo>\n"
        "<TMPL_VAR non_extant>A\n"
        "<TMPL_VAR non_extant DEFAULT=A>\n"
        "<TMPL_VAR DEFAULT=A non_extant>\n");
    ensure_equals(t.render_to_string(context), "A\nA\nA\nA\nA\nA\n");
}}

unit_test(html tags) {
    string_template t("<foo>\nA foo <bar /> element.\n</foo>");
    ensure_equals(t.render_to_string(context), t.text());
}}

unit_test(comment tag) {
    string_template t("<TMPL_COMMENT> A comment </TMPL_COMMENT>");
    ensure_equals(t.render_to_string(context), "");
}}

unit_test(single quotes) {
    string_template t("A<TMPL_VAR NAME='bar'>C");
    ensure_equals(t.render_to_string(context), "ABC");
}}

unit_test(double quotes) {
    string_template t("A<TMPL_VAR NAME=\"bar\">C");
    ensure_equals(t.render_to_string(context), "ABC");
}}

unit_test(simple if tag) {
    string_template t(
        "<TMPL_IF true_var>Good</TMPL_IF>");
    ensure_equals(t.render_to_string(context), "Good");
}}

unit_test(invalid if tag) {
    string_template t("<TMPL_IF>Bad</TMPL_IF>");
    ensure_throws(std::logic_error, t.render_to_string());
}}

unit_test(compound if tag) {
    string_template t(
        "<TMPL_IF non_extant>Bad<TMPL_ELSE>Good</TMPL_IF> "
        "<TMPL_IF true_var>Good<TMPL_ELSE>Bad</TMPL_IF> "
        "<TMPL_IF false_var>Bad<TMPL_ELSE>Good</TMPL_IF>");
    ensure_equals(t.render_to_string(context), "Good Good Good");
}}

unit_test(simple unless tag) {
    string_template t(
        "<TMPL_UNLESS false_var>Good</TMPL_UNLESS>");
    ensure_equals(t.render_to_string(context), "Good");
}}

unit_test(compound unless tag) {
    string_template t(
        "<TMPL_UNLESS non_extant>Good<TMPL_ELSE>Bad</TMPL_UNLESS> "
        "<TMPL_UNLESS true_var>Bad<TMPL_ELSE>Good</TMPL_UNLESS> "
        "<TMPL_UNLESS false_var>Good<TMPL_ELSE>Bad</TMPL_UNLESS>");
    ensure_equals(t.render_to_string(context), "Good Good Good");
}}

unit_test(mismatched tags) {
    ensure_throws(cs::parsing_error,
        string_template("<TMPL_IF true_var>Bad</TMPL_UNLESS>"));
}}

unit_test(loop tag) {
    string_template t("<TMPL_LOOP friends><TMPL_VAR name>: <TMPL_VAR age>; </TMPL_LOOP>");
    ensure_equals(t.render_to_string(context), "joe: 23; bob: 55; lou: 41; ");
}}

unit_test(empty loop element) {
    string_template t("<TMPL_LOOP friends></TMPL_LOOP>");
    ensure_equals(t.render_to_string(context), "");
}}

unit_test(loop __size__ variable) {
    string_template t("<TMPL_LOOP friends><TMPL_VAR __SIZE__> </TMPL_LOOP>");
    ensure_equals(t.render_to_string(context), "3 3 3 ");
}}

unit_test(loop __total__ variable) {
    string_template t("<TMPL_LOOP friends><TMPL_VAR __TOTAL__> </TMPL_LOOP>");
    ensure_equals(t.render_to_string(context), "3 3 3 ");
}}

unit_test(loop __first__ variable) {
    string_template t("<TMPL_LOOP friends><TMPL_VAR __FIRST__> </TMPL_LOOP>");
    ensure_equals(t.render_to_string(context), "1 0 0 ");
}}

unit_test(loop __last__ variable) {
    string_template t("<TMPL_LOOP friends><TMPL_VAR __LAST__> </TMPL_LOOP>");
    ensure_equals(t.render_to_string(context), "0 0 1 ");
}}

unit_test(loop __inner__ variable) {
    string_template t("<TMPL_LOOP friends><TMPL_VAR __INNER__> </TMPL_LOOP>");
    ensure_equals(t.render_to_string(context), "0 1 0 ");
}}

unit_test(loop __outer__ variable) {
    string_template t("<TMPL_LOOP friends><TMPL_VAR __OUTER__> </TMPL_LOOP>");
    ensure_equals(t.render_to_string(context), "1 0 1 ");
}}

unit_test(loop __odd__ variable) {
    string_template t("<TMPL_LOOP friends><TMPL_VAR __ODD__> </TMPL_LOOP>");
    ensure_equals(t.render_to_string(context), "1 0 1 ");
}}

unit_test(loop __even__ variable) {
    string_template t("<TMPL_LOOP friends><TMPL_VAR __EVEN__> </TMPL_LOOP>");
    ensure_equals(t.render_to_string(context), "0 1 0 ");
}}

unit_test(loop __counter__ variable) {
    string_template t("<TMPL_LOOP friends><TMPL_VAR __COUNTER__> </TMPL_LOOP>");
    ensure_equals(t.render_to_string(context), "1 2 3 ");
}}

unit_test(file template) {
    file_template t("samples/variables.tmpl");
    ensure_equals(t.render_to_string(context), "ABC");
}}

unit_test(include tag) {
    file_template t("samples/example.tmpl");
    ensure_equals(t.render_to_string(context),
        "============\nABC|\nABC\n============");
}}
