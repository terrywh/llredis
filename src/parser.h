#ifndef LLREDIS_PARSER_H
#define LLREDIS_PARSER_H

#include "cconsts.h"
#include "cparser.h"
#include <string>

namespace llparse::redis {

    template <class StringT = std::string>
    class parser {
    public:

        parser();

        int in_array() {
            return array_;
        }
    private:
        llredis_t parser_;
        int        array_;
    };
}

#endif // LLREDIS_PARSER_H