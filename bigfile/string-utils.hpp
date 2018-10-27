#ifndef _STRING_UTILS_HPP_
#define _STRING_UTILS_HPP_

#include <string>
#include "app-exception.hpp"
#include "ivalidator.hpp"

class StringUtils {
public: 
    static bool isNumber( const std::string& s );
    static unsigned long toLongVal( const std::string& s );
    static void getDirPathFromFilePath( const std::string& filePath, std::string& folderPath );
};

class MaxUnsignedNumberValidator : public IValidator {
private:
    std::string maxString;
    std::string test;
public: 
    MaxUnsignedNumberValidator( const std::string& maxString, const std::string& test );
    ~MaxUnsignedNumberValidator();
    void validate();
};

class InvalidStringNumber : public AppException {
public:
    InvalidStringNumber(const std::string& message ) : AppException(message) {
    }
};

class InvalidUlongString : public AppException {
public:
    InvalidUlongString(const std::string& message ) : AppException(message) {}
    virtual ~InvalidUlongString() {}
};

class OutOfRangeStringExp : public AppException {
public:
    OutOfRangeStringExp(const std::string& message ) : AppException(message) {}
    virtual ~OutOfRangeStringExp() {}
};

class InvalidSParams : public AppException {
public:
    InvalidSParams() : AppException() {
    }
};
#endif 
