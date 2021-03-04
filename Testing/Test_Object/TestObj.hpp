#ifndef CPP_STD_LIB_TESTOBJ_HPP
#define CPP_STD_LIB_TESTOBJ_HPP

#define CONSTRUCTOR 1
#define COPY_CONSTRUCTOR 2
#define MOVE_CONSTRUCTOR 3

class testObj {
public:
	int val = 0;
	unsigned int status = 0;

	explicit testObj(int i) : val(i) {
		status = 1;
	}

	testObj(const testObj &o) : val(o.val) {
		status = 2;
	}

	testObj(testObj &&o) noexcept: val(o.val) {
		status = 3;
	}
};

#endif //CPP_STD_LIB_TESTOBJ_HPP
