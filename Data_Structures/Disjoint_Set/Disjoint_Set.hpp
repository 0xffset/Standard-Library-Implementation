#ifndef DISJOINT_SET_HPP
#define DISJOINT_SET_HPP

#include <initializer_list>
#include <cstring>
#include <iostream>

namespace oxf {
    template<class T>
    class disjoint_set {
    public:
        struct item {
            T value;
            size_t parent;
            size_t children;
            bool isBase;
        };

    private:
        size_t bufferSize;
        item *buffer;

    public:
        disjoint_set(std::initializer_list<T> list) : bufferSize(0),
                                                      buffer(new item[list.size()]) {
            for (auto elem : list) {
                buffer[bufferSize] = item{elem, bufferSize, 0, true};
                ++bufferSize;
            }
        }

        disjoint_set<T>& operator=(std::initializer_list<T> list) {
            free(*buffer);
        }

        item *find(const T &val) {
            item *temp = nullptr;

            for (size_t i = 0; i < bufferSize; ++i) {
                if (memcmp(&(buffer[i].value), &val, sizeof(T)) == 0) {
                    temp = &(buffer[i]);
                    break;
                }
            }

            // TODO: add collapsing find
            if (temp != nullptr) {
                while (!(temp->isBase)) {
                    temp = &(buffer[temp->parent]);
                }
            }

            return temp;
        }

        void to_union(const T &val1, const T &val2) {
            item *item1 = this->find(val1);
            item *item2 = this->find(val2);

            if (item1->parent != item2->parent) {
                size_t diff = item1->children - item2->children; // weighted union

                if (diff < 0) { // if item 2 has more children; item 2 parent
                    item2->children += item1->children + 1;

                    item1->parent = item2->parent;
                    item1->isBase = false;
                } else { // else (if item 1 has more children or equal amount of children); item 1 parent
                    item1->children += item2->children + 1;

                    item2->parent = item1->parent;
                    item2->isBase = false;
                }
            }
        }

        item *data() const noexcept { return buffer; }

        size_t size() const noexcept { return bufferSize; }
    };
}

#endif
