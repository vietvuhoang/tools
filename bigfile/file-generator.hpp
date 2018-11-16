#ifndef _FILE_GENERATOR_HPP_
#define _FILE_GENERATOR_HPP_

#include <string>
#include "app-exception.hpp"
#include "ivalidator.hpp"
#include "igenerator.hpp"
#include <functional>

// typedef void (*progress_cb_t)(unsigned long, unsigned long);
// typedef void (*generate_buffer_cb_t)(unsigned char*, unsigned int size);

using ProcessCb = std::function<void(unsigned long, unsigned long)>;
using GenerateBufferCb = std::function<void(unsigned char*, unsigned long size)>;

class FileGenerator : public IGenerator {

private:
    std::string name;
    unsigned long size;
    int fd;
    ProcessCb progressCb;
    GenerateBufferCb genCb;
    bool everytime;
    void release();
    void linkAt();
public: 
    static void checkFreeSpace( const std::string& folder, unsigned long size );
    FileGenerator( const std::string& name, unsigned long size, GenerateBufferCb pcb = NULL, ProcessCb gcb = NULL );
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
