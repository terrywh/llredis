#ifndef LLREDIS_BUILDER_TYPE_H
#define LLREDIS_BUILDER_TYPE_H

#include "value_type.hpp"

namespace llparse::redis {
    
    class builder {
    public:
        builder()
        : root_(nullptr) {
            data_.push( &root_ );
        }

        void append(std::string value) {
            if(data_.top()->index() == value_type::INDEX_ARRAY)
                std::get<std::vector<value_type>>(*data_.top()).emplace_back( std::move(value) );
            else
                root_ = std::move(value);
        }

        void append(std::int64_t value) {
            if(data_.top()->index() == value_type::INDEX_ARRAY)
                std::get<std::vector<value_type>>(*data_.top()).emplace_back( value );
            else
                root_ = value;
        }

        void append(std::nullptr_t value) {
            if(data_.top()->index() == value_type::INDEX_ARRAY)
                std::get<std::vector<value_type>>(*data_.top()).emplace_back( value );
            else
                root_ = value;
        }

        void array_start() {
            std::vector<value_type> array;
            if(data_.top()->index() == value_type::INDEX_ARRAY) {
                auto& ref = std::get<std::vector<value_type>>(*data_.top()).emplace_back( array );
                data_.push(&ref);
            }
            else 
                root_ = std::move(array);
        }

        void array_end() {
            data_.pop();
        }
        /**
         * 从容器中获取构建完毕的数据实例
         * 注意：
         *  1. 本函数调用后，当前 builder 对象数据将被清空（可重复使用）
         */
        value_type value() {
            value_type rv = std::move(root_);
            std::stack<value_type*> data;
            root_ = nullptr;
            data.push(&root_);
            data_.swap(data);
            return std::move(rv);
        }
    private:
        value_type              root_;
        std::stack<value_type*> data_;
    };
}

#endif // LLREDIS_BUILDER_TYPE_H