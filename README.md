# Standard Library Implementations
This repository contains a custom standard library implementation which I made to learn and practice or/and out of curiosity and fun. <br>
Documentation for the functions/methods is provided via doxygen descriptions from inside the IDE/text editor.
#### Project namespace: oxf

## Vector
##### TODO: Add construction/assignment with iterable type
Header only implementation of a vector.
```cpp
oxf::vector<T> vec;
```

## Array
##### TODO: Add construction/assignment with iterable type
##### TODO: Fix insert bug causing memory access violations
Header only implementation of an array.
```cpp
oxf::array<T, N> arr;
```

## Disjoint Set
### Not finished
##### TODO: Add collapsing find
##### TODO: Add documentation
Header only implementation of a very basic disjoint set.
```cpp
oxf::disjoint_set<T> set;
```

## Heap
##### TODO: Fix potential bugs
##### TODO: Add features if necessary
##### TODO: Add heap moving and copying (figure out how to copy functions)
Header only implementation of a dynamic sized heap (sometimes priority queue).
```cpp
oxf::heap<T> heap;
```

## Thanks, Credits and Sources
Thanks to [Martin York](https://github.com/Loki-Astari) for the really good [series on vectors](https://lokiastari.com/blog/2016/02/27/vector/val.html). <br>
Thanks to [Abdul Bari](https://www.youtube.com/channel/UCZCFT11CWBi3MHNlGf019nw) for his great videos about [disjoint sets](https://www.youtube.com/watch?v=wU6udHRIkcc) and [heaps](https://www.youtube.com/watch?v=HqPJF2L5h9U).
