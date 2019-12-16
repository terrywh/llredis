#ifndef LLREDIS_PARSER_H
#define LLREDIS_PARSER_H

#include "cconsts.h"
#include "cparser.h"
#include <string>
#include <string_view>

namespace llparse::redis {

    template <typename BuilderT>
    class parser {
    public:

        parser(BuilderT& ctr)
        : array_(0)
        , ctr_(ctr) {
            llredis_init(&parser_);
            parser_.data = this;
        }
        /**
         * 解析并返回可能的错误码
         */
        int execute(std::string_view data) {
            return llredis_execute(&parser_, data.data(), data.data() + data.size());
        }
        /**
         * 错误信息
         */
        const char* error() {
            return parser_.reason;
        }

        int in_array() {
            return array_;
        }

        BuilderT& container() {
            return ctr_;
        }

        void on_value(std::int64_t value) {
            ctr_.append(value);
            if(array_ > 0 && --array_ == 0) ctr_.array_end();
        }

        void on_value(std::string_view value, bool end) {
            cache_.append(value);
            if(end) {
                ctr_.append(std::move(cache_));
                if(array_ > 0 && --array_ == 0) ctr_.array_end();
            }
        }

        void on_value(std::nullptr_t value) {
            ctr_.append(nullptr);
            if(array_ > 0 && --array_ == 0) ctr_.array_end();
        }

        void on_array(int size) {
            array_ = size;
            ctr_.array_start();
        }

    private:
        llredis_t   parser_;
        int          array_;
        std::string  cache_;
        BuilderT&    ctr_;
    };
}

#endif // LLREDIS_PARSER_H