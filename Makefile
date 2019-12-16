# 安装
PREFIX=/data/vendor/llredis-0.1.0
# 编译环境
NODE=/data/server/node/bin/node
CXX?=clang++
CC?=clang
AR?=ar
CFLAGS?= -g -O0
CFLAGS+= -std=c11 -fPIC
CXXFLAGS?= -g -O0
CXXFLAGS+= -std=c++17 -fPIC

.PHONY: clean
all: llredis

clean:
	rm -rf build/* libllredis.a llredis
	rm -rf src/cparser.c src/cconsts.h src/cparser.h 

HEADERS_FILES = src/builder.hpp src/cconsts.h \
 src/cparser.c src/cparser.h src/handler.hpp \
 src/parser.hpp src/value_type.hpp

./src/cconsts.h: ./src/builder.mjs ./src/parser.mjs
	${NODE} --experimental-modules ./src/builder.mjs
# ./src/cparser.h: ./src/builder.mjs ./src/parser.mjs
# 	${NODE} --experimental-modules ./src/builder.mjs
# ./src/cparser.c: ./src/builder.mjs ./src/parser.mjs
# 	${NODE} --experimental-modules ./src/builder.mjs

./build/cparser.o: ./src/cparser.c
	${CC} ${CFLAGS} -c $< -o $@

llredis: ./build/test/main.o ./build/cparser.o
	${CXX} $^ -o $@

./build/test:
	mkdir -p build/test

./build/redis:
	mkdir -p build/redis

./build/test/main.o: ./test/main.cpp ${HEADERS_FILES} ./build/test
	${CXX} ${CXXFLAGS} -c $< -o $@

install:
	mkdir -p ${PREFIX}/include
	cp builder.hpp ${PREFIX}/include
	cp cconsts.h   ${PREFIX}/include
	cp cparser.h   ${PREFIX}/include
	cp handler.hpp ${PREFIX}/include
	cp parser.hpp  ${PREFIX}/include
	cp value_type.hpp ${PREFIX}/include
	cp lllredis.hpp ${PREFIX}/include
