#ifndef LLREDIS_VALUE_TYPE_H
#define LLREDIS_VALUE_TYPE_H
#include <cassert>
#include <cstdint>
#include <iostream>
#include <stack>
#include <string>
#include <variant>
#include <vector>

namespace llparse::redis {
    // 数据容器
    class value_type: public std::variant<std::nullptr_t, std::string, std::int64_t, std::vector<value_type>> {
    public:
        using variant::variant;
        enum {
            INDEX_NULL,
            INDEX_STRING,
            INDEX_INTEGER,
            INDEX_ARRAY, 
        };
        /**
         * 作为 BOOL 的条件判定，当以下条件为真时返回 TRUE：
         * 1. 类型为 NULL
         * 2. 类型为 STRING 但其长度为 0
         * 3. 类型为 INTEGER 但其值为 0
         * 4. 类型为 ARRAY 但其长度为 0
         */
        operator bool() {
            switch(index()) {
            case INDEX_INTEGER:
                return std::get<std::int64_t>(*this) == 0;
            case INDEX_STRING:
                return std::get<std::string>(*this).empty();
            case INDEX_ARRAY:
                return std::get<std::vector<value_type>>(*this).empty();
            case INDEX_NULL:
            default:
                return true;
            }
        }
        
        operator std::nullptr_t() {
            assert(index() == INDEX_NULL);
            return nullptr;
        }

        operator int() {
            assert(index() == INDEX_INTEGER);
            return std::get<std::int64_t>(*this);
        }

        operator std::int64_t() {
            assert(index() == INDEX_INTEGER);
            return std::get<std::int64_t>(*this);
        }

        operator std::string&() {
            assert(index() == INDEX_STRING);
            return std::get<std::string>(*this);
        }

        std::string_view view() {
            assert(index() == INDEX_STRING);
            return std::get<std::string>(*this);
        }
        
        operator std::vector<value_type>&() {
            assert(index() == INDEX_ARRAY);
            return std::get<std::vector<value_type>>(*this);
        }
        /**
         * 数组成员访问
         */
        value_type& operator [](int idx) {
            assert(index() == INDEX_ARRAY);
            return std::get<std::vector<value_type>>(*this)[idx];
        }

        static void indent(int z) {
            for(int i=0;i<z;++i) 
                std::cout << "    ";
        }

        void dump(int ind = 0) {
            indent(ind);
            switch(index()) {
            case INDEX_NULL:
                std::cout << "null" << std::endl;
                break;
            case INDEX_STRING:
                std::cout << "string(" << std::get<std::string>(*this) << ")" << std::endl;
                break;
            case INDEX_INTEGER:
                std::cout << "int(" << std::get<std::int64_t>(*this) << ")" << std::endl;
                break;
            case INDEX_ARRAY:
                std::cout << "array(" << std::endl;
                auto vec = std::get<std::vector<value_type>>(*this);
                for(auto i=vec.begin(); i!=vec.end(); ++i) {
                    i->dump(ind + 1);
                }
                indent(ind);
                std::cout << ")" << std::endl;
                break;
            }
        }
    };
}

#endif // LLREDIS_VALUE_TYPE_H