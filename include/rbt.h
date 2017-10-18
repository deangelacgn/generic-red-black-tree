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

template < typename KeyType, typename ValueType, typename KeyTypeLess=std::less<KeyType> >
class RBT
{
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
		
		void rotate_right_child( RBNode * & root, RBNode * & node )
		{
			RBNode * aux = root->right;
			root->right = aux->left;

			if(aux->left != null_node)
			{
				aux->left->parent = root;
			}

			aux->parent = root->parent;

			if(root->parent == null_node)
			{
				m_root = aux;
			}
			else if(root == root->parent->left)
			{
				root->parent->left = aux;
			}
			else
			{
				root->parent->right = aux;
			}

			aux->left = root;
			root->parent = aux;
		}

		void rotate_left_child( RBNode * & root, RBNode * & node )
		{
			RBNode * aux = root->left;
			root->left = aux->right;

			if(aux->right != null_node)
			{
				aux->right->parent = root;
			}

			aux->parent = root->parent;

			if(root->parent == null_node)
			{
				m_root = aux;
			}
			else if(root == root->parent->right)
			{
				root->parent->right = aux;
			}
			else
			{
				root->parent->left = aux;
			}

			aux->right = root;
			root->parent = aux;
		}

		void rotate_right_child1( RBNode * root)
		{
			RBNode * aux = root->right;
			root->right = aux->left;

			if(aux->left != null_node)
			{
				aux->left->parent = root;
			}

			aux->parent = root->parent;

			if(root->parent == null_node)
			{
				m_root = aux;
			}
			else if(root == root->parent->left)
			{
				root->parent->left = aux;
			}
			else
			{
				root->parent->right = aux;
			}

			aux->left = root;
			root->parent = aux;
		}

		void rotate_left_child1( RBNode *  root)
		{				
			RBNode * aux = root->left;
			root->left = aux->right;
			

			if(aux->right != null_node)
			{	
				aux->right->parent = root;
			}

			aux->parent = root->parent;
			if(root->parent == null_node)
			{  
				m_root = aux;
			}
			else if(root == root->parent->right)
			{   
				root->parent->right = aux;
			}
			else
			{   
				root->parent->left = aux;
			}
			aux->right = root;
			root->parent = aux;
		}		

		RBNode * findNode(const KeyType & key)
		{
			RBNode * temp = this->m_root;

			while(temp != null_node)
            {
                if(m_key_less(temp->key, key))
                {
                    temp = temp->left;
                }
                else if(!m_key_less(temp->key, key) && !m_key_less(key, temp->key))
                {
                    return temp;
                }
                else
                {
                    temp = temp->right;
                }
            }

            return null_node;
		}


		void rearrange( RBNode * & root, RBNode * & inserted_node) 
		{
			while((inserted_node != root) && (inserted_node->color != color_e::BLACK) && (inserted_node->parent->color == color_e::RED))
			{
				m_parent = inserted_node->parent;
				m_grand = inserted_node->parent->parent;

				if(m_parent == m_grand ->left)
				{
					m_uncle = m_grand->right; //uncle

					if((m_uncle != null_node) && (m_uncle->color == color_e::RED))
					{
						m_grand->color = color_e::RED;
						m_parent->color = color_e::BLACK;
						m_uncle->color = color_e::BLACK;
						inserted_node = m_grand;
					}
					else
					{
						if(inserted_node == m_parent->right)
						{
							rotate_right_child(m_root, m_parent);
							inserted_node = m_parent;
							m_parent = inserted_node->parent;
						}

						rotate_left_child(m_root, m_grand);

						//Change colors
						aux = m_parent->color;
						m_parent->color = m_grand->color;
						m_grand->color = aux;

						inserted_node = m_parent;

					}
				}
				else
				{
					m_uncle = m_grand->left;

					if((m_uncle != null_node) && (m_uncle->color == color_e::RED))
					{
						m_grand->color = color_e::RED;
						m_parent->color = color_e::BLACK;
						m_uncle->color = color_e::BLACK;
						inserted_node = m_grand;
					}
					else
					{
						if(inserted_node == m_parent->left)
						{
							rotate_left_child(m_root, m_parent);
							inserted_node = m_parent;
							m_parent = inserted_node->parent;
						}

						rotate_right_child(m_root, m_grand);

						// Change colors
						aux = m_parent->color;
						m_parent->color = m_grand->color;
						m_grand->color = aux;

						inserted_node = m_parent;
					}

				}
			}

			root->color = color_e::BLACK;
		}

		void clear( RBNode * & root )
		{
			delete root;
			root = null_node;
			m_n_nodes = 0;
		}

		static void print(color_e c)
		{
			std::cout << c << std::endl;
		}
  		

		RBNode * clone( const RBNode * root )
		{
			if(root == null_node)
            {
                return null_node;
            }

            RBNode * new_node = new RBNode(root->color, root->is_null_node, root->key, root->data);

            new_node->left = clone(root->left);
            new_node->right = clone(root->right);

            return new_node;
		}

		RBNode * getSibling( const RBNode * root){
		
			
			if(root->parent->left == root){
				return root->parent->right;
			}
			else if(root->parent->right == root){
				return root->parent->left;
			}
			return null_node;
		
		}
		////////////////////////////////////////////////////////////////////
		void remove( RBNode *root, const KeyType & key)
		{		
			RBNode * temp = new RBNode();				
			RBNode * temp2 = new RBNode();
			if(successor(root) != root){			
				temp = (RBNode*)successor(root);
				temp2 = temp->left;
			}else if((predecessor(root) != root)){
				temp = (RBNode*)predecessor(root);
				temp2 = temp->right;
			}else{					
				temp = (RBNode*)root;
				temp2 = null_node;
			}
			
			//substituindo por root por temp	    
			
			/*if(root->parent != null_node){
				if(root->parent->left == root){
					root->parent->left = temp;
				}
	            else{
					root->parent->right = temp;
				}
			}*/
		
			if(temp->parent == null_node){
				m_root = temp2;

			}else if(temp->parent->right == temp){
				temp->parent->right = temp2; 
			
			}else {								
				temp->parent->left = temp2;
			
			}

			if(temp2 != null_node){
				temp2->parent = temp->parent;
			}
			/*
			temp->left = root->left;
			temp->right = root->right;
	    	temp->parent = root->parent;
			temp->color = root->color;	
			*/
			if(root != temp){
				if(root != m_root)				
					root->color = temp->color;
				root->key = temp->key;
	 	    	root->data = temp->data;
			}
			
			//
			if(temp->color == color_e::BLACK){
				if(temp2->color == color_e::RED){  //ou temp2 == red ou temp2==null_node
					temp2->color = color_e::BLACK;
				}else{
					temp2->parent = temp->parent;				
					fixRemove(temp2);
					temp2->parent = nullptr;
				}
			}
			//
			delete temp;
			temp = nullptr;	


		}
	
		void fixRemove(RBNode *root){
                                               //nó duplo preto diabo		
			std::cout<<"No duplo"<<std::endl;
			while(true){
				if(root->right != null_node){     //////////// NÓ DUPLO NA ESQUERDA
					if(getSibling(root)->color == color_e::RED){                          //caso 1
						std::cout<<"Caso 1"<<std::endl;							
						root->parent->color=color_e::RED;   							
						getSibling(root)->color=color_e::BLACK;
						rotate_right_child1(root->parent);	

					}
					else if(getSibling(root)->left->color == color_e::BLACK && getSibling(root)->right->color == color_e::BLACK){ //caso 2
						std::cout<<"Caso 2"<<std::endl;
						getSibling(root)->color = color_e::RED;
						if(root->parent != null_node)						
							root=root->parent;
						else
							break;
						if(root->color == color_e::RED){
							root->color=color_e::BLACK;
							break;
						}
					}
					else if(getSibling(root)->right->color == color_e::BLACK){                     
						std::cout<<"Caso 3"<<std::endl;							
						getSibling(root)->color=color_e::RED;
						getSibling(root)->left->color=color_e::BLACK; 
						rotate_left_child1(root->parent->right);

						}else{
						//caso 4
					
						std::cout<<"Caso 4"<<std::endl;							
						getSibling(root)->color = root->parent->color;								
						root->parent->color = color_e::BLACK;							
						getSibling(root)->right->color = color_e::BLACK;											
						rotate_right_child1(root->parent);
						//root->parent->left = null_node;
						break;
						}
				}
				else{						//////////// NÓ DUPLO NA DIREITA
					if(getSibling(root)->color == color_e::RED){                          //caso 1
						std::cout<<"Caso 1"<<std::endl;							
						root->parent->color=color_e::RED;   							
						getSibling(root)->color=color_e::BLACK;
						rotate_left_child1(root->parent);	

					}
					else if(getSibling(root)->left->color == color_e::BLACK && getSibling(root)->right->color == color_e::BLACK){ //caso 2
						std::cout<<"Caso 2"<<std::endl;
						getSibling(root)->color = color_e::RED;
						if(root->parent != null_node)						
							root=root->parent;
						else
							break;
						if(root->color == color_e::RED){
							root->color=color_e::BLACK;
							break;
						}
					}
					else if(getSibling(root)->left->color == color_e::BLACK){                     
						std::cout<<"Caso 3"<<std::endl;							
						getSibling(root)->color=color_e::RED;
						getSibling(root)->right->color=color_e::BLACK; 
						rotate_right_child1(root->parent->left);

						}else{
						//caso 4
					
						std::cout<<"Caso 4"<<std::endl;							
						getSibling(root)->color = root->parent->color;								
						root->parent->color = color_e::BLACK;							
						getSibling(root)->left->color = color_e::BLACK;											
						rotate_left_child1(root->parent);
						//root->parent->left = null_node;
						break;
						}
				}//termina o else		
			}//termina o while
		}//termina a função
		const RBNode * successor( const RBNode * root ) const
		{
			if(root == null_node)
			{
				return null_node;
			}

			if(root->right == null_node)
			{
				return root;
			}

			root = root->right;
			while(root->left != null_node)
			{
				root = root->left;
			}

			return root;
		}
	
		const RBNode * predecessor( const RBNode * root ) const
				
		{
			if(root == null_node)
            {   
                return null_node;
            }

            if(root->left == null_node)
            {
            	return root;
            }

            root = root->left;
       		while(root->right != null_node)
       		{
       			root = root->right;
       		}

       		return root;
		}

		bool red_has_black_son(const RBNode * root) const
		{
			if(root == null_node)
			{
				return true;
			}
			if(root->color == color_e::RED)
			{
				if(!((root->left->color == color_e::BLACK) && (root->right->color == color_e::BLACK)))
				{
					return false;
				}
			}
		
			return red_has_black_son(root->left) && red_has_black_son(root->right);

		}

		bool validate_tree( const RBNode * root ) const
		{
			return (root->color == color_e::BLACK) && red_has_black_son(root) &&  (count_blacks(root) != -1);
		}

		int count_blacks( const RBNode * root ) const 
		{
			if(root != null_node)
			{
				int left_subtree, right_subtree;

				if (root->color == color_e::BLACK)
				{
					left_subtree = count_blacks(root->left);
					right_subtree = count_blacks(root->right);

					if(left_subtree == right_subtree)
					{
						return 1 + left_subtree;
					}
					else
					{
						return -1;
					}
				}
				else
				{
					left_subtree =  count_blacks(root->left);
					right_subtree = count_blacks(root->right);

					if(left_subtree == right_subtree)
					{
						return left_subtree;
					}
					else
					{
						return -1;
					}
				}
			}
			
			return 1;
			
		}

		bool key_equal( const KeyType & k1, const KeyType & k2 ) const
		{
			if(!m_key_less(k1, k2) && !m_key_less(k2, k1))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		//=== Tree traversal members

		template < typename UnaryFunction >

		void preorder( const RBNode * root, const UnaryFunction & visit )
		{
			if(root == null_node)
            {
                return;
            }
            
            visit(root->data);

            preorder(root->left, visit);
            preorder(root->right, visit);
		}

		template < typename UnaryFunction >
		
		void inorder( const RBNode * root, const UnaryFunction & visit )
		{
			if(root == null_node)
            {
                return;
            }

            inorder(root->left, visit);
            
            visit(root->color);
            
            inorder(root->right, visit);
		}

		template < typename UnaryFunction >

		void postorder( const RBNode * root, const UnaryFunction & visit ) 
		{
			if(root == null_node)
            {
                return;
            }

            postorder(root->left, visit);
            postorder(root->right, visit);

            visit(root->data);

		}

		void delete_postorder( const RBNode * root ) 
		{
			if(m_root != nullptr)
			{
				if(root == null_node)
            	{
                	return;
            	}

	            delete_postorder(root->left);
	            delete_postorder(root->right);

	            delete root;
	            root = nullptr;
			}
			else
			{
				throw std::runtime_error("function called upon an empty tree");
			}
		}


	public :

		//=== special member

		explicit RBT(const KeyTypeLess & comp = KeyTypeLess()):m_n_nodes(0), m_key_less(comp)
		{ 
			null_node = new RBNode(color_e::BLACK, true);
			m_root = null_node;
		}

		RBT( const RBT & other ):m_key_less(other.m_key_less) 
		{
			*this = other;
		}

		~RBT()
		{
			//delete_postorder(m_root);
			delete m_root;
			//delete null_node;
		}

		RBT & operator=( const RBT & rhs )
		{
			delete m_root;
            m_root = clone(rhs.m_root);
            m_n_nodes = rhs.m_n_nodes;
            m_key_less = rhs.m_key_less;

            return *this;
		}


		//=== access members

		const ValueType & find_min( void ) const
		{
			if(m_root != null_node)
            {
                RBNode * temp = m_root;
                while(temp->left != null_node)
                {
                    temp = temp->left;
                }

                return temp->data;
            }
            else
            {
            	throw std::runtime_error("find_max: called upon an empty tree");
            }
		}

		const ValueType & find_max( void ) const
		{
			if(m_root != null_node)
            {
                RBNode * temp = m_root;
                while(temp->right != null_node)
                {
                    temp = temp->right;
                }

                return temp->data;
            }
            else
            {
            	throw std::runtime_error("find_max: called upon an empty tree");
            }
		}

		bool contains( const KeyType & key ) const
		{
			RBNode * temp = this->m_root;

			while(temp != null_node)
            {
                if(m_key_less(temp->key, key))
                {
                    temp = temp->left;
                }
                else if(!m_key_less(temp->key, key) && !m_key_less(key, temp->key))
                {
                    return true;
                }
                else
                {
                    temp = temp->right;
                }
            }

            return false;
		}

		template < typename UnaryFunction >

		void preorder( const UnaryFunction & visit )
		{
			preorder(this->m_root, visit);
		}

		template < typename UnaryFunction >

		void postorder( const UnaryFunction & visit ) 
		{
			postorder(this->m_root, visit);
		}

		

		void inorder()
		{
			inorder(this->m_root, print);
		}

		inline bool empty( void ) const { return m_n_nodes == 0; };

		inline size_t size( void ) const { return m_n_nodes; };

		bool retrieve( const KeyType & key, ValueType & value ) const 
		{
			RBNode * temp = m_root;

			while(temp != null_node)
            {
                if(m_key_less(temp->key, key))
                {
                    temp = temp->left;
                }
                else if(!m_key_less(temp->key, key) && !m_key_less(key, temp->key))
                {
                    value = temp->data;
                    return true;
                }
                else
                {
                    temp = temp->right;
                }
            }
            return false;
		}

		bool validate( ) const
		{
			//return (m_root->color == color_e::BLACK) && red_has_black_son(m_root) && (count_blacks(m_root) != -1);
			return validate_tree(m_root);
		}

		//=== MODIFIER MEMBERS

		void clear( void )
		{
			clear(m_root);
		}

		int count_blacks( void )
		{
			return count_blacks(m_root);
		}  

		void insert( const KeyType & key , const ValueType & value)
		{
			RBNode * root = insertUtil(m_root, key, value);
			m_current = findNode(key);
			rearrange(m_root, m_current);

		}

		RBNode * insertUtil(RBNode * & root, const KeyType & key, const ValueType & value)
		{	
			if(contains(key))
            {
                return root;
            }
            else
            {
                if(root == null_node)
                {
                    root = new RBNode(color_e::RED, false, key, value, null_node, null_node, null_node);
                    m_n_nodes++;
                }
                else if(m_key_less(root->key, key))
                {
                    RBNode * left_child = insertUtil(root->left, key, value);
					root->left = left_child;
					left_child->parent = root;
                }
                else
                {
                	RBNode * right_child = insertUtil(root->right, key, value);
					root->right = right_child;
					right_child->parent = root;
                }

                return root;
            }
		}

		void remove( const KeyType & key )
		{
			if(contains(key))
            {
				RBNode * temp = new RBNode();
				temp = this->m_root;

				while(temp != null_node)
		        {
		            if(m_key_less(temp->key, key))
		            {
		                temp = temp->left;
		            }
		            else if(!m_key_less(temp->key, key) && !m_key_less(key, temp->key))
		            {
		                remove(temp,key);
						return;
		            }
		            else
		            {
		                temp = temp->right;
		            }
					if(temp==null_node){
						return;		
					}
		        }

		    	delete temp;
		    	temp = nullptr;		
			}
		
		}

		
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
#endif
