#include "gtest/gtest.h"

#include "test_itself.cc"

TEST(test_itself_test, eq) {
	ASSERT_EQ(1, add(1, 0));
	ASSERT_EQ(5, add(2, 3));
//	ASSERT_EQ(7, add(2, 3)); // will fail
}

TEST(test_itself_test, ne) {
	ASSERT_NE(0, add(1, 0));
	ASSERT_NE(4, add(2, 3));
//	ASSERT_NE(5, add(2, 3)); // will fail
}

