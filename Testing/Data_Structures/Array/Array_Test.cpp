#define BOOST_TEST_MODULE Array_Test

#include <boost/test/included/unit_test.hpp>
#include <string>

#include <Array/Array.hpp>
#include <TestObj.hpp>

BOOST_AUTO_TEST_CASE(Initializer_List_Constructor) {
	oxf::array<int, 5> arr({0, 1, 2, 3, 4});

	for (size_t i = 0; i < 5; ++i) {
		BOOST_TEST(arr.at(i) == i);
	}
}

BOOST_AUTO_TEST_CASE(Initializer_List_Assignment) {
	oxf::array<int, 5> arr = {0, 1, 2, 3, 4};

	for (size_t i = 0; i < 5; ++i) {
		BOOST_TEST(arr.at(i) == i);
	}
}

BOOST_AUTO_TEST_CASE(Copy_Constructor) {
	oxf::array<int, 5> original({1, 2, 3, 4, 5});
	oxf::array<int, 5> copy(original);

	for (size_t i = 0; i < 5; ++i) {
		BOOST_TEST(original.at(i) == copy.at(i));
	}
}

BOOST_AUTO_TEST_CASE(Copy_Assignment) {
	oxf::array<int, 5> original({1, 2, 3, 4, 5});
	oxf::array<int, 5> copy = original;

	for (size_t i = 0; i < 5; ++i) {
		BOOST_TEST(original.at(i) == copy.at(i));
	}
}

BOOST_AUTO_TEST_CASE(Move_Constructor) {
	oxf::array<int, 5> original({0, 1, 2, 3, 4});
	oxf::array<int, 5> copy(std::move(original));

	for (size_t i = 0; i < 5; ++i) {
		BOOST_TEST(copy.at(i) == i);
	}
}

BOOST_AUTO_TEST_CASE(Move_Assignment) {
	oxf::array<int, 5> original({0, 1, 2, 3, 4});
	oxf::array<int, 5> copy = std::move(original);

	for (size_t i = 0; i < 5; ++i) {
		BOOST_TEST(copy.at(i) == i);
	}
}

BOOST_AUTO_TEST_CASE(Square_Bracket_Access) {
	oxf::array<int, 5> arr({0, 1, 2, 3, 4});

	for (size_t i = 0; i < 5; ++i) {
		BOOST_TEST(arr[i] == i);
	}
}

BOOST_AUTO_TEST_CASE(Iterators) {
	oxf::array<int, 5> arr({0, 1, 2, 3, 4});

	int confirmArr[] = {0, 1, 2, 3, 4};
	size_t count = 0;

	for (auto i : arr) {
		BOOST_TEST(i == confirmArr[count]);
		++count;
	}
}

BOOST_AUTO_TEST_CASE(Size) {
	oxf::array<int, 5> arr({0, 1, 2, 3, 4});
	BOOST_TEST(arr.size() == 5);
}

BOOST_AUTO_TEST_CASE(Copy_Insert) {
	oxf::array<std::string, 5> arr{"0", "1", "__", "3", "4"};
	std::string s = "2";
	arr.insert(2, s);

	BOOST_TEST(arr.at(1) == "1");
	BOOST_TEST(arr.at(2) == "2");
	BOOST_TEST(arr.at(3) == "3");
}


BOOST_AUTO_TEST_CASE(Move_Insert) {
	oxf::array<std::string, 5> arr{"0", "1", "__", "3", "4"};
	arr.insert(2, "2");

	BOOST_TEST(arr.at(1) == "1");
	BOOST_TEST(arr.at(2) == "2");
	BOOST_TEST(arr.at(3) == "3");
}

BOOST_AUTO_TEST_CASE(Equals) {
	oxf::array<int, 5> one({0, 1, 2, 3, 4});
	oxf::array<int, 5> two({0, 1, 2, 3, 4});

	BOOST_TEST((one == two));
	two.insert(2, 9);
	BOOST_TEST(!(one == two));
}

BOOST_AUTO_TEST_CASE(Swap) {
	oxf::array<int, 5> one({0, 1, 2, 3, 4});
	oxf::array<int, 5> two({5, 6, 7, 8, 9});

	one.swap(two);
	for (size_t i = 0; i < 5; ++i) {
		BOOST_TEST(two.at(i) == i);
	}

	for (size_t i = 5; i < 10; ++i) {
		BOOST_TEST(one.at(i - 5) == i);
	}
}

BOOST_AUTO_TEST_CASE(emplace) {
	oxf::array<testObj, 2> arr;
	arr.emplace(0, 0);
	arr.emplace(1, 1);
	BOOST_TEST((arr.at(0).status == CONSTRUCTOR && arr.at(0).val == 0));
	BOOST_TEST((arr.at(1).status == CONSTRUCTOR && arr.at(1).val == 1));
}

BOOST_AUTO_TEST_CASE(Clear_Insert) {
	oxf::array<std::string, 5> arr{"0", "1", "__", "3", "4"};
	arr.clear();
	arr.insert(2, "2");

	BOOST_TEST(arr.at(1) != "1");
	BOOST_TEST(arr.at(2) == "2");
	BOOST_TEST(arr.at(3) != "3");
}
