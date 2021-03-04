#ifndef OXF_VECTOR_HPP
#define OXF_VECTOR_HPP

#include <iterator>
#include <utility>
#include <stdexcept>

#define OXF_VECTOR_ASSERT_INDEX(i)  if (i >= itemCounter) { throw std::runtime_error("index out of range"); }

#define OXF_VECTOR_INTERNAL_SIMPLE_COPY_RETURN(b) typename std::enable_if<std::is_nothrow_move_constructible<X>::value == b>::type
#define OXF_VECTOR_INTERNAL_MOVE_COPY_RETURN(b) typename std::enable_if<std::is_nothrow_move_constructible<X>::value == b>::type
#define OXF_VECTOR_INTERNAL_SWAP_ITEMS_RETURN(b) typename std::enable_if<std::is_nothrow_move_constructible<X>::value == b>::type

#define OXF_VECTOR_INTERNAL_COPY_ASSIGN_RETURN(b) typename std::enable_if<std::is_nothrow_destructible<X>::value == b>::type

#define OXF_VECTOR_INTERNAL_CLEAR_ITEMS_RETURN(b) typename std::enable_if<std::is_trivially_destructible<X>::value == b>::type
#define OXF_VECTOR_INTERNAL_SHIFT_LEFT_RETURN(b) typename std::enable_if<std::is_trivially_destructible<X>::value == b>::type
#define OXF_VECTOR_INTERNAL_SHIFT_RIGHT_RETURN(b) typename std::enable_if<std::is_trivially_destructible<X>::value == b>::type
#define OXF_VECTOR_INTERNAL_INSERT_RETURN(b) typename std::enable_if<std::is_trivially_destructible<X>::value == b>::type
#define OXF_VECTOR_INTERNAL_POP_BACK_RETURN(b) typename std::enable_if<std::is_trivially_destructible<X>::value == b>::type
#define OXF_VECTOR_INTERNAL_EMPLACE_RETURN(b) typename std::enable_if<std::is_trivially_destructible<X>::value == b>::type
#define OXF_VECTOR_INTERNAL_COPY_REPLACE_RETURN(b) typename std::enable_if<std::is_trivially_destructible<X>::value == b>::type
#define OXF_VECTOR_INTERNAL_MOVE_REPLACE_RETURN(b) typename std::enable_if<std::is_trivially_destructible<X>::value == b>::type
#define OXF_VECTOR_INTERNAL_EMPLACE_REPLACE_RETURN(b) typename std::enable_if<std::is_trivially_destructible<X>::value == b>::type

#define OXF_VECTOR_BUFFER_INIT(size) (T *) ::operator new(size * sizeof(T))
#define OXF_VECTOR_SIZE(x) (2>x?2:x)

namespace oxf {
	template<class T>
	class vector {
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
		size_t itemCounter;
		T *buffer;

	private:
		/**
		 * Changes the size of the buffer
		 * @warning Size is not validated, this can lead to memory leaks if Vector&lt;T&gt;::internalResize(size_t)
		 *          is called with newSize < bufferSize
		 * @param newSize
		 */
		void internalResize(size_t newSize) {
			vector<T> temp(newSize);
			this->internalSimpleCopy<T>(temp);
			temp.swap(*this);
		}

		/**
		 * Changes the size of the buffer on demand to make room for more items
		 */
		void resizeOnDemand() {
			if (itemCounter >= bufferSize) {
				this->internalResize(bufferSize * 1.6);
			}
		}

		/**
		 * Copies item to end of buffer and increments the item counter
		 * @param item - Item to be pushed
		 */
		void internalPushBack(const T &item) {
			new(buffer + itemCounter) T(item);
			++itemCounter;
		}

		/**
		 * Moves item to end of buffer and increments the item counter
		 * @param item - Item to be moved
		 */
		void internalMoveBack(T &&item) {
			new(buffer + itemCounter) T(std::move(item));
			++itemCounter;
		}

		/**
		 * Constructs item at the end of the buffer and increments the item counter
		 * @tparam Args - Unknown amount of arguments and their types
		 * @param args - Argument(s) for the constructor
		 */
		template<class... Args>
		void internalEmplaceBack(Args &&... args) {
			new(buffer + itemCounter) T(std::move(args)...);
			++itemCounter;
		}

		/**
		 * Moves or copies an item from ori to dst <br>
		 * Move or copy are dependent on if the datatype stored in the Vector is nothrow move constructable or not
		 * @tparam X  - Datatype of data stored in Vector
		 * @param ori - Origin of item to be copied or moved
		 * @param dst - Destination of move or copy
		 * @return Decides which method gets generated
		 */
		template<class X>
		OXF_VECTOR_INTERNAL_MOVE_COPY_RETURN(false) internalMoveOrCopy(size_t ori, size_t dst) {
			new(buffer + dst) T(buffer[ori]);
		}

		template<class X>
		OXF_VECTOR_INTERNAL_MOVE_COPY_RETURN(true) internalMoveOrCopy(size_t ori, size_t dst) {
			new(buffer + dst) T(std::move(buffer[ori]));
		}

		/**
		 * Copy or move copy the current Vector into the destination Vector <br>
		 * Move or copy are dependent on if the datatype stored in the Vector is nothrow move constructable or not
		 * @tparam X - Datatype of data stored in Vector
		 * @param dst - Destination Vector
		 * @return Decides which methods gets generated
		 */
		template<class X>
		OXF_VECTOR_INTERNAL_SIMPLE_COPY_RETURN(false) internalSimpleCopy(vector<T> &dst) {
			for (size_t i = 0; i < itemCounter; ++i) {
				dst.internalPushBack(buffer[i]);
			}
		}

		template<class X>
		OXF_VECTOR_INTERNAL_SIMPLE_COPY_RETURN(true) internalSimpleCopy(vector<T> &dst) {
			for (size_t i = 0; i < itemCounter; ++i) {
				dst.internalMoveBack(std::move(buffer[i]));
			}
		}

		/**
		 * Destructs the items in the Vector if they are not trivial destructible
		 * @tparam X - Datatype of data stored in Vector
		 * @return Decides which methods get generated
		 */
		template<class X>
		OXF_VECTOR_INTERNAL_CLEAR_ITEMS_RETURN(false) internalClearItems() {
			for (size_t i = 0; i < itemCounter; ++i) {
				buffer[i].~T();
			}
			itemCounter = 0;
		}

		template<class X>
		OXF_VECTOR_INTERNAL_CLEAR_ITEMS_RETURN(true) internalClearItems() {
			itemCounter = 0;
		}

		/**
		 * Copies a Vector into the current Vector; tries to avoid new buffer allocation if enough space has already
		 * been allocated to copy the Vector \b and if the items are no throw destructible, else it allocates a new buffer <br>
		 * Destructs items of current Vector if not trivial destructible
		 * @tparam X - Datatype of data stored in Vector
		 * @param vec - Vector to be copied
		 * @return Decides which methods get generated
		 */
		template<class X>
		OXF_VECTOR_INTERNAL_COPY_ASSIGN_RETURN(false) internalCopyAssign(const vector<X> &vec) {
			vector<T> temp(vec);
			temp.swap(*this);
		}

		template<class X>
		OXF_VECTOR_INTERNAL_COPY_ASSIGN_RETURN(true) internalCopyAssign(const vector<X> &vec) {
			// if the buffer has enough memory allocated simply clear it and copy the elements
			// no new memory allocation
			if (bufferSize <= vec.itemCounter) {
				this->internalClearItems<T>();

				for (size_t i = 0; i < vec.itemCounter; ++i) {
					this->internalPushBack(vec.buffer[i]);
				}
			} else {
				// if not enough memory has been allocated we need to allocate more
				// aka. create new Vector of the Vector we want to copy and swap the current one with it
				vector<T> temp(vec);
				temp.swap(*this);
			}
		}

		/**
		 * Erases the item at index and shifts all trailing items one to the left <br>
		 * Destructs item if not trivial destructible
		 * @tparam X - Datatype of data stored in Vector
		 * @param index - Item to be erased
		 * @return Decides which methods get generated
		 */
		template<class X>
		OXF_VECTOR_INTERNAL_SHIFT_LEFT_RETURN(false) internalShiftLeft(size_t index) {
			--itemCounter;
			for (size_t i = index; i < itemCounter; ++i) {
				buffer[i].~T();
				this->internalMoveOrCopy<T>(i + 1, i);
			}
			buffer[itemCounter].~T();
		}

		template<class X>
		OXF_VECTOR_INTERNAL_SHIFT_LEFT_RETURN(true) internalShiftLeft(size_t index) {
			--itemCounter;
			for (size_t i = index; i < itemCounter; ++i) {
				this->internalMoveOrCopy<T>(i + 1, i);
			}
		}

		/**
		 * Shifts items in Vector one to the right
		 * using Vector&lt;T&gt;::internalMoveOrCopy&lt;T&gt;(size_t, size_t) <br>
		 * Destructs items if not trivial destructible
		 * @tparam X - Datatype of data stored in Vector
		 * @param index - Starting point for shift
		 * @return Decides which methods get generated
		 */
		template<class X>
		OXF_VECTOR_INTERNAL_SHIFT_RIGHT_RETURN(false) internalShiftRight(size_t index) {
			this->reserve(itemCounter + 1);

			for (size_t i = itemCounter; i > index; --i) {
				this->internalMoveOrCopy<T>(i - 1, i);
				buffer[i - 1].~T();
			}
		}

		template<class X>
		OXF_VECTOR_INTERNAL_SHIFT_RIGHT_RETURN(true) internalShiftRight(size_t index) {
			this->reserve(itemCounter + 1);

			for (size_t i = itemCounter; i > index; --i) {
				this->internalMoveOrCopy<T>(i - 1, i);
			}
		}

		/**
		 * Inserts an item at index, copy shifting already existing items to the right
		 * using Vector&lt;T&gt;::internalShiftRight&lt;T&gt;(size_t) <br>
		 * Destructs items if not trivial destructible
		 * @tparam X - Datatype of data stored in Vector
		 * @param index - Index of new item
		 * @param item - Item to be inserted
		 * @return Decides which methods get generated
		 */
		template<class X>
		OXF_VECTOR_INTERNAL_INSERT_RETURN(false) internalCopyInsert(size_t index, const T &item) {
			this->internalShiftRight<T>(index);

			buffer[index].~T();
			new(buffer + index) T(item);
			++itemCounter;
		}

		template<class X>
		OXF_VECTOR_INTERNAL_INSERT_RETURN(true) internalCopyInsert(size_t index, const T &item) {
			this->internalShiftRight<T>(index);

			new(buffer + index) T(item);
			++itemCounter;
		}

		/**
		 * Same as Vector&lt;T&gt;::internalCopyInsert(const T &, size_t) but moving instead of copying
		 * @tparam X - Datatype of data stored in Vector
		 * @param index - Index of new item
		 * @param item - Item to be inserted
		 * @return Decides which methods get generated
		 */
		template<class X>
		OXF_VECTOR_INTERNAL_INSERT_RETURN(false) internalMoveInsert(size_t index, T &&item) {
			this->internalShiftRight<T>(index);

			buffer[index].~T();
			new(buffer + index) T(std::move(item));
			++itemCounter;
		}

		template<class X>
		OXF_VECTOR_INTERNAL_INSERT_RETURN(true) internalMoveInsert(size_t index, T &&item) {
			this->internalShiftRight<T>(index);

			new(buffer + index) T(std::move(item));
			++itemCounter;
		}

		/**
		 * Constructs item at index in buffer, shifting already existing items one to the right
		 * using Vector&lt;T&gt;::internalShiftRight&lt;T&gt;(size_t) <br>
		 * Destructs items if not trivial destructible
		 * @tparam X - Datatype of data stored in Vector
		 * @tparam Args - Unknown amount of arguments and their type
		 * @param index - Index of new item
		 * @param args - Arguments for the constructor
		 * @return Decides which methods get generated
		 */
		template<class X, class... Args>
		OXF_VECTOR_INTERNAL_EMPLACE_RETURN(false) internalEmplace(size_t index, Args &&... args) {
			this->internalShiftRight<T>(index);

			buffer[index].~T();
			new(buffer + index) T(std::move(args)...);
			++itemCounter;
		}

		template<class X, class... Args>
		OXF_VECTOR_INTERNAL_EMPLACE_RETURN(true) internalEmplace(size_t index, Args &&... args) {
			this->internalShiftRight<T>(index);

			new(buffer + index) T(std::move(args)...);
			++itemCounter;
		}

		/**
		 * Removes the last item from the Vector <br>
		 * Destructs the item if not trivial destructible
		 * @tparam X - Datatype of data stored in Vector
		 * @return Decides which methods get generated
		 */
		template<class X>
		OXF_VECTOR_INTERNAL_POP_BACK_RETURN(false) internalPopBack() {
			--itemCounter;
			buffer[itemCounter].~T();
		}

		template<class X>
		OXF_VECTOR_INTERNAL_POP_BACK_RETURN(true) internalPopBack() {
			--itemCounter;
		}

		/**
		 * Copies item and replaces index i with it <br>
		 * Destructs the item if not trivial destructible
		 * @tparam X - Datatype of data stored in Vector
		 * @param i - Index to replace
		 * @param item - Item to replace with
		 * @return Decides which methods get generated
		 */
		template<class X>
		OXF_VECTOR_INTERNAL_COPY_REPLACE_RETURN(false) internalCopyReplace(size_t i, const T &item) {
			buffer[i].~T();
			new(buffer + i) T(item);
		}

		template<class X>
		OXF_VECTOR_INTERNAL_COPY_REPLACE_RETURN(true) internalCopyReplace(size_t i, const T &item) {
			new(buffer + i) T(item);
		}

		/**
		 * Same as Vector&lt;T&gt;::internalCopyReplace&lt;T&gt;(size_t, const T&) but with moving
		 */
		template<class X>
		OXF_VECTOR_INTERNAL_MOVE_REPLACE_RETURN(false) internalMoveReplace(size_t i, T &&item) {
			buffer[i].~T();
			new(buffer + i) T(std::move(item));
		}

		template<class X>
		OXF_VECTOR_INTERNAL_MOVE_REPLACE_RETURN(true) internalMoveReplace(size_t i, T &&item) {
			new(buffer + i) T(std::move(item));
		}

		/**
		 * Same as Vector&lt;T&gt;::internalCopyReplace&lt;T&gt;(size_t, const T&) but with emplacing
		 */
		template<class X, class... Args>
		OXF_VECTOR_INTERNAL_EMPLACE_REPLACE_RETURN(false) internalEmplaceReplace(size_t i, Args &&... args) {
			buffer[i].~T();
			new(buffer + i) T(std::move(args)...);
		}

		template<class X, class... Args>
		OXF_VECTOR_INTERNAL_EMPLACE_REPLACE_RETURN(true) internalEmplaceReplace(size_t i, Args &&... args) {
			new(buffer + i) T(std::move(args)...);
		}

		/**
		 * Swaps two items positions in the Vector <br>
		 * Destructs the items if not trivial destructible
		 * @tparam X - Datatype of data stored in Vector
		 * @param a - Index of first item
		 * @param b - Index of second item
		 * @return Decides which methods get generated
		 */
		template<class X>
		OXF_VECTOR_INTERNAL_SWAP_ITEMS_RETURN(false) internalSwapItemsInit(size_t a, size_t b) {
			T temp = T(buffer[a]);

			this->internalCopyReplace<T>(a, buffer[b]);
			this->internalCopyReplace<T>(b, temp);
		}

		template<class X>
		OXF_VECTOR_INTERNAL_SWAP_ITEMS_RETURN(true) internalSwapItemsInit(size_t a, size_t b) {
			T temp = T(std::move(buffer[a]));

			this->internalCopyReplace<T>(a, std::move(buffer[b]));
			this->internalCopyReplace<T>(b, std::move(temp));
		}

	public:
		/**
		 * Default constructor <br>
		 * The Vector \b always allocates and has space for \b at \b least two items out of ease of implementing
		 * and logical use for a list type
		 */
		vector() : bufferSize(2), itemCounter(0), buffer(OXF_VECTOR_BUFFER_INIT(2)) {}

		/**
		 * Constructs Vector of size MAX(2, size)
		 * @param size - Size of Vector
		 */
		explicit vector(size_t size) : bufferSize(OXF_VECTOR_SIZE(size)), itemCounter(0),
		                               buffer(OXF_VECTOR_BUFFER_INIT(bufferSize)) {}

		/**
		 * Copy constructs a Vector from a given Vector
		 * @param vec - Vector to be copied
		 */
		vector(const vector<T> &vec) : bufferSize(vec.bufferSize), itemCounter(0),
		                               buffer(OXF_VECTOR_BUFFER_INIT(bufferSize)) {
			try {
				for (size_t i = 0; i < vec.itemCounter; ++i) {
					this->push_back(vec.buffer[i]);
				}
			} catch (...) {
				this->~vector();

				throw;
			}
		}

		/**
		 * Move constructs Vector from given Vector
		 * @param vec - Vector to be moved
		 */
		vector(vector<T> &&vec) noexcept: bufferSize(0), itemCounter(0), buffer(nullptr) {
			vec.swap(*this);
		}

		/**
		 * Constructs Vector from initializer list <br>
		 * Allocates at least space for two items in all cases
		 * @param list - List of items
		 */
		vector(std::initializer_list<T> list) : bufferSize(OXF_VECTOR_SIZE(list.size())), itemCounter(0),
		                                        buffer(OXF_VECTOR_BUFFER_INIT(bufferSize)) {
			for (auto item : list) {
				this->internalPushBack(item);
			}
		}

		/**
		 * Destructor calls, if necessary, the destructor on all items of the Vector and then deletes the buffer
		 */
		~vector() {
			if (buffer != nullptr) {
				this->internalClearItems<T>();
				::operator delete(buffer);
				buffer = nullptr;
			}
		}

		/**
		 * Copy assignment operator
		 * @param vec - Vector to be copied
		 * @return Self
		 */
		vector<T> &operator=(const vector<T> &vec) {
			if (this != &vec) {
				this->internalCopyAssign(vec);
			}

			return *this;
		}

		/**
		 * Move assignment operator
		 * @param vec - Vector to be moved
		 * @return Self
		 */
		vector<T> &operator=(vector<T> &&vec) noexcept {
			if (this != &vec) {
				vec.swap(*this);
			}

			return *this;
		}

		/**
		 * Initializer list assignment operator
		 * @param list - Initializer list
		 * @return Self
		 */
		vector<T> &operator=(std::initializer_list<T> list) {
			this->internalClearItems<T>();

			for (auto item : list) {
				this->push_back(item);
			}

			return *this;
		}

		// ***************
		// * Item Access *
		// ***************
		/**
		 * Returns item reference at index i
		 * @warning Doesn't check bounds
		 * @param i - Index
		 * @return Item reference
		 */
		T &operator[](size_t i) {
			return buffer[i];
		}

		/**
		 * Returns const item reference at index i
		 * @warning Doesn't check bounds
		 * @param i - Index
		 * @return Item reference
		 */
		T &operator[](size_t i) const {
			return buffer[i];
		}

		/**
		 * Returns item reference at index i
		 * @param i - Index
		 * @return Item reference
		 */
		T &at(size_t i) {
			OXF_VECTOR_ASSERT_INDEX(i)

			return buffer[i];
		}

		/**
		 * Returns const item reference at index i
		 * @param i - Index
		 * @return Item reference
		 */
		T &at(size_t i) const {
			OXF_VECTOR_ASSERT_INDEX(i)

			return buffer[i];
		}

		T *data() noexcept { return buffer; }

		T &front() { return buffer[0]; }

		T &front() const { return buffer[0]; }

		T &back() { return buffer[itemCounter - 1]; }

		T &back() const { return buffer[itemCounter - 1]; }

		// *************
		// * Iterators *
		// *************
		iterator begin() { return buffer; }

		riterator rbegin() { return riterator(end()); }

		const_iterator begin() const { return buffer; }

		const_riterator rbegin() const { return const_riterator(end()); }

		iterator end() { return buffer + itemCounter; }

		riterator rend() { return riterator(begin()); }

		const_iterator end() const { return buffer + itemCounter; }

		const_riterator rend() const { return const_riterator(begin()); }

		const_iterator cbegin() const { return begin(); }

		const_riterator crbegin() const { return rbegin(); }

		const_iterator cend() const { return end(); }

		const_riterator crend() const { return rend(); }

		// ************
		// * Capacity *
		// ************
		bool empty() noexcept { return itemCounter == 0; }

		size_t size() const noexcept { return itemCounter; }

		size_t capacity() const noexcept { return bufferSize; }

		size_t maxCapacity() const noexcept { return SIZE_MAX; }

		/**
		 * Allocates more memory for the buffer <br>
		 * If newSize is smaller than the current size nothing happens
		 * @param newSize - Size of buffer
		 */
		void reserve(size_t newSize) {
			if (newSize > bufferSize) {
				this->internalResize(newSize);
			}
		}

		/**
		 * Frees all memory the buffer allocated which is not currently occupied by items
		 */
		void shrink_to_fit() {
			vector<T> temp(OXF_VECTOR_SIZE(itemCounter));
			this->internalSimpleCopy<T>(temp);
			temp.swap(*this);
		}

		// *************
		// * Modifiers *
		// *************
		/**
		 * Clears the Vector <br>
		 * Destructs all items if not trivial destructible
		 */
		void clear() {
			this->internalClearItems<T>();
		}

		/**
		 * Erases item at index i <br>
		 * Destructs item if not trivial destructible
		 * @param i - Index
		 */
		void erase(size_t i) {
			OXF_VECTOR_ASSERT_INDEX(i)

			this->internalShiftLeft<T>(i);
		}

		/**
		 * Copy inserts item at index i
		 * @param i - Index
		 * @param item - Item to be inserted
		 */
		void insert(size_t i, const T &item) {
			OXF_VECTOR_ASSERT_INDEX(i)

			this->internalCopyInsert<T>(i, item);
		}

		/**
		 * Move inserts item at index i
		 * @param i - Index
		 * @param item - Item to be inserted
		 */
		void insert(size_t i, T &&item) {
			OXF_VECTOR_ASSERT_INDEX(i)

			this->internalMoveInsert<T>(i, std::move(item));
		}

		/**
		 * Copy pushes item at the end of the Vector
		 * @param item - Item to be pushed
		 */
		void push_back(const T &item) {
			this->resizeOnDemand();

			this->internalPushBack(item);
		}

		/**
		 * Move pushes item at the end of the Vector
		 * @param item - Item to be pushed
		 */
		void push_back(T &&item) {
			this->resizeOnDemand();

			this->internalMoveBack(std::move(item));
		}

		/**
		 * Removes the last item of the Vector
		 */
		void pop_back() {
			this->internalPopBack<T>();
		}

		/**
		 * Swaps the content of two vectors
		 * @param vec - Vector to swap with
		 */
		void swap(vector<T> &vec) {
			std::swap(bufferSize, vec.bufferSize);
			std::swap(itemCounter, vec.itemCounter);
			std::swap(buffer, vec.buffer);
		}

		/**
		 * Swaps two items in the Vector
		 * @param a - Index of item A
		 * @param b - Index of item B
		 */
		void swapItems(size_t a, size_t b) {
			OXF_VECTOR_ASSERT_INDEX(a)
			OXF_VECTOR_ASSERT_INDEX(b)

			this->internalSwapItemsInit<T>(a, b);
		}

		/**
		 * Resizes the Vector
		 * @param size - New item count of Vector
		 */
		void resize(size_t size) {
			for (size_t i = itemCounter; i > size; --i) {
				this->internalPopBack<T>();
			}
		}

		/**
		 * Constructs item in place at end of Vector
		 * @tparam Args - Unknown amount of arguments and their type
		 * @param args - Constructor arguments
		 */
		template<class... Args>
		void emplace_back(Args &&... args) {
			this->resizeOnDemand();

			this->internalEmplaceBack(std::move(args)...);
		}

		/**
		 * Constructs item in place at index i, shifting already existing item and all trailing items to the right
		 * @tparam Args - Unknown amount of arguments and their type
		 * @param i - Index
		 * @param args - Constructor arguments
		 */
		template<class... Args>
		void emplace(size_t i, Args &&... args) {
			OXF_VECTOR_ASSERT_INDEX(i)

			this->internalEmplace<T>(i, std::move(args)...);
		}

		/**
		 * Copies item and replaces index i with it
		 * @param i - Index to replace
		 * @param item - Item to replace with
		 */
		void replace(size_t i, const T &item) {
			OXF_VECTOR_ASSERT_INDEX(i)

			this->internalCopyReplace<T>(i, item);
		}

		/**
		 * Moves item and replaces index i with it
		 * @param i - Index to replace
		 * @param item - Item to replace with
		 */
		void replace(size_t i, T &&item) {
			OXF_VECTOR_ASSERT_INDEX(i)

			this->internalMoveReplace<T>(i, std::move(item));
		}

		/**
		 * Constructs item and replaces index i with it
		 * @tparam Args - Unknown amount of arguments and their type
		 * @param i - Index to replace
		 * @param args - Constructor arguments
		 */
		template<class... Args>
		void replace(size_t i, Args &&... args) {
			OXF_VECTOR_ASSERT_INDEX(i)

			this->internalEmplaceReplace<T>(i, std::move(args)...);
		}

		// ************************
		// * Non-Member Functions *
		// ************************
		/**
		 * Returns if two vectors items are the same
		 * @param vec - Vector to compare to
		 * @return
		 */
		bool operator==(const vector<T> &vec) {
			return (itemCounter == vec.itemCounter) && (memcmp(buffer, vec.buffer, itemCounter * sizeof(T)) == 0);
		}

		/**
		 * Returns if two vectors items are not the same
		 * @param vec - Vector to compare to
		 * @return
		 */
		bool operator!=(const vector<T> &vec) {
			return !(*this == vec);
		}
	};
}

#endif