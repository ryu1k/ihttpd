#include "gtest/gtest.h"

#include "test_itself_b.cc"

TEST(test_itself_test_b, eq_ne) {
	ASSERT_EQ(2, sub(5, 3));
	ASSERT_NE(5, sub(2, 3));
//	ASSERT_EQ(7, add(2, 3)); // will fail
}

