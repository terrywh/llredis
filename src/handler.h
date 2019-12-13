#include "cconsts.h"
#include "cparser.h"

extern "C" {
    // 类型回调 (新数据开始)
    int llredis__on_type(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp);
    // 错误信息（简单字符串）
    int llredis__on_error(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp);
    // 简单字符串
    int llredis__on_simple_string(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp);
    // 负数（符号）
    int llredis__on_integer_m(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp);
    // 整数
    int llredis__on_integer(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp);
    // 字符串长度（计算剩余）
    int llredis__on_bulk_string_next(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp);
    // 长度字符串
    int llredis__on_bulk_string(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp);
    // 数组成员数量
    int llredis__on_array_size(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp);
    // 特殊 NULL
    int llredis__on_null(
        llredis_t* s, const unsigned char* p,
        const unsigned char* endp);
}

