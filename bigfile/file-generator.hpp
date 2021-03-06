#ifndef _FILE_GENERATOR_HPP_
#define _FILE_GENERATOR_HPP_

#include <string>
#include "app-exception.hpp"
#include "ivalidator.hpp"
#include "igenerator.hpp"

class FileGenerator : public IGenerator {

private:
    std::string name;
    unsigned long size;
    int fd;
    void release();
    void linkAt();
public: 
    static void checkFreeSpace( const std::string& folder, unsigned long size );
    FileGenerator( const std::string& name, unsigned long size );
    virtual ~FileGenerator();
    void generate();
};

class FilePathValidator : public IValidator {
private:
    std::string name;
public:
    FilePathValidator( const std::string& name );
    void validate();
};


class InvalidFileExp : public AppException {
public:
    InvalidFileExp() : AppException() {}
    InvalidFileExp(const std::string& path) : AppException(path) {}
    virtual ~InvalidFileExp() {}
};


class InvalidFileSizeExp : public InvalidFileExp {
public:
    InvalidFileSizeExp() : InvalidFileExp() {}
    virtual ~InvalidFileSizeExp() {}
};

class InvalidFilePathExp : public InvalidFileExp {
public:
    InvalidFilePathExp(const std::string& path) : InvalidFileExp( path ) {}
    virtual ~InvalidFilePathExp() {}
};

class CouldNotOpenFileExp : public InvalidFileExp {
public:
    CouldNotOpenFileExp(const std::string& path) : InvalidFileExp( path ) {}
    virtual ~CouldNotOpenFileExp() {}
};

class CouldNotCheckFolderExp : public InvalidFileExp {
public:
    CouldNotCheckFolderExp(const std::string& path) : InvalidFileExp( path ) {}
    virtual ~CouldNotCheckFolderExp() {}
};


class SpaceNotEnoughtExp : public InvalidFileExp {
public:
    SpaceNotEnoughtExp() : InvalidFileExp() {}
    virtual ~SpaceNotEnoughtExp() {}
};


class CouldNotCompleteWriteJobExp : public InvalidFileExp {
public:
    CouldNotCompleteWriteJobExp() : InvalidFileExp() {}
    virtual ~CouldNotCompleteWriteJobExp() {}
};

class FileExistedExp : public InvalidFilePathExp {
  public:
    FileExistedExp(const std::string& path) : InvalidFilePathExp( path ) {}
    virtual ~FileExistedExp() {}  
};


class InvalidFolderExp : public InvalidFilePathExp {
  public:
    InvalidFolderExp(const std::string& path) : InvalidFilePathExp( path ) {}
    virtual ~InvalidFolderExp() {}  
};

#endif
