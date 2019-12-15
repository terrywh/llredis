#include "handler.h"
#include <iostream>
#include <string_view>

extern "C" {
    // 类型回调 (新数据开始)
    int llredis__on_type(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp) {

        std::cout << "type: " << p[0] << "\n";

        return 0;
    }
    // 错误信息（简单字符串）
    int llredis__on_error(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp) {
    
        std::cout << "error: "
            << std::string_view(reinterpret_cast<const char*>(p), endp - p)
            << " (" << (s->size == LLREDIS_UNKNOWN_SIZE) << ")" << std::endl;
        return 0;
    }
    // 简单字符串
    int llredis__on_simple_string(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp) {

        std::cout << "string: "
            << std::string_view(reinterpret_cast<const char*>(p), endp - p)
            << " (" << (s->size == LLREDIS_UNKNOWN_SIZE) << ")" << std::endl;
        return 0;
    }
    // 负数（符号）
    int llredis__on_integer_m(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp) {

        s->size = -s->size;
        return 0;
    }
    // 数值
    int llredis__on_integer(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp) {

        std::cout << "integer: " << (std::int64_t)s->size << std::endl;
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
            << " (" << (s->size == LLREDIS_UNKNOWN_SIZE) << ")" <<std::endl;
        return 0;
    }
    // 数组成员数量
    int llredis__on_array_size(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp) {

        std::cout << "array size: " << s->size << std::endl;
        return 0;
    }
    // 特殊 NULL
    int llredis__on_null(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp) {
        
        std::cout << "null" << std::endl;
        return 0;
    }
}