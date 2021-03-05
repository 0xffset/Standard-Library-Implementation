#include <iostream>
#include <Array/Array.hpp>
#include <TestObj.hpp>
#include <typeinfo>

template<class T>
class test {
public:
	T val;

	explicit test(T v) : val(std::move(v)) {}
};

int main() {
	oxf::array<std::string, 5> arr = {std::string("0"), std::string("1"), std::string("__"), std::string("3"),std::string("4")};

	arr.insert(2, std::string("2"));
	/*oxf::array<testObj, 5> arr = {testObj(0), testObj(1), testObj(-1), testObj(3), testObj(4)};
	arr.insert(2, testObj(2));*/

	test<std::string> test1("Hi");
	std::cout << typeid("test1.val").name() << std::endl;
}