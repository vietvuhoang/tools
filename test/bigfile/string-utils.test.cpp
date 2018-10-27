#include "string-utils.hpp"
#include <string>
#include <gtest/gtest.h>
#include <limits.h>

#define MAX_ULONG_STR "4294967295"

const char* getMaxUlongStr();
void validateUlongString( const std::string& s );
int getFolderFromFilepath( const char* filename, char* dirpath, int size );

TEST( StringUtils, isNumber_normal ) {
    EXPECT_NO_THROW( {
        std::string sNum = "123456";
        EXPECT_TRUE( StringUtils::isNumber(sNum) );
    });
}

TEST( StringUtils, isNumber_invalid ) {
    EXPECT_NO_THROW( {
        std::string sNum = "123a456";
        EXPECT_FALSE( StringUtils::isNumber(sNum) );
    });
}

TEST( StringUtils, isNumber_blank ) {
    EXPECT_NO_THROW( {
        EXPECT_FALSE( StringUtils::isNumber( "" ) );
    });
}

TEST( MaxUnsignedNumberValidator, invalid_test_string ) {
    EXPECT_THROW( {
        MaxUnsignedNumberValidator validator( MAX_ULONG_STR, "123a456" );
        validator.validate();
    }, InvalidStringNumber );
}

TEST( MaxUnsignedNumberValidator, string_too_long ) {
    EXPECT_THROW( {
        MaxUnsignedNumberValidator validator( MAX_ULONG_STR, "42949672957777" );
        validator.validate();
    }, OutOfRangeStringExp );
}


TEST( MaxUnsignedNumberValidator, over_value ) {
    EXPECT_THROW( {
        MaxUnsignedNumberValidator validator( MAX_ULONG_STR, "4294968295" );
        validator.validate();
    }, OutOfRangeStringExp );
}

TEST( MaxUnsignedNumberValidator, valid ) {
    EXPECT_NO_THROW( {
        MaxUnsignedNumberValidator validator( MAX_ULONG_STR, "4294967295" );
        validator.validate();
    });
}

TEST( StringUtils, toLongVal ) {
    EXPECT_NO_THROW( {
        EXPECT_EQ( StringUtils::toLongVal("18446744073709551615") , 18446744073709551615UL );
        EXPECT_EQ( StringUtils::toLongVal("1234"), 1234 );
    });
}

TEST( StringUtils_getFolderFromFilepath, filepath_is_a_file ) {
    EXPECT_NO_THROW( {

        char filePath[ PATH_MAX ] = "/opt/google/chrome/file";
        char dirPath[ PATH_MAX ];

        ASSERT_EQ( ::getFolderFromFilepath( filePath, dirPath, PATH_MAX ), 0);
        EXPECT_STREQ( dirPath, "/opt/google/chrome");
    });
}

TEST( StringUtils_getFolderFromFilepath, filepath_is_a_folder ) {
    EXPECT_NO_THROW( {

        char filePath[ PATH_MAX ] = "/opt/google/chrome/";
        char dirPath[ PATH_MAX ];

        ASSERT_EQ( ::getFolderFromFilepath( filePath, dirPath, PATH_MAX ), 0);
        EXPECT_STREQ( dirPath, "/opt/google/chrome");
    });
}

TEST( StringUtils_getFolderFromFilepath, filepath_is_in_current_folder ) {
    EXPECT_NO_THROW( {

        char filePath[ PATH_MAX ] = "abc.txt";
        char dirPath[ PATH_MAX ];

        ASSERT_EQ( ::getFolderFromFilepath( filePath, dirPath, PATH_MAX ), 0);
        EXPECT_STREQ( dirPath, ".");
    });
}

TEST( StringUtils_getFolderFromFilepath, filepath_empty ) {
    EXPECT_NO_THROW( {

        char filePath[ PATH_MAX ] = "";
        char dirPath[ PATH_MAX ];

        ASSERT_EQ( ::getFolderFromFilepath( filePath, dirPath, PATH_MAX ), 0);
        EXPECT_STREQ( dirPath, ".");
    });
}

TEST( StringUtils_getFolderFromFilepath, for_invalid_params ) {
    EXPECT_NO_THROW( {
        char filePath[ PATH_MAX ] = "";
        char dirPath[ PATH_MAX ];
        ASSERT_EQ( ::getFolderFromFilepath( NULL, dirPath, PATH_MAX ), -1);
        ASSERT_EQ( ::getFolderFromFilepath( filePath, NULL, PATH_MAX ), -1);
        ASSERT_EQ( ::getFolderFromFilepath( filePath, dirPath, 1 ), -1);
    });
}

TEST( StringUtils_getFolderFromFilepath, dirPath_too_small ) {
    EXPECT_NO_THROW( {
        char filePath[ PATH_MAX ] = "/opt/google/chrome/";
        char dirPath[ 15 ];
        ASSERT_EQ( ::getFolderFromFilepath( filePath, dirPath, sizeof(dirPath) ), -1);
    });
}

TEST( StringUtils_getDirPathFromFilePath, normal_case ) {
    EXPECT_NO_THROW( {
        std::string filePath =  "/opt/google/chrome/aaaa";
        std::string dirPath;

        StringUtils::getDirPathFromFilePath( filePath, dirPath );

        EXPECT_STREQ( "/opt/google/chrome", dirPath.c_str() );
    });
}
