#include "file-generator.hpp"
#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <limits.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TESTFOLDER ".test"
#define TESTFILE "test.dat"
#define TESTNEWFILE "newtest.dat"
#define TESTDUMMYDATA "TESTDUMMYDATA"

#define GB (1024 * 1024 * 1024L)

class MockProgress {
public:
    MOCK_METHOD2( update, void(unsigned long,unsigned long));
};

class MockGen {
public:
    MOCK_METHOD2( generate, void(unsigned char*, unsigned int));
};

static MockProgress* gmockProgress;
static MockGen* gmockGen;

class FileGeneratorTest : public ::testing::Test
{
  protected:
    
    std::string s;

    void SetUp() override
    {
        
        s = "Hello World";

        char cmd[PATH_MAX];
        snprintf(cmd, PATH_MAX, "mkdir -p %s", TESTFOLDER);
        int ret = system(cmd);

        ASSERT_EQ(ret, 0);

        snprintf(cmd, PATH_MAX, "echo '%s' > %s/%s", TESTDUMMYDATA, TESTFOLDER, TESTFILE);
        ret = system(cmd);

        ASSERT_EQ(ret, 0);
    }

    void TearDown() override
    {
        char cmd[PATH_MAX];
        snprintf(cmd, PATH_MAX, "rm -rf %s", TESTFOLDER);
        int ret = system(cmd);
        ASSERT_EQ(ret, 0);
    }
};

TEST_F(FileGeneratorTest, check_disk_space)
{
    char filepath[PATH_MAX];

    struct statvfs info;

    snprintf(filepath, PATH_MAX, "%s", TESTFOLDER);

    int ret = statvfs(filepath, &info);

    if (ret < 0)
    {
        FAIL() << "Could not check diskspace";
    }

    std::cout << "Block Size " << info.f_bsize << std::endl;
    std::cout << "Block Free " << info.f_bfree << std::endl;
}

TEST_F(FileGeneratorTest, generate_BIG_file)
{
    char filepath[PATH_MAX];

    snprintf(filepath, PATH_MAX, "%s/%s", TESTFOLDER, TESTNEWFILE);

    unsigned long size_to_write = 5 * GB + 1;

    EXPECT_NO_THROW({
        FileGenerator gen(filepath, size_to_write);
        gen.generate();
    });

    int fd = open(filepath, O_RDONLY);

    ASSERT_GT( fd, 0 );

    lseek( fd, 0, SEEK_SET );

    off_t size =lseek( fd, 0, SEEK_END );

    EXPECT_EQ( size, size_to_write );
}

TEST_F(FileGeneratorTest, generate_file_with_progress_bar )
{
    char filepath[PATH_MAX];

    snprintf(filepath, PATH_MAX, "%s/%s", TESTFOLDER, TESTNEWFILE);

    unsigned long size_to_write = GB + 5;


    MockProgress mockProgress;
    MockGen mockGen;

    gmockProgress = &mockProgress;
    gmockGen = &mockGen;

    EXPECT_CALL( mockGen, generate( ::testing::_, ::testing::_)).Times( 1 );
    EXPECT_CALL( mockProgress, update( ::testing::_, size_to_write)).Times( ::testing::AtLeast(1));

    EXPECT_NO_THROW({
        FileGenerator gen(filepath, size_to_write, [](unsigned char* buff, unsigned int size ) {
            gmockGen->generate( buff, size );
        }, []( unsigned long total, unsigned long size ) {
            gmockProgress->update( total, size );
        } );
        gen.generate();
    });

    int fd = open(filepath, O_RDONLY);

    ASSERT_GT( fd, 0 );

    lseek( fd, 0, SEEK_SET );

    off_t size =lseek( fd, 0, SEEK_END );

    EXPECT_EQ( size, size_to_write );
}
