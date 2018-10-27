#include "string-utils.hpp"
#include <algorithm>
#include <cstring>
#include <libgen.h>
#include <limits.h>
#define MAX_ULONG_STR "18446744073709551615"

#ifndef UNITTESTS
    #define STATIC
#else
    #define STATIC static
#endif

STATIC bool isNumber(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

STATIC int getFolderFromFilepath( const char* filename, char* dirpath, int size ) {

    if ( filename == NULL || dirpath == NULL || size <= 1 ) return -1;
    int len = strlen(filename);

    if ( len  == 0 ) {
        strncpy( dirpath, ".", size );
        return  0;
    } 

    if ( filename[ len - 1] == '/') {
        if (len == 1) {            
            strncpy( dirpath, "/", size );
            return  0;
        }

        if ( len - 1 > size ) return -1;

        strncpy( dirpath, filename, len - 1 );
        return 0;
    }

    char* tmp = strdup( filename );
    char* dir = dirname( tmp );

    if ( strlen(dir) > size ) {
        free( tmp);
        return -1;
    }

    strncpy( dirpath, dir, size );

    free( tmp);

    return 0;
}

MaxUnsignedNumberValidator::MaxUnsignedNumberValidator( const std::string& maxString, const std::string& test ) : maxString(maxString), test(test) {
    
    if ( !StringUtils::isNumber(maxString)) throw InvalidStringNumber( maxString );

}

MaxUnsignedNumberValidator::~MaxUnsignedNumberValidator() {
    
}

void MaxUnsignedNumberValidator::validate() {
    
    if (!StringUtils::isNumber( test )) throw InvalidStringNumber( test );
    if ( test.length() < maxString.length() ) return;
    if ( test.length() > maxString.length() ) throw OutOfRangeStringExp( test );

    for ( int i = 0; i < maxString.length(); i++) {
        if ( test.at(i) > maxString.at(i) ) throw OutOfRangeStringExp( test );
    }

}

bool StringUtils::isNumber( const std::string& s ) {
    return ::isNumber(s);
}

unsigned long StringUtils::toLongVal( const std::string& s ) {
    
    MaxUnsignedNumberValidator validator( MAX_ULONG_STR, s );

    validator.validate();

    return std::stoul( s.c_str());
}

void StringUtils::getDirPathFromFilePath( const std::string& filePath, std::string& folderPath ) {
    
    char dirpath[PATH_MAX];
    if ( getFolderFromFilepath( filePath.c_str(), dirpath, PATH_MAX ) < 0 ) throw InvalidSParams();
    folderPath = dirpath;
}
