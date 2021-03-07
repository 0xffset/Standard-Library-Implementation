#define BOOST_TEST_MODULE Array_Test

#include <boost/test/included/unit_test.hpp>
#include <string>

#include <Array/Array.hpp>
#include <TestObj.hpp>

BOOST_AUTO_TEST_CASE(Default_Constructor) {
	oxf::array<int, 3> arr1;
	oxf::array<std::string, 3> arr2;

	BOOST_TEST(arr1.size() == 3);
	BOOST_TEST(arr2.size() == 3);
}

BOOST_AUTO_TEST_CASE(Initializer_List_Constructor) {
	oxf::array<int, 3> arr1({0, 1, 2});
	oxf::array<std::string, 3> arr2({"0", "1", "2"});

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(arr1[i] == i);
		BOOST_TEST(arr2[i] == std::to_string(i));
	}
}

BOOST_AUTO_TEST_CASE(Copy_Constructor) {
	oxf::array<int, 3> arr1({0, 1, 2});
	oxf::array<int, 3> arr1Copy(arr1);
	oxf::array<std::string, 3> arr2({"0", "1", "2"});
	oxf::array<std::string, 3> arr2Copy(arr2);

	BOOST_TEST(arr1Copy.size() == 3);
	BOOST_TEST(arr2Copy.size() == 3);
	BOOST_TEST(arr1.data() != arr1Copy.data());
	BOOST_TEST(arr2.data() != arr2Copy.data());

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(arr1[i] == arr1Copy[i]);
		BOOST_TEST(arr2[i] == arr2Copy[i]);
	}
}

BOOST_AUTO_TEST_CASE(Move_Constructor) {
	oxf::array<int, 3> arr1({0, 1, 2});
	int *arr1DataPointer = arr1.data();
	oxf::array<int, 3> arr1Move(std::move(arr1));
	oxf::array<std::string, 3> arr2({"0", "1", "2"});
	std::string *arr2DataPointer = arr2.data();
	oxf::array<std::string, 3> arr2Move(std::move(arr2));

	BOOST_TEST(arr1Move.size() == 3);
	BOOST_TEST(arr1Move.data() == arr1DataPointer);
	BOOST_TEST(arr2Move.size() == 3);
	BOOST_TEST(arr2Move.data() == arr2DataPointer);
	BOOST_TEST(arr1.data() == nullptr);
	BOOST_TEST(arr1.size() == 0);
	BOOST_TEST(arr2.data() == nullptr);
	BOOST_TEST(arr2.size() == 0);

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(arr1Move[i] == i);
		BOOST_TEST(arr2Move[i] == std::to_string(i));
	}
}

BOOST_AUTO_TEST_CASE(Destructor) {
	oxf::array<int, 3> arr1({0, 1, 2});
	oxf::array<std::string, 3> arr2({"0", "1", "2"});

	BOOST_CHECK_NO_THROW(arr1.~array());
	BOOST_CHECK_NO_THROW(arr2.~array());
}

BOOST_AUTO_TEST_CASE(Initializer_List_Assignment_Operator) {
	oxf::array<int, 3> arr1 = {0, 1, 2};
	oxf::array<std::string, 3> arr2 = {"0", "1", "2"};

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(arr1[i] == i);
		BOOST_TEST(arr2[i] == std::to_string(i));
	}
}

BOOST_AUTO_TEST_CASE(Copy_Assignment_Operator) {
	oxf::array<int, 3> arr1({0, 1, 2});
	oxf::array<int, 3> arr1Copy = arr1;
	oxf::array<std::string, 3> arr2({"0", "1", "2"});
	oxf::array<std::string, 3> arr2Copy = arr2;

	BOOST_TEST(arr1Copy.size() == 3);
	BOOST_TEST(arr2Copy.size() == 3);
	BOOST_TEST(arr1.data() != arr1Copy.data());
	BOOST_TEST(arr2.data() != arr2Copy.data());

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(arr1[i] == arr1Copy[i]);
		BOOST_TEST(arr2[i] == arr2Copy[i]);
	}
}

BOOST_AUTO_TEST_CASE(Move_Assignment_Operator) {
	oxf::array<int, 3> arr1({0, 1, 2});
	int *arr1DataPointer = arr1.data();
	oxf::array<int, 3> arr1Move = std::move(arr1);
	oxf::array<std::string, 3> arr2({"0", "1", "2"});
	std::string *arr2DataPointer = arr2.data();
	oxf::array<std::string, 3> arr2Move = std::move(arr2);

	BOOST_TEST(arr1Move.size() == 3);
	BOOST_TEST(arr1Move.data() == arr1DataPointer);
	BOOST_TEST(arr2Move.size() == 3);
	BOOST_TEST(arr2Move.data() == arr2DataPointer);
	BOOST_TEST(arr1.data() == nullptr);
	BOOST_TEST(arr1.size() == 0);
	BOOST_TEST(arr2.data() == nullptr);
	BOOST_TEST(arr2.size() == 0);

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(arr1Move[i] == i);
		BOOST_TEST(arr2Move[i] == std::to_string(i));
	}
}

BOOST_AUTO_TEST_CASE(Square_Bracket_Access_Operator) {
	oxf::array<int, 3> arr1({0, 1, 2});
	oxf::array<std::string, 3> arr2({"0", "1", "2"});

	BOOST_CHECK_NO_THROW(arr1[-1]);
	BOOST_CHECK_NO_THROW(arr1[1]);
	BOOST_CHECK_NO_THROW(arr1[5]);
	BOOST_CHECK_NO_THROW(arr2[-1]);
	BOOST_CHECK_NO_THROW(arr2[1]);
	BOOST_CHECK_NO_THROW(arr2[5]);

	arr1[0] = 9;
	arr2[0] = "9";

	BOOST_TEST(arr1[0] == 9);
	BOOST_TEST(arr2[0] == "9");
}

BOOST_AUTO_TEST_CASE(At_Access_Method) {
	oxf::array<int, 3> arr1({0, 1, 2});
	oxf::array<std::string, 3> arr2({"0", "1", "2"});

	BOOST_CHECK_THROW(arr1.at(-1), std::runtime_error);
	BOOST_CHECK_NO_THROW(arr1[1]);
	BOOST_CHECK_THROW(arr1.at(5), std::runtime_error);
	BOOST_CHECK_THROW(arr2.at(-1), std::runtime_error);
	BOOST_CHECK_NO_THROW(arr2[1]);
	BOOST_CHECK_THROW(arr2.at(5), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(Copy_Insert_Method) {
	oxf::array<int, 3> arr1({0, 3, 2});
	oxf::array<std::string, 3> arr2({"0", "3", "2"});
	oxf::array<testObj, 3> arr3({testObj(0), testObj(3), testObj(2)});

	int n = 1;
	std::string s = "1";
	testObj o(1);

	arr1.insert(1, n);
	arr2.insert(1, s);
	arr3.insert(1, o);
	BOOST_CHECK_THROW(arr1.insert(5, n), std::runtime_error);
	BOOST_CHECK_THROW(arr2.insert(5, s), std::runtime_error);
	BOOST_CHECK_THROW(arr3.insert(5, o), std::runtime_error);

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(arr1[i] == i);
		BOOST_TEST(arr2[i] == std::to_string(i));
	}

	BOOST_TEST(arr3[1].status == COPY_CONSTRUCTOR);
}

BOOST_AUTO_TEST_CASE(Move_Insert_Method) {
	oxf::array<int, 3> arr1({0, 3, 2});
	oxf::array<std::string, 3> arr2({"0", "3", "2"});
	oxf::array<testObj, 3> arr3({testObj(0), testObj(3), testObj(2)});

	arr1.insert(1, 1);
	arr2.insert(1, "1");
	arr3.insert(1, testObj(1));
	BOOST_CHECK_THROW(arr1.insert(5, 1), std::runtime_error);
	BOOST_CHECK_THROW(arr2.insert(5, "1"), std::runtime_error);
	BOOST_CHECK_THROW(arr3.insert(5, testObj(1)), std::runtime_error);

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(arr1[i] == i);
		BOOST_TEST(arr2[i] == std::to_string(i));
	}

	BOOST_TEST(arr3[1].status == MOVE_CONSTRUCTOR);
}

BOOST_AUTO_TEST_CASE(Emplace_Method) {
	oxf::array<int, 3> arr1({0, 3, 2});
	oxf::array<std::string, 3> arr2({"0", "3", "2"});
	oxf::array<testObj, 3> arr3({testObj(0), testObj(3), testObj(2)});

	arr1.emplace(1, 1);
	arr2.emplace(1, "1");
	arr3.emplace(1, 1);
	BOOST_CHECK_THROW(arr1.emplace(5, 1), std::runtime_error);
	BOOST_CHECK_THROW(arr2.emplace(5, "1"), std::runtime_error);
	BOOST_CHECK_THROW(arr3.emplace(5, 1), std::runtime_error);

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(arr1[i] == i);
		BOOST_TEST(arr2[i] == std::to_string(i));
	}

	BOOST_TEST(arr3[1].status == CONSTRUCTOR);
}

BOOST_AUTO_TEST_CASE(Swap_Method) {
	oxf::array<int, 3> arrInt1({0, 1, 2});
	oxf::array<int, 3> arrInt2({3, 4, 5});
	int* arrInt1DataPointer = arrInt1.data();
	int* arrInt2DataPointer = arrInt2.data();
	oxf::array<std::string, 3> arrString1({"0", "1", "2"});
	oxf::array<std::string, 3> arrString2({"3", "4", "5"});
	std::string* arrString1DataPointer = arrString1.data();
	std::string* arrString2DataPointer = arrString2.data();

	arrInt1.swap(arrInt2);
	arrString1.swap(arrString2);

	BOOST_TEST(arrInt1.data() == arrInt2DataPointer);
	BOOST_TEST(arrInt2.data() == arrInt1DataPointer);
	BOOST_TEST(arrString1.data() == arrString2DataPointer);
	BOOST_TEST(arrString2.data() == arrString1DataPointer);
}

BOOST_AUTO_TEST_CASE(Equality_Operators) {
	oxf::array<int, 3> arrInt1({0, 1, 2});
	oxf::array<int, 3> arrInt2({0, 4, 2});
	oxf::array<std::string, 3> arrString1({"0", "1", "2"});
	oxf::array<std::string, 3> arrString2({"0", "4", "2"});

	BOOST_TEST(!(arrInt1 == arrInt2));
	BOOST_TEST((arrInt1 != arrInt2));
	BOOST_TEST(!(arrString1 == arrString2));
	BOOST_TEST((arrString1 != arrString2));

	arrInt2[1] = 1;
	arrString2[1] = "1";

	BOOST_TEST((arrInt1 == arrInt2));
	BOOST_TEST(!(arrInt1 != arrInt2));
	BOOST_TEST((arrString1 == arrString2));
	BOOST_TEST(!(arrString1 != arrString2));
}
