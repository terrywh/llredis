
import { promises as fs } from 'fs';
import { dirname } from 'path';
import { fileURLToPath } from 'url';
const __dirname  = dirname(fileURLToPath(import.meta.url));

import parser, {begin_node, ERRORS, VALUE_TYPES} from './parser.mjs';

async function constant_header() {
    let header = "#ifndef LLREDIS__CONSTANT_H\n#define LLREDIS__CONSTANT_H\n\n#include <cstdint>\n";

    // 错误号
    header += "\nenum llredis_error_value_t: std::uint8_t {\n";
    for(let key in ERRORS) {
        header += "\t" + ("ERROR_" + key).padEnd(16) + " = " + ERRORS[key] + ",\n";
    }
    header += "};\n"
    
    // 错误文本
    // header += "\n\const char* error_s[] = {\n";
    // for(let key in ERRORS) {
    //     header += "\t\t\"" + key + "\",\n";
    // }
    // header += "\t};\n"
    // 类型定义
    header += "\nenum llredis_value_type_t: std::uint8_t {\n";
    for(let key in VALUE_TYPES) {
        header += "\t" + ("VALUE_TYPE_" + key).padEnd(16) + " = '" + VALUE_TYPES[key] + "',\n";
    }
    header += "};\n";
    // 未知长度大小（匹配结束符）
    header += "\nconst std::uint64_t LLREDIS_UNKNOWN_SIZE = -1;\n";
   
    header += "\n#endif // LLREDIS__CONSTANT_H\n";
    await fs.writeFile(__dirname + "/cconsts.h", header);
}

async function build() {
    const art = parser.build(begin_node, {"c": {
        "header": "cparser",
    }});
    await fs.writeFile(__dirname + "/cparser.h", art.header);
    await fs.writeFile(__dirname + "/cparser.c", art.c);
    await constant_header();
    console.log("SUCCESS: parser generated.");
}

build().catch((error) => {
    console.log(error);
    process.exit(-1);
});