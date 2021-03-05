#ifndef OXF_ARRAY_HPP
#define OXF_ARRAY_HPP

#include <iterator>
#include <utility>
#include <stdexcept>

#define OXF_ARRAY_ASSERT_INDEX(i)  if (i >= bufferSize) { throw std::runtime_error("val out of range"); }
#define OXF_ARRAY_INTERNAL_CLEAR_ITEMS_RETURN(b) typename std::enable_if<std::is_trivially_destructible<X>::value == b>::type
#define OXF_ARRAY_INTERNAL_INSERT_RETURN(b) typename std::enable_if<std::is_trivially_destructible<X>::value == b>::type
#define OXF_ARRAY_INTERNAL_EMPLACE_RETURN(b) typename std::enable_if<std::is_trivially_destructible<X>::value == b>::type

#define OXF_ARRAY_BUFFER_INIT(size) (T *) ::operator new(size * sizeof(T))

namespace oxf {
	template<class T, size_t N>
	class array {
	public:
		using value_type = T;
		using reference = T &;
		using const_reference = T const &;
		using pointer = T *;
		using const_pointer = T const *;
		using iterator = T *;
		using const_iterator = T const *;
		using riterator = std::reverse_iterator<iterator>;
		using const_riterator = std::reverse_iterator<const_iterator>;
		using difference_type = std::ptrdiff_t;
		using size_type = size_t;

	private:
		size_t bufferSize;
		T *buffer;
		bool valid[N] = {false};

	private:
		/**
		 * Destructs the items in the array if they are not trivial destructible
		 * @tparam X - Datatype of data stored in array
		 * @return Decides which methods get generated
		 */
		template<class X>
		OXF_ARRAY_INTERNAL_CLEAR_ITEMS_RETURN(false) internalClearItems() {
			for (size_t i = 0; i < N; ++i) {
				if (valid[i]) {
					buffer[i].~T();
					valid[i] = false;
				}
			}
		}

		template<class X>
		OXF_ARRAY_INTERNAL_CLEAR_ITEMS_RETURN(true) internalClearItems() {}

		/**
		 * Copies an array into the current array
		 * @param arr - Array to be copied
		 */
		void internalCopyAssign(const array<T, N> &arr) {
			this->internalClearItems<T>();
			for (size_t i = 0; i < N; ++i) {
				if (arr.valid[i]) {
					new(buffer + i) T(arr[i]);
					valid[i] = true;
				}
			}
		}

		/**
		 * Inserts an item at val <br>
		 * Destructs items if not trivial destructible
		 * @tparam X - Datatype of data stored in array
		 * @param index - Index of new item
		 * @param item - Item to be inserted
		 * @return Decides which methods get generated
		 */
		template<class X>
		OXF_ARRAY_INTERNAL_INSERT_RETURN(false) internalCopyInsert(size_t index, const T &item) {
			if (valid[index]) {
				buffer[index].~T();
			}
			new(buffer + index) T(item);
			valid[index] = true;
		}

		template<class X>
		OXF_ARRAY_INTERNAL_INSERT_RETURN(true) internalCopyInsert(size_t index, const T &item) {
			new(buffer + index) T(item);
			valid[index] = true;
		}

		/**
		 * Same as array&lt;T, N&gt;::internalCopyInsert(const T &, size_t) but moving instead of copying <br>
		 * Destructs items if not trivial destructible
		 * @tparam X - Datatype of data stored in array
		 * @param index - Index of new item
		 * @param item - Item to be inserted
		 * @return Decides which methods get generated
		 */
		template<class X>
		OXF_ARRAY_INTERNAL_INSERT_RETURN(false) internalMoveInsert(size_t index, T &&item) {
			std::cout << "1" << std::endl;
			std::cout << typeid(buffer[index]).name() << std::endl;
			std::cout << "2" << std::endl;

			if (valid[index]) {
				buffer[index].~T();
			}
			new(buffer + index) T(std::move(item));
			valid[index] = true;
		}

		template<class X>
		OXF_ARRAY_INTERNAL_INSERT_RETURN(true) internalMoveInsert(size_t index, T &&item) {
			new(buffer + index) T(std::move(item));
			valid[index] = true;
		}

		/**
		 * Constructs item at val in buffer <br>
		 * Destructs items if not trivial destructible
		 * @tparam X - Datatype of data stored in array
		 * @tparam Args - Unknown amount of arguments and their type
		 * @param index - Index of new item
		 * @param args - Arguments for the constructor
		 * @return Decides which methods get generated
		 */
		template<class X, class... Args>
		OXF_ARRAY_INTERNAL_EMPLACE_RETURN(false) internalEmplace(size_t index, Args &&... args) {
			if (valid[index]) {
				buffer[index].~T();
			}
			new(buffer + index) T(std::move(args)...);
			valid[index] = true;
		}

		template<class X, class... Args>
		OXF_ARRAY_INTERNAL_EMPLACE_RETURN(true) internalEmplace(size_t index, Args &&... args) {
			new(buffer + index) T(std::move(args)...);
			valid[index] = true;
		}

	public:
		/**
		 * Default constructor
		 */
		array() : bufferSize(N), buffer(OXF_ARRAY_BUFFER_INIT(N)) {}


		/**
		 * Copy constructs a array from a given array
		 * @param arr - Array to be copied
		 */
		array(const array<T, N> &arr) : bufferSize(N), buffer(OXF_ARRAY_BUFFER_INIT(N)) {
			try {
				size_t i = 0;
				for (auto item : arr) {
					this->internalCopyInsert<T>(i, item);
					++i;
				}
			} catch (...) {
				this->~array();

				throw;
			}
		}

		/**
		 * Move constructs array from given array
		 * @param arr - Array to be moved
		 */
		array(array<T, N> &&arr) noexcept: bufferSize(0), buffer(nullptr) {
			arr.swap(*this);
		}

		/**
		 * Constructs array from initializer list <br>
		 * @param list - List of items
		 */
		array(std::initializer_list<T> list) : bufferSize(list.size()), buffer(OXF_ARRAY_BUFFER_INIT(bufferSize)) {
			size_t i = 0;
			for (auto item : list) {
				this->internalCopyInsert<T>(i, item);
				++i;
			}
		}

		/**
		 * Destructor calls, if necessary, the destructor on all items of the array and then deletes the buffer
		 */
		~array() {
			if (buffer != nullptr) {
				this->internalClearItems<T>();
				::operator delete(buffer);
				buffer = nullptr;
			}
		}

		/**
		 * Copy assignment operator
		 * @param arr - Array to be copied
		 * @return Self
		 */
		array<T, N> &operator=(const array<T, N> &arr) {
			if (this != &arr) {
				this->internalCopyAssign(arr);
			}

			return *this;
		}

		/**
		 * Move assignment operator
		 * @param arr - Array to be moved
		 * @return Self
		 */
		array<T, N> &operator=(array<T, N> &&arr) noexcept {
			if (this != &arr) {
				arr.swap(*this);
			}

			return *this;
		}

		/**
		 * Initializer list assignment operator
		 * @param list - Initializer list
		 * @return Self
		 */
		array<T, N> &operator=(std::initializer_list<T> list) {
			if (list.size() > bufferSize) {
				throw std::out_of_range("Initializer list bigger than array!");
			}

			this->internalClearItems<T>();

			size_t i = 0;
			for (auto item : list) {
				this->internalCopyInsert<T>(i, item);
				++i;
			}

			return *this;
		}

		// ***************
		// * Item Access *
		// ***************
		/**
		 * Returns item reference at val i
		 * @warning Doesn't check bounds
		 * @param i - Index
		 * @return Item reference
		 */
		T &operator[](size_t i) {
			return buffer[i];
		}

		/**
		 * Returns item reference at val i
		 * @param i - Index
		 * @return Item reference
		 */
		T &at(size_t i) {
			OXF_ARRAY_ASSERT_INDEX(i)

			return buffer[i];
		}

		T *data() noexcept { return buffer; }

		T &front() { return buffer[0]; }

		T &front() const { return buffer[0]; }

		T &back() { return buffer[bufferSize - 1]; }

		T &back() const { return buffer[bufferSize - 1]; }


		// *************
		// * Iterators *
		// *************
		iterator begin() { return buffer; }

		riterator rbegin() { return riterator(end()); }

		const_iterator begin() const { return buffer; }

		const_riterator rbegin() const { return const_riterator(end()); }

		iterator end() { return buffer + bufferSize; }

		riterator rend() { return riterator(begin()); }

		const_iterator end() const { return buffer + bufferSize; }

		const_riterator rend() const { return const_riterator(begin()); }

		const_iterator cbegin() const { return begin(); }

		const_riterator crbegin() const { return rbegin(); }

		const_iterator cend() const { return end(); }

		const_riterator crend() const { return rend(); }


		// ************
		// * Capacity *
		// ************

		constexpr size_t size() const noexcept { return bufferSize; }


		// *************
		// * Modifiers *
		// *************
		/**
		 * Clears the array <br>
		 * Destructs all items if not trivial destructible
		 */
		void clear() {
			this->internalClearItems<T>();
		}

		/**
		 * Copy inserts item at val i
		 * @param item - Item to be inserted
		 * @param i - Index
		 */
		void insert(size_t i, const T &item) {
			OXF_ARRAY_ASSERT_INDEX(i)

			this->internalCopyInsert<T>(i, item);
		}

		/**
		 * Move inserts item at val i
		 * @param item - Item to be inserted
		 * @param i - Index
		 */
		void insert(size_t i, T &&item) {
			OXF_ARRAY_ASSERT_INDEX(i)

			this->internalMoveInsert<T>(i, std::move(item));
		}

		/**
		 * Swaps the content of two arrays
		 * @param arr - Array to swap with
		 */
		void swap(array<T, N> &arr) {
			std::swap(bufferSize, arr.bufferSize);
			std::swap(buffer, arr.buffer);
		}

		/**
		 * Constructs item in place at val i
		 * @tparam Args - Unknown amount of arguments and their type
		 * @param i - Index
		 * @param args - Constructor arguments
		 */
		template<class... Args>
		void emplace(size_t i, Args &&... args) {
			OXF_ARRAY_ASSERT_INDEX(i)

			this->internalEmplace<T>(i, std::move(args)...);
		}

		// ************************
		// * Non-Member Functions *
		// ************************
		/**
		 * Returns if two arrays items are the same
		 * @param arr - Array to compare to
		 * @return
		 */
		bool operator==(const array<T, N> &arr) {
			return memcmp(buffer, arr.buffer, bufferSize * sizeof(T)) == 0;
		}

		/**
		 * Returns if two arrays items are not the same
		 * @param arr - Array to compare to
		 * @return
		 */
		bool operator!=(const array<T, N> &arr) {
			return !(*this == arr);
		}
	};
}

#endif
