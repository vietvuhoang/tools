#include "file-generator.hpp"
#include <string>
#include <gtest/gtest.h>
#include <limits.h>

#define TESTFOLDER ".test"
#define TESTFILE "test.dat"
#define TESTNEWFILE "newtest.dat"
#define TESTDUMMYDATA "TESTDUMMYDATA"

class FileValidatorTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
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

TEST_F(FileValidatorTest, file_is_already_existed)
{
    char filepath[PATH_MAX];

    snprintf(filepath, PATH_MAX, "%s/%s", TESTFOLDER, TESTFILE);

    EXPECT_THROW({
        FilePathValidator validator(filepath);

        validator.validate();
    },
                 FileExistedExp);
}

TEST_F(FileValidatorTest, filepath_is_empty)
{
    char filepath[PATH_MAX] = "";

    EXPECT_THROW({
        FilePathValidator validator(filepath);

        validator.validate();
    },
                 InvalidFilePathExp);
}

TEST_F(FileValidatorTest, invalid_folder)
{
    char filepath[PATH_MAX] = "";

    snprintf(filepath, PATH_MAX, "%s/wtf/%s", TESTFOLDER, TESTFILE);

    EXPECT_THROW({
        FilePathValidator validator(filepath);

        validator.validate();
    },
                 InvalidFolderExp);
}

TEST_F(FileValidatorTest, valid_new_file)
{
    char filepath[PATH_MAX] = "";

    snprintf(filepath, PATH_MAX, "%s/%s", TESTFOLDER, TESTNEWFILE);

    EXPECT_NO_THROW({
        FilePathValidator validator(filepath);

        validator.validate();
    });
}
