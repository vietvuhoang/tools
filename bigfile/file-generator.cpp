#include "file-generator.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <limits.h>
#include "string-utils.hpp"
#include <sys/statvfs.h>

#include <iostream>

#define MB (1024*1024)

#define _10MB (10*MB)

FileGenerator::FileGenerator(const std::string &name, unsigned long size) : name(name), size(size), fd(0)
{
    if (size == 0) throw InvalidFileSizeExp();
    FilePathValidator validator(name);
    validator.validate();
}

FileGenerator::~FileGenerator()
{
    this->release();
}

void FileGenerator::release()
{
    if (fd > 0)
    {
        close(fd);
        fd = 0;
    }

    sync();
}

void FileGenerator::generate()
{    
    std::string folder;

    StringUtils::getDirPathFromFilePath(name, folder);

    FileGenerator::checkFreeSpace( folder, size );

    int tmpFile = open(folder.c_str(), O_TMPFILE | O_RDWR, S_IRUSR | S_IWUSR);

    if (tmpFile < 0 ) {
        char err[PATH_MAX];
        snprintf( err, PATH_MAX, "[%s] %s", folder.c_str(), strerror(errno) );
        throw CouldNotOpenFileExp( err );
    }   

    this->fd = tmpFile;

    unsigned long total = 0;

    unsigned char buff[ 4 * MB];

    do {

        size_t sz = 0;
        
        if ( (size - total) > sizeof(buff) ) {
            sz = sizeof(buff);
        } else {
            sz = size - total;
        }

        int ret = write( fd, buff, sz );
        
        if ( ret < 0 ) throw CouldNotCompleteWriteJobExp();

        total += ret;

    } while ( total < size );

    sync();
    this->linkAt();
    this->release();
}

void FileGenerator::linkAt() {
    char path[PATH_MAX];
    snprintf(path, PATH_MAX,  "/proc/self/fd/%d", fd);
    int ret = linkat( AT_FDCWD, path, AT_FDCWD, name.c_str(), AT_SYMLINK_FOLLOW );
    if ( ret < 0 ) throw CouldNotCompleteWriteJobExp();
}

FilePathValidator::FilePathValidator(const std::string &name) : name(name)
{
}

void FilePathValidator::validate()
{
    if (name.length() == 0)
        throw InvalidFilePathExp(name);

    int ret = access(name.c_str(), F_OK);

    if (ret >= 0)
        throw FileExistedExp(name);
    if (ret < 0 && errno != ENOENT)
        throw InvalidFilePathExp(name);

    std::string dirPath;

    StringUtils::getDirPathFromFilePath(name, dirPath);

    ret = access(dirPath.c_str(), F_OK | W_OK | X_OK);

    if (ret < 0)
        throw InvalidFolderExp(name);
}

void FileGenerator::checkFreeSpace( const std::string& folder, unsigned long size ) {
    struct statvfs info;

    int ret = statvfs( folder.c_str() , &info );

    if ( ret < 0 ) throw CouldNotCheckFolderExp( folder );

    if ( (info.f_bsize * info.f_bfree) < size ) throw SpaceNotEnoughtExp();
}
