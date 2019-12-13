 
#include "../src/cparser.h"
#include <iostream>
#include <string>

template <class T>
void null_deleter(T* t) {}

void error_assert(int err, llredis_t* parser) {
    if(err != 0) {
        std::cout 
            << "ERROR: (" << parser->error << ") "
            << parser->reason << "\n";
        std::exit(-1);
    }
}

int main(int argc, char* argv[]) {
    llredis_t parser;
    llredis_init(&parser);
    
    int err = 0;
    std::string str;

    str.assign("+abc\r\n");
    err = llredis_execute(&parser, str.c_str(), str.c_str() + str.size());
    error_assert(err, &parser);

    str.assign("-abc\r\n");
    err = llredis_execute(&parser, str.c_str(), str.c_str() + str.size());
    error_assert(err, &parser);

    str.assign(":123\r\n");
    err = llredis_execute(&parser, str.c_str(), str.c_str() + str.size());
    error_assert(err, &parser);

    str.assign(":-321\r\n");
    err = llredis_execute(&parser, str.c_str(), str.c_str() + str.size());
    error_assert(err, &parser);

    str.assign("$8\r\nxyz\r\n123\r\n");
    err = llredis_execute(&parser, str.c_str(), str.c_str() + str.size());
    error_assert(err, &parser);

    str.assign("*2\r\n:1\r\n:2\r\n:3\r\n");
    err = llredis_execute(&parser, str.c_str(), str.c_str() + str.size());
    error_assert(err, &parser);

    str.assign("$-1\r\n");
    err = llredis_execute(&parser, str.c_str(), str.c_str() + str.size());
    error_assert(err, &parser);

    str.assign("$0\r\n\r\n");
    err = llredis_execute(&parser, str.c_str(), str.c_str() + str.size());
    error_assert(err, &parser);
}