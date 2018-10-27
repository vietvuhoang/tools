#include <iostream>
#include <algorithm>
#include "app-exception.hpp"
#include "string-utils.hpp"
#include "file-generator.hpp"

extern char *__progname;

int main(int agrc, const char** argv ) {

    try {
        if (agrc < 3 ) {
            std::cout << "Usage : " << __progname << " " << "<filename> <size>" << std::endl;
            return 0;
        }

        std::string file = argv[1];
        
        unsigned long size = StringUtils::toLongVal( argv[2] );

        FileGenerator generator( file, size );

        generator.generate();

    } catch ( std::exception& e ) {
        std::cout << "[" << __progname << "]" << "[ERROR] " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
