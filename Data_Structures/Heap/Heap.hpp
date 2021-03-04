#ifndef HEAP_LIBRARY_HPP
#define HEAP_LIBRARY_HPP

#include <Vector/Vector.hpp>

/**
 * Pre-made max heap function
 * @param T - Type of data stored in heap
 */
#define OXF_MAX_HEAP(T) [](const T &t1, const T &t2) { return t1 > t2; }

namespace oxf {
	template<class T>
	class heap {
	private:
		vector<T> data;

		/**
		 * Sorting condition for heap
		 */
		bool (*condition)(const T &, const T &) = [](const T &t1, const T &t2) {
			return t1 < t2;
		};

	private:
		/**
		 * Swaps two items in the heap if the sorting condition applies
		 * @param pos1 - Index of first item
		 * @param pos2 - Index of second item
		 * @return If items have been swapped
		 */
		bool internalSwapOnDemand(size_t pos1, size_t pos2) {
			if (condition(data[pos1], data[pos2])) {
				data.swapItems(pos1, pos2);
				return true;
			}
			return false;
		}

		/**
		 * Returns the child that fits the condition
		 * @param i1 - Index of child one
		 * @param i2 - Index of child two
		 * @return Child
		 */
		size_t internalGetCorrectChild(size_t i1, size_t i2) {
			return condition(data[i1], data[i2]) ? i1 : i2;
		}

		/**
		 * Sorts the latest added item in the heap
		 */
		void internalPush() {
			size_t pos = data.size() - 1;

			/*
			 * While swaps happen and is not base item continue loop.
			 * Subtracting one if even number to adjust for 0 based indexing.
			 */
			while (pos != 0 && this->internalSwapOnDemand(pos, (pos >> 1) - (pos % 2 == 0))) {
				pos >>= 1;
				if (pos % 2 == 0) {
					--pos;
				}
			}
		}

		/**
		 * Rearranges the items after the base has been popped
		 */
		void internalRemoveProcedure() {
			size_t pos = 1;
			size_t child;

			/**
			 * pos: left child of parent
			 * child: val of child that fits the condition (left or right)
			 *
			 * While swaps happen and there are two children left continue loop.
			 * Get best suited children and swap with parent (pos >> 1) if the swap condition applies, then adjust
			 * pos to the child of child (consider val for left and right child and adjust accordingly)
			 */
			while (data.size() > pos + 1 &&
			       this->internalSwapOnDemand((child = this->internalGetCorrectChild(pos, pos + 1)), pos >> 1)) {
				pos = (pos << 1) + (pos + 1 == child);
			}
			if (data.size() > pos) { this->internalSwapOnDemand(pos, pos >> 1); }
		}

	public:
		heap() = default;

		/**
		 * Constructor; pre-allocates memory for size items in the buffer
		 * @param size - Amount of memory to allocate
		 */
		explicit heap(size_t size) : data(size) {}

		/**
		 * Constructor; pre-allocates memory for size items in buffer with custom sorting condition
		 * @tparam FuncSig - Implicit return of function
		 * @param size - Amount of memory to allocate
		 * @param f - Custom sorting condition
		 */
		template<class FuncSig>
		explicit heap(size_t size, FuncSig f) : data(size), condition(f) {}

		/**
		 * Constructor; assigns custom sorting condition
		 * @tparam FuncSig - Implicit return of function
		 * @param f - Custom sorting condition
		 */
		template<class FuncSig>
		explicit heap(FuncSig f) : data(), condition(f) {}

		/**
		 * Constructor; initializes the heap with the list of items \b and sorts them
		 * @param list - List of items
		 */
		heap(std::initializer_list<T> list) : data(list.size()) {
			for (auto i : list) {
				this->push(i);
			}
		}

		/**
		 * Constructor; initializes the heap with list of items \b and sorts them with a custom sorting condition
		 * @tparam FuncSig - Implicit return of function
		 * @param list - List of items
		 * @param f - Custom sorting condition
		 */
		template<class FuncSig>
		heap(std::initializer_list<T> list, FuncSig f) : data(list.size()), condition(f) {
			for (auto i : list) {
				this->push(i);
			}
		}

		/**
		 * Destructs buffer
		 */
		~heap() {
			data.~vector();
			condition = nullptr;
		}

		/**
		 * Clears the buffer, fills it with the list of items \b and sorts them
		 * @param list - List of items
		 * @return Self
		 */
		heap<T> &operator=(std::initializer_list<T> list) {
			data.clear();

			for (auto i : list) {
				this->push(i);
			}

			return *this;
		}

		// ***************
		// * Item Access *
		// ***************
		/**
		 * Returns reference to base item
		 * @return Item
		 */
		T &peak() { return data[0]; }

		/**
		 * Returns const reference to base item
		 * @return Item
		 */
		T &peak() const { return data[0]; }

		// ************
		// * Capacity *
		// ************
		bool empty() noexcept { return data.empty(); }

		size_t size() noexcept { return data.size(); }

		size_t capacity() const noexcept { return data.capacity(); }

		size_t maxCapacity() const noexcept { return data.maxCapacity(); }

		// *************
		// * Modifiers *
		// *************
		/**
		 * Clears the heaps buffer
		 */
		void clear() {
			data.clear();
		}

		/**
		 * Copies an item onto the heap and sorts it
		 * @param item - Item to be copied
		 */
		void push(const T &item) {
			data.push_back(item);

			this->internalPush();
		}

		/**
		 * Moves an item onto the heap and sorts it
		 * @param item - Item to be moved
		 */
		void push(T &&item) {
			data.push_back(std::move(item));

			this->internalPush();
		}

		/**
		 * Removes the base item and rearranges the remaining heap
		 */
		void pop() {
			data.swapItems(0, data.size() - 1);
			data.pop_back();

			this->internalRemoveProcedure();
		}
	};
}

#endif
