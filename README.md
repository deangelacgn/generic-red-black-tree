# Generic Red-Black Tree

This RBT(short for Red-Black Tree) class implements the following public methods:

* find_min()
* find_max()
* contains()
* empty()
* size()
* retrieve()
* validate()
* count_blacks()
* clear()
* insert()
* remove()
* preorder()
* postorder()
* inorder()

Moreover, this RBT class has a Default constructor and a Copy constructor.

The structure of of the tree was inspired in the one proposed by Cormen et al in the book Introduction to Algorithms, which uses a sentinel node to represent all leafs of the tree, as shown in the figure below. 

![alt text](https://image.slidesharecdn.com/10redblacktrees-150322214538-conversion-gate01/95/10-redblack-trees-23-638.jpg?cb=1456759557)
Rotation, Insertion and Removal methods were also implemented according to the pseudocodes provided in the book. You can find more details about the implementation and description of all methods and constructors here.

### Prerequisites

In order to use this RBT class your compiler needs to support C++11 Standard. If you use a GCC compiler, at least GCC 4.8.1 is needed.

### How to use

Clone this repo to your local machine and move your .cpp files to the src folder inside the project.

## Running the tests

Inside the src folder you will find a drive_rbt.cpp that has a few unit tests. Compile and run this file to check if everything is okay.

```
g++ -std=c++11 drive_rbt.cpp -g && ./a.out
```

If all tests pass, you can start playing around with the RBT by including rbt.h in your own .cpp files.

## Authors

* Deangela C. G. Neves
* Maria Clara Martins 

