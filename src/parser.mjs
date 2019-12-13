"use strict";
// REDIS 协议参考：https://redis.io/topics/protocol

import llparse from 'llparse';
// 错误常量
export const ERRORS = Object.fromEntries([
    "ERROR_SUCCESS",
    "ERROR_UNKNOWN_TYPE",
    "ERROR_SIZE_OVERFLOW",
    "ERROR_UNEXPECTED_TOKEN",
    "ERROR_UNKNOWN_ERROR",
].map((v, i) => [v, i]));
// 值类型
export const VALUE_TYPES = {
    "ERROR": '-',
    "SIMPLE_STRING": '+',
    "INTEGER": ':',
    "BULK_STRING": '$',
    "ARRAY": '*',
};
// 
const NUMBER_MAPPING = Object.fromEntries([
    "0", "1", "2", "3", "4",
    "5", "6", "7", "8", "9", 
    "ERROR_UNKNOWN_ERROR",
].map((v, i) => [v, i]));
// 
const parser = new llparse.LLParse("llredis");

function E(name, where) {
    if(ERRORS[name]) return parser.error(ERRORS[name], name + " @" + where);
    else throw new Error("undefined error '" + name + "'");
}
// 用于存储部分 整数、长度 信息；
parser.property("i64", "size");

const    before_type      = parser.node("before_type");
const     reset_size      = parser.invoke(parser.code.update("size", 0));
const           type_span = parser.span(parser.code.span("llredis__on_type"));
const        on_type      = parser.node("on_type");
// 简单字符串
const          error_span = parser.span(parser.code.span("llredis__on_error"));
const       on_error      = parser.node("on_error");
const    simple_string_span = parser.span(parser.code.span("llredis__on_simple_string"));
const on_simple_string      = parser.node("on_simple_string");
// 整数
const     on_integer      = parser.node("on_integer");
const   calc_integer_p    = parser.invoke(
    parser.code.mulAdd("size", {base: 10, signed: true}),
    {1: E("ERROR_SIZE_OVERFLOW", "calc_integer")});
const   calc_integer_m      = parser.invoke(
    parser.code.mulAdd("size", {base: 10, signed: true}),
    {1: E("ERROR_SIZE_OVERFLOW", "calc_integer")});
const     on_integer_m    = parser.node("on_integer");
const  after_integer      = parser.invoke(parser.code.match("llredis__on_integer"));
const  after_integer_m    = parser.invoke(parser.code.match("llredis__on_integer_m"));
// 长度字符串
const    on_bulk_string_size = parser.node("on_bulk_string_size");
const  calc_bulk_string_size = parser.invoke(
    parser.code.mulAdd("size", {base: 10, signed: false}),
    {1: E("ERROR_SIZE_OVERFLOW", "calc_bulk_string_size")});
const       bulk_string_span = parser.span(parser.code.span("llredis__on_bulk_string"));
const    on_bulk_string      = parser.node("on_bulk_string");
const    on_bulk_string_r    = parser.node("on_bulk_string_r");
const after_bulk_string   = parser.node("after_bulk_string");
const    if_bulk_string_r = parser.invoke(
    parser.code.isEqual("size", 0), {1: bulk_string_span.end(after_bulk_string)});
const    on_bulk_string_next = parser.invoke(
    parser.code.match("llredis__on_bulk_string_next")); 
// 数组（大小）
const    on_array_size = parser.node("on_array_size");
const  calc_array_size = parser.invoke(
    parser.code.mulAdd("size", {base: 10, signed: false}),
    {1: E("ERROR_SIZE_OVERFLOW", "calc_bulk_string_size")});
const after_array_size = parser.invoke(parser.code.match("llredis__on_array_size"));
// 特殊：NULL（实际为 -1 长度字符串）
const    on_null = parser.invoke(parser.code.match("llredis__on_null"));

before_type
    .match('\r', before_type)
    .match('\n', before_type)
    .otherwise(reset_size);
reset_size
    .otherwise(type_span.start(on_type));
on_type
    .match('-', type_span.end(error_span.start(on_error)))
    .match('+', type_span.end(simple_string_span.start(on_simple_string)))
    .match(':', type_span.end(on_integer))
    .match('$', type_span.end(on_bulk_string_size))
    .match('*', type_span.end(on_array_size))
    .otherwise(E("ERROR_UNKNOWN_TYPE", "on_type"));
on_error
    .peek('\r', error_span.end(before_type)) // "\r\n" -> before_type
    .skipTo(on_error);

on_simple_string
    .peek('\r', simple_string_span.end(before_type)) // "\r\n" -> before_type
    .skipTo(on_simple_string)

on_integer
    .match("+", on_integer)
    .match("-", on_integer_m)
    .select(NUMBER_MAPPING, calc_integer_p)
    .match("\r\n", after_integer)
    .otherwise(E("ERROR_UNEXPECTED_TOKEN", "on_integer"));
calc_integer_p
    .otherwise(on_integer);

on_integer_m
    .select(NUMBER_MAPPING, calc_integer_m)
    .match("\r\n", after_integer_m)
    .otherwise(E("ERROR_UNEXPECTED_TOKEN", "on_integer"));
calc_integer_m
    .otherwise(on_integer_m);
after_integer_m // change to minus sign
    .otherwise(after_integer)
after_integer // invoke callback
    .otherwise(before_type);

on_bulk_string_size
    .match("-1\r\n", on_null)
    .select(NUMBER_MAPPING, calc_bulk_string_size)
    .match("\r\n", bulk_string_span.start(on_bulk_string))
    .otherwise(E("ERROR_UNEXPECTED_TOKEN", "on_bulk_string_size"));
calc_bulk_string_size
    .otherwise(on_bulk_string_size);
on_bulk_string
    .peek('\r', if_bulk_string_r) // -> on_bulk_string
    .skipTo(on_bulk_string_next);
if_bulk_string_r
    .otherwise(on_bulk_string_r);
on_bulk_string_r
    .skipTo(on_bulk_string_next); // '\r'
on_bulk_string_next
    .otherwise(if_bulk_string_r);
after_bulk_string
    .match('\r', before_type) // '\n' -> before_type
    .otherwise(E("ERROR_UNEXPECTED_TOKEN", "after_bulk_string"));

on_null
    .otherwise(before_type);

on_array_size
    .select(NUMBER_MAPPING, calc_array_size)
    .match("\r\n", after_array_size)
    .otherwise(E("ERROR_UNEXPECTED_TOKEN", "after_bulk_string"));
calc_array_size
    .otherwise(on_array_size);
after_array_size
    .otherwise(before_type);

export default parser;
export let begin_node = before_type;
