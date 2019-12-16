#include "cconsts.h"
#include "cparser.h"
#include "builder.hpp"
#include "parser.hpp"
#include <iostream>
#include <string_view>

#ifndef LLREDIS_IMPL_BUILDER
#define LLREDIS_IMPL_BUILDER llparse::redis::builder
#endif

#define LLREDIS_CALL_PARSER_SPAN(builder) reinterpret_cast<llparse::redis::parser<builder>*>(s->data) \
    ->on_value({ \
        reinterpret_cast<const char*>(p), static_cast<std::size_t>(endp - p) \
    }, s->size == 0)

#define LLREDIS_CALL_PARSER_SIZE(builder) reinterpret_cast<llparse::redis::parser<builder>*>(s->data) \
    ->on_value(static_cast<std::int64_t>(s->size))

#define LLREDIS_CALL_PARSER_NULL(builder) reinterpret_cast<llparse::redis::parser<builder>*>(s->data) \
    ->on_value(nullptr)

#define LLREDIS_CALL_PARSER_ARRAY(builder) reinterpret_cast<llparse::redis::parser<builder>*>(s->data) \
    ->on_array(static_cast<std::int64_t>(s->size))

extern "C" {
    // 类型：新数据项开始
    int llredis__on_type(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp) {

        std::cout << "type: " << p[0] << "\n";
        s->type = p[0];

        return 0;
    }
    // 错误信息（简单字符串）
    int llredis__on_error(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp) {
    
        std::cout << "error: "
            << std::string_view(reinterpret_cast<const char*>(p), endp - p)
            << " (" << (s->size == 0) << ")" << std::endl;
        
        LLREDIS_CALL_PARSER_SPAN(LLREDIS_IMPL_BUILDER);
        return 0;
    }
    // 简单字符串
    int llredis__on_simple_string(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp) {

        std::cout << "string: "
            << std::string_view(reinterpret_cast<const char*>(p), endp - p)
            << " (" << (s->size == 0) << ")" << std::endl;
        
        LLREDIS_CALL_PARSER_SPAN(LLREDIS_IMPL_BUILDER);
        return 0;
    }
    // 负数（符号）
    int llredis__on_integer_m(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp) {

        s->size = -s->size;
        return 0;
    }
    // 整数
    int llredis__on_integer(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp) {

        std::cout << "integer: " << (std::int64_t)s->size << std::endl;
        LLREDIS_CALL_PARSER_SIZE(LLREDIS_IMPL_BUILDER);
        return 0;
    }
    // 字符串长度（计算剩余）
    int llredis__on_bulk_string_next(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp) {
        
        -- s->size;
        return 0;
    }
    // 长度字符串
    int llredis__on_bulk_string(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp) {

        std::cout << "bulk_string: "
            << std::string_view(reinterpret_cast<const char*>(p), endp - p)
            << " (" << (s->size == 0) << ")" <<std::endl;
        
        LLREDIS_CALL_PARSER_SPAN(LLREDIS_IMPL_BUILDER);
        return 0;
    }
    // 数组成员数量
    int llredis__on_array_size(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp) {

        std::cout << "array size: " << s->size << std::endl;
        LLREDIS_CALL_PARSER_ARRAY(LLREDIS_IMPL_BUILDER);
        return 0;
    }
    int llredis__on_null(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp) {
        
        std::cout << "null" << std::endl;
        LLREDIS_CALL_PARSER_NULL(LLREDIS_IMPL_BUILDER);
        return 0;
    }
}
