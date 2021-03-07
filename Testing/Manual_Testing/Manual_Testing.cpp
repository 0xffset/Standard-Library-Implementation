#include <iostream>
#include <Vector/Vector.hpp>

#define MANUAL_DEBUG
#include <TestObj.hpp>

int main() {
	oxf::vector<testObj> vec({testObj(0), testObj(1), testObj(2)});
	std::cout << " - Post Constructor -" << std::endl;
	oxf::vector<testObj> vec2(std::move(vec));

	for (const auto& i : vec2) {
		std::cout << "[" << i.val << "] " << i.status << std::endl;
	}
}