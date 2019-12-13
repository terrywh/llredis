#ifndef LLREDIS_VALUE_TYPE_H
#define LLREDIS_VALUE_TYPE_H
#include <cstdint>
#include <string>
#include <variant>
#include <vector>


namespace llparse::redis {
    // 数据容器
    class value: public std::variant<std::string, std::int64_t, std::vector<value>> {
    //     union {
    //         std::string          str;
    //         std::int64_t         i64;
    //         std::vector<value_t> arr;
    //     };
    //     std::uint8_t type; // 用于确认上述 union 实际包含类型
    //     template <int
    //     void as() {
    //         return 
    //     }
    };
}

#endif // LLREDIS_VALUE_TYPE_H