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

libllredis.a: ./build/cparser.o ./build/handler.o
	${AR} rcs $@ $^ 

./build/cparser.o: ./src/cparser.c ./src/cparser.h
	${CC} ${CFLAGS} -c $< -o $@

./build/handler.o: ./src/handler.cpp ./src/handler.h ./src/cconsts.h ./src/cparser.h 
	${CXX} ${CXXFLAGS} -c $< -o $@

./src/cconsts.h: ./src/builder.mjs ./src/parser.mjs
	${NODE} --experimental-modules ./src/builder.mjs
./src/cparser.h: ./src/builder.mjs ./src/parser.mjs
	${NODE} --experimental-modules ./src/builder.mjs
./src/cparser.c: ./src/builder.mjs ./src/parser.mjs
	${NODE} --experimental-modules ./src/builder.mjs

llredis: libllredis.a
llredis: ./build/test/main.o
	${CXX} $^ -o $@

./build/test:
	mkdir -p build/test

./build/redis:
	mkdir -p build/redis

./build/test/main.o: ./test/main.cpp ./src/cparser.h ./src/cconsts.h ./build/test
	${CXX} ${CXXFLAGS} -c $< -o $@
