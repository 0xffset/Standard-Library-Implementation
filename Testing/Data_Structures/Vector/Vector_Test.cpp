#define BOOST_TEST_MODULE Vector_Test

#include <boost/test/included/unit_test.hpp>
#include <string>
#include <cstdint>

#include <Vector/Vector.hpp>
#include <TestObj.hpp>

BOOST_AUTO_TEST_CASE(Default_Constructor) {
	oxf::vector<int> vec;

	BOOST_TEST(vec.capacity() == 2);
	BOOST_TEST(vec.size() == 0);
}

BOOST_AUTO_TEST_CASE(Size_Constructor) {
	oxf::vector<int> vec(5);

	BOOST_TEST(vec.capacity() == 5);
	BOOST_TEST(vec.size() == 0);
}

BOOST_AUTO_TEST_CASE(Initializer_List_Constructor) {
	oxf::vector<int> vec1({0, 1, 2});
	oxf::vector<std::string> vec2({"0", "1", "2"});

	BOOST_TEST(vec1.size() == 3);
	BOOST_TEST(vec1.capacity() == 3);

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(vec1[i] == i);
		BOOST_TEST(vec2[i] == std::to_string(i));
	}
}

BOOST_AUTO_TEST_CASE(Copy_Constructor) {
	oxf::vector<int> vec1Original({0, 1, 2});
	oxf::vector<int> vec1Copy(vec1Original);
	oxf::vector<std::string> vec2Original({"0", "1", "2"});
	oxf::vector<std::string> vec2Copy(vec2Original);

	BOOST_TEST(vec1Copy.size() == vec1Original.size());
	BOOST_TEST(vec1Copy.capacity() == vec1Original.capacity());
	BOOST_TEST(vec2Copy.size() == vec2Original.size());
	BOOST_TEST(vec2Copy.capacity() == vec2Original.capacity());

	// Different data pointers
	BOOST_TEST(vec1Original.data() != vec1Copy.data());
	BOOST_TEST(vec2Original.data() != vec2Copy.data());

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(vec1Original[i] == vec1Copy[i]);
		BOOST_TEST(vec2Original[i] == vec2Copy[i]);
	}
}

BOOST_AUTO_TEST_CASE(Move_Constructor) {
	oxf::vector<int> vec1Original({0, 1, 2});
	const int *vec1DataPointer = vec1Original.data();
	oxf::vector<int> vec1Move(std::move(vec1Original));
	oxf::vector<std::string> vec2Original({"0", "1", "2"});
	const std::string *vec2DataPointer = vec2Original.data();
	oxf::vector<std::string> vec2Move(std::move(vec2Original));

	BOOST_TEST(vec1Move.size() == 3);
	BOOST_TEST(vec1Move.capacity() == 3);
	BOOST_TEST(vec2Move.size() == 3);
	BOOST_TEST(vec2Move.capacity() == 3);
	BOOST_TEST(vec1Original.size() == 0);
	BOOST_TEST(vec1Original.capacity() == 0);
	BOOST_TEST(vec2Original.size() == 0);
	BOOST_TEST(vec2Original.capacity() == 0);

	// Correct Pointers
	BOOST_TEST(vec1Move.data() == vec1DataPointer);
	BOOST_TEST(vec1Original.data() == nullptr);
	BOOST_TEST(vec2Move.data() == vec2DataPointer);
	BOOST_TEST(vec2Original.data() == nullptr);

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(vec1Move[i] == i);
		BOOST_TEST(vec2Move[i] == std::to_string(i));
	}
}

BOOST_AUTO_TEST_CASE(Destructor) {
	oxf::vector<int> vec1({0, 1, 2});
	oxf::vector<std::string> vec2({"0", "1", "2"});

	BOOST_CHECK_NO_THROW(vec1.~vector());
	BOOST_CHECK_NO_THROW(vec2.~vector());
}

BOOST_AUTO_TEST_CASE(Copy_Assigment_Operator) {
	oxf::vector<int> vec1Original({0, 1, 2});
	oxf::vector<int> vec1Copy = vec1Original;
	oxf::vector<std::string> vec2Original({"0", "1", "2"});
	oxf::vector<std::string> vec2Copy = vec2Original;

	BOOST_TEST(vec1Copy.size() == 3);
	BOOST_TEST(vec1Copy.capacity() == 3);
	BOOST_TEST(vec2Copy.size() == 3);
	BOOST_TEST(vec2Copy.capacity() == 3);

	// Different data pointers
	BOOST_TEST(vec1Original.data() != vec1Copy.data());
	BOOST_TEST(vec2Original.data() != vec2Copy.data());

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(vec1Original[i] == vec1Copy[i]);
		BOOST_TEST(vec2Original[i] == vec2Copy[i]);
	}
}

BOOST_AUTO_TEST_CASE(Move_Assignment) {
	oxf::vector<int> vec1Original({0, 1, 2});
	const int *vec1DataPointer = vec1Original.data();
	oxf::vector<int> vec1Move = std::move(vec1Original);
	oxf::vector<std::string> vec2Original({"0", "1", "2"});
	const std::string *vec2DataPointer = vec2Original.data();
	oxf::vector<std::string> vec2Move = std::move(vec2Original);

	BOOST_TEST(vec1Move.size() == 3);
	BOOST_TEST(vec1Move.capacity() == 3);
	BOOST_TEST(vec2Move.size() == 3);
	BOOST_TEST(vec2Move.capacity() == 3);
	BOOST_TEST(vec1Original.size() == 0);
	BOOST_TEST(vec1Original.capacity() == 0);
	BOOST_TEST(vec2Original.size() == 0);
	BOOST_TEST(vec2Original.capacity() == 0);

	// Correct Pointers
	BOOST_TEST(vec1Move.data() == vec1DataPointer);
	BOOST_TEST(vec1Original.data() == nullptr);
	BOOST_TEST(vec2Move.data() == vec2DataPointer);
	BOOST_TEST(vec2Original.data() == nullptr);

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(vec1Move[i] == i);
		BOOST_TEST(vec2Move[i] == std::to_string(i));
	}
}

BOOST_AUTO_TEST_CASE(Initializer_List_Assignment_Operator) {
	oxf::vector<int> vec1 = {0, 1, 2};
	oxf::vector<std::string> vec2 = {"0", "1", "2"};

	BOOST_TEST(vec1.size() == 3);
	BOOST_TEST(vec1.capacity() == 3);

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(vec1[i] == i);
		BOOST_TEST(vec2[i] == std::to_string(i));
	}
}

BOOST_AUTO_TEST_CASE(Square_Bracket_Access_Operator) {
	oxf::vector<int> vec1 = {0, 1, 2};
	oxf::vector<std::string> vec2 = {"0", "1", "2"};

	BOOST_CHECK_NO_THROW(vec1[-1]);
	BOOST_CHECK_NO_THROW(vec1[0]);
	BOOST_CHECK_NO_THROW(vec1[3]);
	BOOST_CHECK_NO_THROW(vec2[-1]);
	BOOST_CHECK_NO_THROW(vec2[0]);
	BOOST_CHECK_NO_THROW(vec2[3]);

	vec1[0] = 9;
	vec2[0] = "9";
	BOOST_TEST(vec1[0] == 9);
	BOOST_TEST(vec2[0] == "9");
}

BOOST_AUTO_TEST_CASE(At_Access_Method) {
	oxf::vector<int> vec1 = {0, 1, 2};
	oxf::vector<std::string> vec2 = {"0", "1", "2"};

	BOOST_CHECK_THROW(vec1.at(-1), std::runtime_error);
	BOOST_CHECK_NO_THROW(vec1[0]);
	BOOST_CHECK_THROW(vec1.at(3), std::runtime_error);
	BOOST_CHECK_THROW(vec2.at(-1), std::runtime_error);
	BOOST_CHECK_NO_THROW(vec2[0]);
	BOOST_CHECK_THROW(vec2.at(-3), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(Capacity) {
	oxf::vector<int> vec1(4);
	oxf::vector<std::string> vec2(4);

	BOOST_TEST(vec1.empty());
	BOOST_TEST(vec2.empty());
	BOOST_TEST(vec1.size() == 0);
	BOOST_TEST(vec2.size() == 0);
	BOOST_TEST(vec1.capacity() == 4);
	BOOST_TEST(vec2.capacity() == 4);

	vec1.push_back(0);
	vec2.push_back("0");

	BOOST_TEST(!vec1.empty());
	BOOST_TEST(!vec2.empty());
	BOOST_TEST(vec1.size() == 1);
	BOOST_TEST(vec2.size() == 1);
	BOOST_TEST(vec1.capacity() == 4);
	BOOST_TEST(vec2.capacity() == 4);

	for (int i = 1; i < 5; ++i) {
		vec1.push_back(i);
		vec2.push_back(std::to_string(i));
	}

	BOOST_TEST(!vec1.empty());
	BOOST_TEST(!vec2.empty());
	BOOST_TEST(vec1.size() == 5);
	BOOST_TEST(vec2.size() == 5);
	BOOST_TEST(vec1.capacity() == 6);
	BOOST_TEST(vec2.capacity() == 6);

	BOOST_TEST(vec1.maxCapacity() == SIZE_MAX);
}

BOOST_AUTO_TEST_CASE(Reserve_Method) {
	oxf::vector<int> vec1(4);
	oxf::vector<std::string> vec2(4);

	vec1.reserve(2);
	vec2.reserve(2);

	BOOST_TEST(vec1.capacity() == 4);
	BOOST_TEST(vec2.capacity() == 4);

	vec1.reserve(5);
	vec2.reserve(5);

	BOOST_TEST(vec1.capacity() == 5);
	BOOST_TEST(vec2.capacity() == 5);
}

BOOST_AUTO_TEST_CASE(Shrink_To_Fit_Method) {
	oxf::vector<int> vec1(4);
	oxf::vector<std::string> vec2(4);

	vec1.push_back(1);
	vec1.shrink_to_fit();
	vec2.push_back("1");
	vec2.shrink_to_fit();

	// min size at all times 2
	BOOST_TEST(vec1.capacity() == 2);
	BOOST_TEST(vec2.capacity() == 2);

	vec1.reserve(4);
	vec1.push_back(2);
	vec1.push_back(3);
	vec1.shrink_to_fit();
	vec2.reserve(4);
	vec2.push_back("2");
	vec2.push_back("3");
	vec2.shrink_to_fit();

	BOOST_TEST(vec1.capacity() == 3);
	BOOST_TEST(vec2.capacity() == 3);

	BOOST_TEST(vec1[1] == 2);
	BOOST_CHECK_NO_THROW(vec1.at(2));
	BOOST_CHECK_THROW(vec1.at(3), std::runtime_error);
	BOOST_TEST(vec2[1] == "2");
	BOOST_CHECK_NO_THROW(vec2.at(2));
	BOOST_CHECK_THROW(vec2.at(3), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(Clear_Method) {
	oxf::vector<int> vec1({0, 1, 2});
	oxf::vector<std::string> vec2({"0", "1", "2"});

	vec1.clear();
	vec2.clear();

	BOOST_TEST(vec1.size() == 0);
	BOOST_TEST(vec1.capacity() == 3);
	BOOST_CHECK_THROW(vec1.at(0), std::runtime_error);

	BOOST_TEST(vec2.size() == 0);
	BOOST_TEST(vec2.capacity() == 3);
	BOOST_CHECK_THROW(vec2.at(0), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(Erase_Method) {
	oxf::vector<int> vec1({0, 1, 1, 2});
	oxf::vector<std::string> vec2({"0", "1", "1", "2"});

	vec1.erase(1);
	vec2.erase(1);

	BOOST_TEST(vec1.size() == 3);
	BOOST_TEST(vec1.capacity() == 4);
	BOOST_TEST(vec2.size() == 3);
	BOOST_TEST(vec2.capacity() == 4);

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(vec1[i] == i);
		BOOST_TEST(vec2[i] == std::to_string(i));
	}
}

#define INSERT_PUSH_BACK_EMPLACE_CAPACITY_CHECK \
BOOST_TEST(vec1.capacity() == 3); \
BOOST_TEST(vec1.size() == 3); \
BOOST_TEST(vec2.capacity() == 3); \
BOOST_TEST(vec2.size() == 3); \
BOOST_TEST(vec3.capacity() == 3); \
BOOST_TEST(vec3.size() == 3);

BOOST_AUTO_TEST_CASE(Copy_Insert_Method) {
	oxf::vector<int> vec1({0, 2});
	oxf::vector<std::string> vec2({"0", "2"});
	oxf::vector<testObj> vec3({testObj(0), testObj(2)});

	int n = 1;
	std::string s = "1";
	testObj o(1);
	vec1.insert(1, n);
	vec2.insert(1, s);
	vec3.insert(1, o);
	BOOST_CHECK_THROW(vec1.insert(5, n), std::runtime_error);
	BOOST_CHECK_THROW(vec2.insert(5, s), std::runtime_error);
	BOOST_CHECK_THROW(vec3.insert(5, o), std::runtime_error);

	INSERT_PUSH_BACK_EMPLACE_CAPACITY_CHECK

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(vec1[i] == i);
		BOOST_TEST(vec2[i] == std::to_string(i));
		BOOST_TEST(vec3[i].val == i);
	}

	BOOST_TEST(vec3[1].status == COPY_CONSTRUCTOR);
}

BOOST_AUTO_TEST_CASE(Move_Insert_Method) {
	oxf::vector<int> vec1({0, 2});
	oxf::vector<std::string> vec2({"0", "2"});
	oxf::vector<testObj> vec3({testObj(0), testObj(2)});

	vec1.insert(1, 1);
	vec2.insert(1, "1");
	vec3.insert(1, testObj(1));
	BOOST_CHECK_THROW(vec1.insert(5, 1), std::runtime_error);
	BOOST_CHECK_THROW(vec2.insert(5, "1"), std::runtime_error);
	BOOST_CHECK_THROW(vec3.insert(5, testObj(1)), std::runtime_error);

	INSERT_PUSH_BACK_EMPLACE_CAPACITY_CHECK

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(vec1[i] == i);
		BOOST_TEST(vec2[i] == std::to_string(i));
		BOOST_TEST(vec3[i].val == i);
	}

	BOOST_TEST(vec3[1].status == MOVE_CONSTRUCTOR);
}

BOOST_AUTO_TEST_CASE(Copy_Push_Back_Method) {
	oxf::vector<int> vec1({0, 1});
	oxf::vector<std::string> vec2({"0", "1"});
	oxf::vector<testObj> vec3({testObj(0), testObj(1)});

	int n = 2;
	std::string s = "2";
	testObj o(2);
	vec1.push_back(n);
	vec2.push_back(s);
	vec3.push_back(o);

	INSERT_PUSH_BACK_EMPLACE_CAPACITY_CHECK

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(vec1[i] == i);
		BOOST_TEST(vec2[i] == std::to_string(i));
		BOOST_TEST(vec3[i].val == i);
	}

	BOOST_TEST(vec3[2].status == COPY_CONSTRUCTOR);
}

BOOST_AUTO_TEST_CASE(Move_Push_Back_Method) {
	oxf::vector<int> vec1({0, 1});
	oxf::vector<std::string> vec2({"0", "1"});
	oxf::vector<testObj> vec3({testObj(0), testObj(1)});

	vec1.push_back(2);
	vec2.push_back("2");
	vec3.push_back(testObj(2));

	INSERT_PUSH_BACK_EMPLACE_CAPACITY_CHECK

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(vec1[i] == i);
		BOOST_TEST(vec2[i] == std::to_string(i));
		BOOST_TEST(vec3[i].val == i);
	}

	BOOST_TEST(vec3[2].status == MOVE_CONSTRUCTOR);
}

BOOST_AUTO_TEST_CASE(Pop_Back_Method) {
	oxf::vector<int> vec1({0, 1, 2});
	oxf::vector<std::string> vec2({"0", "1", "2"});

	vec1.pop_back();
	vec2.pop_back();

	BOOST_TEST(vec1.capacity() == 3);
	BOOST_TEST(vec1.size() == 2);
	BOOST_TEST(vec2.capacity() == 3);
	BOOST_TEST(vec2.size() == 2);
}

BOOST_AUTO_TEST_CASE(Swap_Method) {
	oxf::vector<int> vec1Original({0, 1, 2});
	const int *vec1OriginalDataPointer = vec1Original.data();
	oxf::vector<int> vec1Swap({3});
	const int *vec1SwapDataPointer = vec1Swap.data();
	oxf::vector<std::string> vec2Original({"0", "1", "2"});
	const std::string *vec2OriginalDataPointer = vec2Original.data();
	oxf::vector<std::string> vec2Swap({"3"});
	const std::string *vec2SwapDataPointer = vec2Swap.data();

	vec1Original.swap(vec1Swap);
	vec2Original.swap(vec2Swap);

	BOOST_TEST(vec1Swap.size() == 3);
	BOOST_TEST(vec1Swap.capacity() == 3);
	BOOST_TEST(vec2Swap.size() == 3);
	BOOST_TEST(vec2Swap.capacity() == 3);
	BOOST_TEST(vec1Original.size() == 1);
	BOOST_TEST(vec1Original.capacity() == 2);
	BOOST_TEST(vec2Original.size() == 1);
	BOOST_TEST(vec2Original.capacity() == 2);

	// Correct Pointers
	BOOST_TEST(vec1Swap.data() == vec1OriginalDataPointer);
	BOOST_TEST(vec1Original.data() == vec1SwapDataPointer);
	BOOST_TEST(vec2Swap.data() == vec2OriginalDataPointer);
	BOOST_TEST(vec2Original.data() == vec2SwapDataPointer);

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(vec1Swap[i] == i);
		BOOST_TEST(vec2Swap[i] == std::to_string(i));
	}

	BOOST_TEST(vec1Original[0] == 3);
	BOOST_CHECK_THROW(vec1Original.at(1), std::runtime_error);
	BOOST_TEST(vec2Original[0] == "3");
	BOOST_CHECK_THROW(vec2Original.at(1), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(Swap_Items_Method) {
	oxf::vector<int> vec1({0, 2, 1});
	oxf::vector<std::string> vec2({"0", "2", "1"});

	vec1.swapItems(1, 2);
	vec2.swapItems(1, 2);

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(vec1[i] == i);
		BOOST_TEST(vec2[i] == std::to_string(i));
	}

	BOOST_CHECK_THROW(vec1.swapItems(1, 5), std::runtime_error);
	BOOST_CHECK_THROW(vec2.swapItems(1, 5), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(Resize_Method) {
	oxf::vector<int> vec1({0, 1, 2});
	oxf::vector<std::string> vec2({"0", "1", "2"});

	vec1.resize(5);
	vec2.resize(5);

	BOOST_TEST(vec1.capacity() == 3);
	BOOST_TEST(vec1.size() == 3);
	BOOST_TEST(vec2.capacity() == 3);
	BOOST_TEST(vec2.size() == 3);

	vec1.resize(2);
	vec2.resize(2);

	BOOST_TEST(vec1.capacity() == 3);
	BOOST_TEST(vec1.size() == 2);
	BOOST_CHECK_THROW(vec1.at(2), std::runtime_error);
	BOOST_TEST(vec2.capacity() == 3);
	BOOST_TEST(vec2.size() == 2);
	BOOST_CHECK_THROW(vec2.at(2), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(Emplace_Back_Method) {
	oxf::vector<int> vec1({0, 1});
	oxf::vector<std::string> vec2({"0", "1"});
	oxf::vector<testObj> vec3({testObj(0), testObj(1)});

	vec1.emplace_back(2);
	vec2.emplace_back("2");
	vec3.emplace_back(2);

	INSERT_PUSH_BACK_EMPLACE_CAPACITY_CHECK

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(vec1[i] == i);
		BOOST_TEST(vec2[i] == std::to_string(i));
		BOOST_TEST(vec3[i].val == i);
	}

	BOOST_TEST(vec3[2].status == CONSTRUCTOR);
}

BOOST_AUTO_TEST_CASE(Emplace_Method) {
	oxf::vector<int> vec1({0, 2});
	oxf::vector<std::string> vec2({"0", "2"});
	oxf::vector<testObj> vec3({testObj(0), testObj(2)});

	vec1.emplace(1, 1);
	vec2.emplace(1, "1");
	vec3.emplace(1, 1);

	INSERT_PUSH_BACK_EMPLACE_CAPACITY_CHECK

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(vec1[i] == i);
		BOOST_TEST(vec2[i] == std::to_string(i));
		BOOST_TEST(vec3[i].val == i);
	}

	BOOST_TEST(vec3[1].status == CONSTRUCTOR);
}

BOOST_AUTO_TEST_CASE(Copy_Replace_Method) {
	oxf::vector<int> vec1({0, 3, 2});
	oxf::vector<std::string> vec2({"0", "3", "2"});
	oxf::vector<testObj> vec3({testObj(0), testObj(3), testObj(2)});

	int n = 1;
	std::string s = "1";
	const testObj o(1);
	vec1.replace(1, n);
	vec2.replace(1, s);
	vec3.replace(1, o);
	BOOST_CHECK_THROW(vec1.replace(5, n), std::runtime_error);
	BOOST_CHECK_THROW(vec2.replace(5, s), std::runtime_error);
	BOOST_CHECK_THROW(vec3.replace(5, o), std::runtime_error);

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(vec1[i] == i);
		BOOST_TEST(vec2[i] == std::to_string(i));
		BOOST_TEST(vec3[i].val == i);
	}

	BOOST_TEST(vec3[1].status == COPY_CONSTRUCTOR);
}

BOOST_AUTO_TEST_CASE(Move_Replace_Method) {
	oxf::vector<int> vec1({0, 3, 2});
	oxf::vector<std::string> vec2({"0", "3", "2"});
	oxf::vector<testObj> vec3({testObj(0), testObj(3), testObj(2)});

	vec1.replace(1, 1);
	vec2.replace(1, "1");
	vec3.replace(1, testObj(1));
	BOOST_CHECK_THROW(vec1.replace(5, 1), std::runtime_error);
	BOOST_CHECK_THROW(vec2.replace(5, "1"), std::runtime_error);
	BOOST_CHECK_THROW(vec3.replace(5, testObj(1)), std::runtime_error);

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(vec1[i] == i);
		BOOST_TEST(vec2[i] == std::to_string(i));
		BOOST_TEST(vec3[i].val == i);
	}

	BOOST_TEST(vec3[1].status == MOVE_CONSTRUCTOR);
}

BOOST_AUTO_TEST_CASE(Emplace_Replace_Method) {
	oxf::vector<int> vec1({0, 3, 2});
	oxf::vector<std::string> vec2({"0", "3", "2"});
	oxf::vector<testObj> vec3({testObj(0), testObj(3), testObj(2)});

	vec1.replace(1, 1);
	vec2.replace(1, "1");
	vec3.replace(1, 1);
	BOOST_CHECK_THROW(vec1.replace(5, 1), std::runtime_error);
	BOOST_CHECK_THROW(vec2.replace(5, "1"), std::runtime_error);
	BOOST_CHECK_THROW(vec3.replace(5, 1), std::runtime_error);

	for (int i = 0; i < 3; ++i) {
		BOOST_TEST(vec1[i] == i);
		BOOST_TEST(vec2[i] == std::to_string(i));
		BOOST_TEST(vec3[i].val == i);
	}

	BOOST_TEST(vec3[1].status == CONSTRUCTOR);
}

BOOST_AUTO_TEST_CASE(Equality_Operators) {
	oxf::vector<int> vecInt1({0, 1, 2});
	oxf::vector<int> vecInt2({0, 1});
	oxf::vector<std::string> vecString1({"0", "1", "2"});
	oxf::vector<std::string> vecString2({"0", "1"});

	BOOST_TEST(!(vecInt1 == vecInt2));
	BOOST_TEST((vecInt1 != vecInt2));
	BOOST_TEST(!(vecString1 == vecString2));
	BOOST_TEST((vecString1 != vecString2));

	vecInt2.emplace_back(2);
	vecString2.push_back("2");

	BOOST_TEST((vecInt1 == vecInt2));
	BOOST_TEST(!(vecInt1 != vecInt2));
	BOOST_TEST((vecString1 == vecString2));
	BOOST_TEST(!(vecString1 != vecString2));
}
