#ifndef RBT_H
#define RBT_H
#include <iostream>
#include <string>
#include <functional>


#include <utility>    // std::pair<>
#include <random>     // radom_device, mt19937, std::shuffle.
#include <algorithm>  // std::copy
#include <iterator>   // std::begin, std::end
#include <exception>  // std::out_of_range
#include <stack>      // std::stack
#include <iomanip>    // std::setw()
#include <initializer_list> // std::initializer_list

template < typename KeyType, typename ValueType>
class RBT
{
	public:
		//=== alias
		typedef std::function<bool(const KeyType& a, const KeyType& b) > KeyTypeLess;
	private :
		//! Represents a single binary search tree node.
		enum class color_e{ RED, BLACK, DOUBLE_BLACK }; //!< The colors.
		struct RBNode {
			KeyType key; //!< The unique key.
			ValueType data; //!< The data stored in a node.
			color_e color; //!< The color of the node.
			bool is_null_node; 
			RBNode *left;//!< Pointer to the left child (subtree).
			RBNode *right; //!< Pointer to the right child (subtree).
			RBNode *parent; //!< Pointer to the parent.

			RBNode( color_e c_ = color_e::RED, bool in = false, const KeyType & k_= KeyType(), const ValueType & d_= ValueType(),
                    RBNode * lt_= nullptr, RBNode * rt_= nullptr, RBNode * p_ = nullptr) 
                : key( k_ )
                , data( d_ )
				, color( c_ )
				, is_null_node( in)
                , left( lt_ )
                , right( rt_ )
                , parent( p_ )
                { /* empty */ }

            ~RBNode()
            {
            	if(left != nullptr && !left->is_null_node)
            	{
            		delete left;
            	}
            	if(right != nullptr && !right->is_null_node)
            	{
            		//std::cout << "segundo if" << std::endl;
            		delete right;
            	}       
            }
		};

		RBNode *m_root; //!< Dummy node that represents the root of the entire tree.
		size_t m_n_nodes; //!< The count of nodes stored in the tree.
		std::function<bool(const KeyType &, const KeyType &)> m_key_less; //!< The key comparator function object.
		//=== Aux pointers
		RBNode *m_current;
		RBNode *m_parent;
		RBNode *m_grand;
		RBNode *m_great;
		RBNode *m_sibling;
		RBNode *m_uncle;
		RBNode *null_node;
		//null_node->color = color_e::BLACK;
		color_e aux;

		//=== to help the operations.
		//!< Current node being processed.
		//!< The parent node of ‘m_current‘.
		//!< The grandparent node of ‘m_current‘.
		//!< The great grandparent node of ‘m_current‘.
		//!< The sibling of ‘m_current‘ node.
		//=== Extra black node to make the code simpler.
		 //!< Every null pointer of this tree points to this node.
		private :
		//=== INTERNAL MEMBERS
		void init( const KeyType & smallest_key, const KeyTypeLess & comp ) ;
		
		void rotate_right_child( RBNode *  root, RBNode * & node );

		void rotate_left_child( RBNode *  root, RBNode * & node );

		void rotate_right_child1( RBNode * root);

		void rotate_left_child1( RBNode *  root);	

		RBNode * find_node(const KeyType & key);

		void rearrange( RBNode * & root, RBNode * & inserted_node);

		void clear( RBNode * & root );

		static void print(color_e c)
		{
			std::cout << c << std::endl;
		}

		RBNode * clone( const RBNode * root );

		RBNode * getSibling( const RBNode * root);

		////////////////////////////////////////////////////////////////////
		void remove( RBNode *root, const KeyType & key);
	
		void fixRemove(RBNode *root);

		const RBNode * successor( const RBNode * root ) const;

		const RBNode * predecessor( const RBNode * root ) const;

		bool red_has_black_son(const RBNode * root) const;

		bool validate_tree( const RBNode * root ) const;

		int count_blacks( const RBNode * root ) const;

		bool key_equal( const KeyType & k1, const KeyType & k2 ) const;
		//=== Tree traversal members

		template < typename UnaryFunction >

		void preorder( const RBNode * root, const UnaryFunction & visit );

		template < typename UnaryFunction >
		
		void inorder( const RBNode * root, const UnaryFunction & visit );

		template < typename UnaryFunction >

		void postorder( const RBNode * root, const UnaryFunction & visit );

	public :

		//=== special member

		explicit RBT(const KeyTypeLess & comp = KeyTypeLess()):m_n_nodes(0), m_key_less(comp)
		{ 
			null_node = new RBNode(color_e::BLACK, true);
			m_root = null_node;
		}

		RBT( const RBT & other ) ;

		~RBT();

		RBT & operator=( const RBT & rhs );

		//=== access members

		const ValueType & find_min( void ) const;

		const ValueType & find_max( void ) const;

		bool contains( const KeyType & key ) const;

		template < typename UnaryFunction >

		void preorder( const UnaryFunction & visit );

		template < typename UnaryFunction >

		void postorder( const UnaryFunction & visit );

		void inorder()
		{
			inorder(this->m_root, print);
		}

		inline bool empty( void ) const { return m_n_nodes == 0; };

		inline size_t size( void ) const { return m_n_nodes; };

		bool retrieve( const KeyType & key, ValueType & value ) const ;

		bool validate( ) const;

		//=== MODIFIER MEMBERS

		void clear( void );

		int count_blacks( void );

		void insert( const KeyType & key , const ValueType & value);

		RBNode * insertUtil(RBNode * & root, const KeyType & key, const ValueType & value);

		void remove( const KeyType & key );
		
		friend std::ostream& operator<< ( std::ostream& os_, const RBT & root_ )
        {
            if ( root_.empty() )
            {
                os_ << "<empty tree>";
                return os_;
            }

            typedef std::pair< size_t, RBNode * > stack_type;
            std::stack< stack_type > s;
            // We have at least one element in the tree.
            s.push( std::make_pair( 0, root_.m_root ) ); // push root at level 0.

            // We follow pre-order traversal strategy.
            while( not s.empty() )
            {
                // (1) Visite the root first.
                auto visited = s.top(); s.pop();
                for( auto i(0u) ; i < visited.first ; ++i )
                    os_ << "--";
                if ( visited.second == root_.null_node )
                {
                    os_ << "X\n";
                    continue;
                }
                else
                {
                    os_ << visited.second->data  << "\n";
                }

                // (2) Now we traverse the left and right subtrees.
                // Store the children for future processing (next loop iteration).
                /*
                if ( visited.second->right == nullptr 
                        and visited.second->left == nullptr ) // avoid tail recursion.
                    continue;
                */
                // At least one child left
                //if ( visited.second->right != nullptr ) // avoid tail recursion.
                    s.push( std::make_pair( visited.first+1, visited.second->right ) );
                //if ( visited.second->left != nullptr ) // avoid tail recursion.
                    s.push( std::make_pair( visited.first+1, visited.second->left ) );
            }
            return os_;
        } 

        friend std::ostream& operator<<(std::ostream& out, const color_e color)
        {
		    const char* s = nullptr;
			#define PROCESS_VAL(p) case color_e::p: s = #p; break;
			    switch(color)
			    {
			        PROCESS_VAL(RED);     
			        PROCESS_VAL(BLACK);     
			        PROCESS_VAL(DOUBLE_BLACK);
			    }
			#undef PROCESS_VAL

		    return out << s;
		}
};
#include "rbt.inl"
#endif
