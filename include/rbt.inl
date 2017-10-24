#ifndef RBT_INL_H
#define RBT_INL_H
#include "rbt.h"

template < typename KeyType, typename ValueType >
void RBT< KeyType, ValueType >::rotate_right_child( RBNode *  node )
{
	RBNode * aux = node->right;
	node->right = aux->left;

	if(aux->left != null_node)
	{
		aux->left->parent = node;
	}

	aux->parent = node->parent;

	if(node->parent == null_node)
	{
		m_root = aux;
	}
	else if(node == node->parent->left)
	{
		node->parent->left = aux;
	}
	else
	{
		node->parent->right = aux;
	}

	aux->left = node;
	node->parent = aux;
}

template < typename KeyType, typename ValueType >
void RBT< KeyType, ValueType >::rotate_left_child( RBNode *  node )
{
	RBNode * aux = node->left;
	node->left = aux->right;

	if(aux->right != null_node)
	{
		aux->right->parent = node;
	}

	aux->parent = node->parent;

	if(node->parent == null_node)
	{
		m_root = aux;
	}
	else if(node == node->parent->right)
	{
		node->parent->right = aux;
	}
	else
	{
		node->parent->left = aux;
	}

	aux->right = node;
	node->parent = aux;
}

template < typename KeyType, typename ValueType >
typename RBT< KeyType, ValueType >::RBNode * RBT< KeyType, ValueType >::find_node(const KeyType & key)
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

template < typename KeyType, typename ValueType >
bool RBT< KeyType, ValueType >::equal( const RBNode* a, const RBNode* b, const RBT< KeyType, ValueType > & other) const
{

	if(a == this->null_node && b == other.null_node)
	{
		return true;
	}
	else if(a != null_node && b != null_node)
	{
		return key_equal(a->key, b->key) && equal(a->left, b->left, other) && equal(a->right, b->right, other);
	}

	return false;
}


template < typename KeyType, typename ValueType >
void RBT< KeyType, ValueType >::rearrange( RBNode * & root, RBNode * & inserted_node)
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
					rotate_right_child(m_parent);
					inserted_node = m_parent;
					m_parent = inserted_node->parent;
				}

				rotate_left_child(m_grand);

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
					rotate_left_child(m_parent);
					inserted_node = m_parent;
					m_parent = inserted_node->parent;
				}

				rotate_right_child(m_grand);

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

template < typename KeyType, typename ValueType >
void RBT< KeyType, ValueType >::clear( RBNode * & root )
{
	delete root;
	root = null_node;
	m_n_nodes = 0;
}

template < typename KeyType, typename ValueType >
typename RBT< KeyType, ValueType >::RBNode * RBT< KeyType, ValueType >::clone( const RBNode * root)
{
	if(root->is_null_node)
    {
        return null_node;
    }

    RBNode * new_node = new RBNode(root->color, root->is_null_node, root->key, root->data);

    new_node->left = clone(root->left);
    new_node->right = clone(root->right);

    if(new_node->left != null_node)
    {
    	new_node->left->parent = new_node;
    }
    if(new_node->right != null_node)
    {
    	new_node->right->parent = new_node;
    }

    new_node->parent = null_node;

    return new_node;
}

template < typename KeyType, typename ValueType >
typename RBT< KeyType, ValueType >::RBNode * RBT< KeyType, ValueType >::getSibling( const RBNode * root)
{
	if(root->parent->left == root){
		return root->parent->right;
	}
	else if(root->parent->right == root){
		return root->parent->left;
	}
	return null_node;
}

template < typename KeyType, typename ValueType >
void RBT< KeyType, ValueType >::remove( RBNode *root, const KeyType & key)
{	//std::cout<<"remove"<<std::endl;
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

	if(root != temp){

		root->key = temp->key;
	    root->data = temp->data;
	}

	if(temp->color == color_e::BLACK){
		//std::cout<<"ai"<<std::endl;
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

template < typename KeyType, typename ValueType >
void RBT< KeyType, ValueType >::fixRemove(RBNode *root){
                                       //nó duplo preto diabo
	//std::cout<<"No duplo"<<std::endl;

	while(root != m_root && root->color==color_e::BLACK ){
		//std::cout<<"1"<<std::endl;

		//std::cout<<getSibling(root)->left->color<<std::endl;
		//std::cout<<"2"<<std::endl;

		if(root->parent->left == root){     //////////// NÓ DUPLO NA ESQUERDA
			if(getSibling(root)->color == color_e::RED){                          //caso 1
				//std::cout<<"Caso 1"<<std::endl;
				root->parent->color=color_e::RED;
				getSibling(root)->color=color_e::BLACK;
				rotate_right_child(root->parent);

			}
			else if(getSibling(root)->left->color == color_e::BLACK && getSibling(root)->right->color == color_e::BLACK){ //caso 2
				//std::cout<<"Caso 2"<<std::endl;
				getSibling(root)->color = color_e::RED;
				//if(root->parent != null_node)
				root=root->parent;
				//else
				//	break;
				/*if(root->color == color_e::RED){
					root->color=color_e::BLACK;
					break;
				}*/
			}
			else if(getSibling(root)->right->color == color_e::BLACK){
				//std::cout<<"Caso 3"<<std::endl;
				getSibling(root)->color=color_e::RED;
				getSibling(root)->left->color=color_e::BLACK;
				rotate_left_child(root->parent->right);

				}else{
				//caso 4

				//std::cout<<"Caso 4"<<std::endl;
				getSibling(root)->color = root->parent->color;
				root->parent->color = color_e::BLACK;
				getSibling(root)->right->color = color_e::BLACK;
				rotate_right_child(root->parent);
				//root->parent->left = null_node;
				break;
				}
		}
		else{						//////////// NÓ DUPLO NA DIREITA
			if(getSibling(root)->color == color_e::RED){                          //caso 1
				//std::cout<<"Caso 1"<<std::endl;
				root->parent->color=color_e::RED;
				getSibling(root)->color=color_e::BLACK;
				rotate_left_child(root->parent);

			}
			else if(getSibling(root)->left->color == color_e::BLACK && getSibling(root)->right->color == color_e::BLACK){ //caso 2
				//std::cout<<"Caso 2"<<std::endl;
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
				//std::cout<<"Caso 3"<<std::endl;
				getSibling(root)->color=color_e::RED;
				getSibling(root)->right->color=color_e::BLACK;
				rotate_right_child(root->parent->left);

				}else{
				//caso 4

				//std::cout<<"Caso 4"<<std::endl;
				getSibling(root)->color = root->parent->color;
				root->parent->color = color_e::BLACK;
				getSibling(root)->left->color = color_e::BLACK;
				rotate_left_child(root->parent);
				//root->parent->left = null_node;
				break;
				}
		}//termina o else
	}//termina o while
	root->color=color_e::BLACK;
	m_root->color=color_e::BLACK;
}//termina a função

template < typename KeyType, typename ValueType >
const typename RBT< KeyType, ValueType >::RBNode * RBT< KeyType, ValueType >::successor( const RBNode * root ) const
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

template < typename KeyType, typename ValueType >
const typename RBT< KeyType, ValueType >::RBNode * RBT< KeyType, ValueType >::predecessor( const RBNode * root ) const
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

template < typename KeyType, typename ValueType >
bool RBT< KeyType, ValueType >::red_has_black_son(const RBNode * root) const
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

template < typename KeyType, typename ValueType >
bool RBT< KeyType, ValueType >::validate_tree( const RBNode * root ) const
{
	return (root->color == color_e::BLACK) && red_has_black_son(root) &&  (count_blacks(root) != -1);
}

template < typename KeyType, typename ValueType >
int RBT< KeyType, ValueType >::count_blacks( const RBNode * root ) const
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

template < typename KeyType, typename ValueType >
bool RBT< KeyType, ValueType >::key_equal( const KeyType & k1, const KeyType & k2 ) const
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

template < typename KeyType, typename ValueType >
template < typename UnaryFunction >
void RBT< KeyType, ValueType >::preorder( const RBNode * root, const UnaryFunction & visit )
{
	if(root == null_node)
    {
        return;
    }

    visit(root->data);

    preorder(root->left, visit);
    preorder(root->right, visit);
}

template < typename KeyType, typename ValueType >
template < typename UnaryFunction >
void RBT< KeyType, ValueType >::inorder( const RBNode * root, const UnaryFunction & visit )
{
	if(root == null_node)
    {
        return;
    }

    inorder(root->left, visit);

    visit(root->data);

    inorder(root->right, visit);
}

template < typename KeyType, typename ValueType >
template < typename UnaryFunction >
void RBT< KeyType, ValueType >::postorder( const RBNode * root, const UnaryFunction & visit )
{
	if(root == null_node)
    {
        return;
    }

    postorder(root->left, visit);
    postorder(root->right, visit);

    visit(root->data);

}

template < typename KeyType, typename ValueType >
RBT< KeyType, ValueType >::RBT( const RBT< KeyType, ValueType > & other ):
	RBT(other.m_key_less)
	{
	// inicializar m_root, null_node e m_n_nodes
		*this = other;
	}

template < typename KeyType, typename ValueType >
RBT< KeyType, ValueType >::~RBT()
{
    if (m_root != null_node)
    {
    	delete m_root;
    }

	delete null_node;
}

template < typename KeyType, typename ValueType >
RBT< KeyType, ValueType > & RBT< KeyType, ValueType >::operator=( const RBT< KeyType, ValueType > & rhs )
{
	if(m_root->is_null_node == false)
	{
		delete m_root;
	}


    m_root = clone(rhs.m_root);
    m_n_nodes = rhs.m_n_nodes;
    m_key_less = rhs.m_key_less;

    return *this;
}

template < typename KeyType, typename ValueType >
bool RBT< KeyType, ValueType >::operator==( const RBT< KeyType, ValueType > & other ) const
{
	return equal(this->m_root, other.m_root, other);
}

template < typename KeyType, typename ValueType >
const ValueType & RBT< KeyType, ValueType >::find_min( void ) const
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

template < typename KeyType, typename ValueType >
const ValueType & RBT< KeyType, ValueType >::find_max( void ) const
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

template < typename KeyType, typename ValueType >
bool RBT< KeyType, ValueType >::contains( const KeyType & key ) const
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

template < typename KeyType, typename ValueType >
template < typename UnaryFunction >
void RBT< KeyType, ValueType >::preorder( const UnaryFunction & visit )
{
	preorder(this->m_root, visit);
}

template < typename KeyType, typename ValueType >
template < typename UnaryFunction >
void RBT< KeyType, ValueType >::postorder( const UnaryFunction & visit )
{
	postorder(this->m_root, visit);
}

template < typename KeyType, typename ValueType >
template < typename UnaryFunction >
void RBT< KeyType, ValueType >::inorder( const UnaryFunction & visit )
{
	inorder(this->m_root, visit);
}

template < typename KeyType, typename ValueType >
bool RBT< KeyType, ValueType >::retrieve( const KeyType & key, ValueType & value ) const
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

template < typename KeyType, typename ValueType >
bool RBT< KeyType, ValueType >::validate( ) const
{
	return validate_tree(m_root);
}

template < typename KeyType, typename ValueType >
void RBT< KeyType, ValueType >::clear( void )
{
	clear(m_root);
}

template < typename KeyType, typename ValueType >
int RBT< KeyType, ValueType >::count_blacks( void )
{
	return count_blacks(m_root);
}

template < typename KeyType, typename ValueType >
void RBT< KeyType, ValueType >::insert( const KeyType & key , const ValueType & value)
{
	RBNode * root = insertUtil(m_root, key, value);
	m_current = find_node(key);
	rearrange(m_root, m_current);
}

template < typename KeyType, typename ValueType >
typename RBT< KeyType, ValueType >::RBNode * RBT< KeyType, ValueType >::insertUtil(RBNode * & root, const KeyType & key, const ValueType & value)
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

template < typename KeyType, typename ValueType >
void RBT< KeyType, ValueType >::remove( const KeyType & key )
{
	if(contains(key))
    {
		RBNode * temp = new RBNode();
		temp = this->m_root;
		m_n_nodes--;

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
#endif
