#include <iostream>   // cout, cin, endl
#include <utility>    // std::pair<>
#include <random>     // radom_device, mt19937, std::shuffle.
#include <iomanip>    // std::setw()
#include <string>     // std::string
#include <cassert>
#include <vector>
#include <algorithm>

#include "../include/rbt.h"

template<class T>
void print(const T &arg)
{
    std::cout << arg << std::endl;
}

int main()
{
    auto n_unit{0}; // unit test count.

    // Our BST will store strings with integer keys.
    typedef std::pair< size_t, size_t > test_type;

    test_type data[] = {
    { 10, 10 },
    { 85, 85 },
    { 15, 15 },
    { 70, 70 },
    { 20, 20 },
    { 60, 60 },
    { 30, 30 },
    { 50, 50 },
    { 65, 65 },
    { 80, 80 },
    { 90, 90 },
    { 40, 40 },
    {  5,  5 },
    { 55, 55 }
    };

    

    {
        // The tree declaration.
//#define USE_FUNCTOR
#ifdef USE_FUNCTOR
        struct CompareKey {
            bool operator()( const size_t & a, const size_t & b )
            {
                return a > b ;
            }
        };
        RBT< size_t, std::string, CompareKey > tree;
#else // Use lambda
        auto compare_keys = []( const size_t &a, const size_t &b )->bool
        {
            return a > b ;
        };
        //BST< size_t, std::string, decltype( compare_keys ) > tree( compare_keys );
#endif
        
    {
    


    {
        std::cout << ">>> Unit teste #" << ++n_unit << ": deletion.\n";

        RBT< size_t, size_t > tree( compare_keys );

        for( const auto & e : data )
        {
            tree.insert( e.first, e.second );
            assert( false == tree.empty() );
        }
        assert( tree.validate() );

        
        auto tree_copy( tree );
        std::cout << "oi" <<std::endl;
        auto tree2 = tree_copy;
        tree2 = tree;
        tree_copy = tree;
        std::cout << "vai dar erro agora" <<std::endl;
        //tree = tree_copy; //DESCOMENTA ESSA LINHA AQUI
        
        for( const auto & e : data)
        {
            //tree = tree_copy; // restore back the original tree.
            //tree.remove( e.first ); // Remove a single node.
            //assert( tree.validate() ); // Check whether everything is fine with the tree.
        }
        
         for( const auto & e : data)
        {
            tree.remove( e.first ); // Remove a single node.
            assert( tree.validate() ); // Check whether everything is fine with the tree.
        }
        std::cout << ">>> Passed!\n\n";

    }


    }

    std::cout << "\n>>> Normal exiting...\n";
}

    return EXIT_SUCCESS;
}
