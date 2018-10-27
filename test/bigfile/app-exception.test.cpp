#include "app-exception.hpp"
#include <gtest/gtest.h>

#define MESSAGE "my message"

TEST(AppException, test_throw_with_message ) {
    EXPECT_THROW( {
        try {
            throw AppException(MESSAGE);
        } catch (const AppException& e) {
            EXPECT_STREQ( MESSAGE, e.what() );
            throw;
        }
    }, AppException );
}

TEST(AppException, test_throw_without_message ) {
    EXPECT_THROW( {
        try {
            throw AppException();
        } catch (const AppException& e) {
            EXPECT_STREQ( "", e.what() );
            throw;
        }
    }, AppException );
}

