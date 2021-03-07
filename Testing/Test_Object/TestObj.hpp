#ifndef CPP_STD_LIB_TESTOBJ_HPP
#define CPP_STD_LIB_TESTOBJ_HPP

#ifdef MANUAL_DEBUG
#include <iostream>
#endif

#define CONSTRUCTOR 1
#define COPY_CONSTRUCTOR 2
#define MOVE_CONSTRUCTOR 3
#define COPY_ASSIGNMENT 4
#define MOVE_ASSIGNMENT 5

class testObj {
public:
	int val = 0;
	unsigned int status = 0;

	explicit testObj(int i) : val(i) {
		status = CONSTRUCTOR;
#ifdef MANUAL_DEBUG
		std::cout << "[" << i << "]" << " Constructor" << std::endl;
#endif
	}

	testObj(const testObj &o) : val(o.val) {
		status = COPY_CONSTRUCTOR;
#ifdef MANUAL_DEBUG
		std::cout << "[" << val << "]" << " Copy Constructor" << std::endl;
#endif
	}

	testObj(testObj &&o) noexcept: val(o.val) {
		status = MOVE_CONSTRUCTOR;
#ifdef MANUAL_DEBUG
		std::cout << "[" << val << "]" << " Move Constructor" << std::endl;
#endif
	}

	testObj &operator=(const testObj &o) {
		if (this != &o) {
			val = o.val;
			status = COPY_ASSIGNMENT;
#ifdef MANUAL_DEBUG
			std::cout << "[" << val << "]" << " Copy Assignment" << std::endl;
#endif
		}

		return *this;
	}

	testObj &operator=(testObj &&o) noexcept {
		if (this != &o) {
			val = o.val;
			status = MOVE_ASSIGNMENT;
#ifdef MANUAL_DEBUG
			std::cout << "[" << val << "]" << " Move Assignment" << std::endl;
#endif
		}

		return *this;
	}
};

#endif //CPP_STD_LIB_TESTOBJ_HPP
