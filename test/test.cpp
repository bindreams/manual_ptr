#include <gtest/gtest.h>

#include "manual_ptr.hpp"

TEST(manual_ptr_test, make_manual) {
	zh::make_manual<int>(5);
}

TEST(manual_ptr_constructors_test, default_ctor) {
	auto ptr1 = zh::manual_ptr<int>{};
	auto ptr2 = zh::manual_ptr<int>{nullptr};

	EXPECT_FALSE(ptr1.has_deleter());
	EXPECT_FALSE(ptr2.has_deleter());
}

TEST(manual_ptr_constructors_test, adopt_ctor) {
	int* danger = new int(5);

	auto ptr = zh::manual_ptr<int>{danger};

	EXPECT_TRUE(ptr.has_deleter());
}

TEST(manual_ptr_constructors_test, copy_ctor) {
	auto ptr = zh::make_manual<int>(5);
	auto copied = ptr;

	EXPECT_TRUE (ptr.has_deleter());
	EXPECT_FALSE(copied.has_deleter());
}

TEST(manual_ptr_constructors_test, move_ctor) {
	auto ptr = zh::make_manual<int>(5);
	auto moved = std::move(ptr);

	EXPECT_FALSE(ptr.has_deleter());
	EXPECT_TRUE (moved.has_deleter());
}

TEST(manual_ptr_assign_test, copy_assign) {
	auto ptr = zh::make_manual<int>(5);
	zh::manual_ptr<int> copied;
	copied = ptr;

	EXPECT_TRUE (ptr.has_deleter());
	EXPECT_FALSE(copied.has_deleter());
}

TEST(manual_ptr_assign_test, move_assign) {
	auto ptr = zh::make_manual<int>(5);
	zh::manual_ptr<int> moved;
	moved = std::move(ptr);

	EXPECT_FALSE(ptr.has_deleter());
	EXPECT_TRUE (moved.has_deleter());
}

int main() {
	testing::InitGoogleTest();
	int result = RUN_ALL_TESTS();

	//system("PAUSE");
	return result;
}