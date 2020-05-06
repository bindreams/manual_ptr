#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <zh/manual_ptr.hpp>

TEST_CASE("Unit.MakeManual") {
	zh::make_manual<int>(5);
}

TEST_CASE("Unit.DefaultCtor") {
	auto ptr1 = zh::manual_ptr<int>{};
	auto ptr2 = zh::manual_ptr<int>{nullptr};

	CHECK(!ptr1.has_deleter());
	CHECK(!ptr2.has_deleter());
}

TEST_CASE("Unit.AdoptCtor") {
	int* danger = new int(5);

	auto ptr = zh::manual_ptr<int>{danger};

	CHECK(ptr.has_deleter());
}

TEST_CASE("Unit.CopyCtor") {
	auto ptr = zh::make_manual<int>(5);
	auto copied = ptr;

	CHECK(ptr.has_deleter());
	CHECK(!copied.has_deleter());
}

TEST_CASE("Unit.MoveCtor") {
	auto ptr = zh::make_manual<int>(5);
	auto moved = std::move(ptr);

	CHECK(!ptr.has_deleter());
	CHECK(moved.has_deleter());
}

TEST_CASE("Unit.CopyAssignment") {
	auto ptr = zh::make_manual<int>(5);
	zh::manual_ptr<int> copied;
	copied = ptr;

	CHECK(ptr.has_deleter());
	CHECK(!copied.has_deleter());
}

TEST_CASE("Unit.MoveAssignment") {
	auto ptr = zh::make_manual<int>(5);
	zh::manual_ptr<int> moved;
	moved = std::move(ptr);

	CHECK(!ptr.has_deleter());
	CHECK(moved.has_deleter());
}
