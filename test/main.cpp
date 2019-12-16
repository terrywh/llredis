 
#include "../src/llredis.hpp"
#include <iostream>
#include <string>

template <class T>
void null_deleter(T* t) {}

void error_assert(int error,
    llparse::redis::parser<llparse::redis::builder>& parser) {
    if(error != 0) {
        std::cout 
            << "ERROR: (" << error << ") "
            << parser.error() << "\n";
        std::exit(-1);
    }
}

int main(int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);

    llparse::redis::builder builder;
    llparse::redis::parser  parser(builder);
  
    int err = 0;
    std::string str;

    str.assign("*8\r\n");
    err = parser.execute({str.c_str(), str.size()});
    error_assert(err, parser);


    str.assign("+abc\r\n");
    err = parser.execute({str.c_str(), str.size()});
    error_assert(err, parser);

    str.assign("-ab");
    err = parser.execute({str.c_str(), str.size()});
    error_assert(err, parser);

    str.assign("c\r\n");
    err = parser.execute({str.c_str(), str.size()});
    error_assert(err, parser);

    str.assign(":123\r\n");
    err = parser.execute({str.c_str(), str.size()});
    error_assert(err, parser);

    str.assign(":-321\r\n");
    err = parser.execute({str.c_str(), str.size()});
    error_assert(err, parser);

    str.assign("$8\r\nxyz\r\n123\r\n");
    err = parser.execute({str.c_str(), str.size()});
    error_assert(err, parser);

    str.assign("*2\r\n:1\r\n:2\r\n:3\r\n");
    err = parser.execute({str.c_str(), str.size()});
    error_assert(err, parser);

    str.assign("$-1\r\n");
    err = parser.execute({str.c_str(), str.size()});
    error_assert(err, parser);

    str.assign("$0\r\n\r\n");
    err = parser.execute({str.c_str(), str.size()});
    error_assert(err, parser);

    auto value = builder.value();
    value.dump();
}