#ifndef SET_H
#define SET_H

extern bool master_dEbUg;
extern bool dEbUg;

enum Color{ Red, Black };

// TreeNode class template definition
template< typename Kty >
class TreeNode
{
   template< typename K > friend class set;
private:
   TreeNode *left;   // left subtree, or smallest element if head
   TreeNode *parent; // parent, or root of tree if head
   TreeNode *right;  // right subtree, or largest element if head
   Color color;      // red or black, black if head
   bool isNil;       // true only if head (also nil) node
   Kty myval;
}; // end class template TreeNode

// set class template definition
template< typename Kty >
class set
{
public:
   // Constructs an empty set object, with head node.
   set();

   // Destroys the set object.
   ~set();

   // Extends the set container by inserting a new element,
   // effectively increasing the container size by one.
   void insert( const Kty &val );

   // Removes from the set container a single element whose value is val
   // This effectively reduces the container size by one, which are destroyed.
   // Returns the number of elements erased.
   unsigned int erase( const Kty &val );

private:
   TreeNode< Kty > *myHead; // pointer to head node
   unsigned int mySize;     // number of elements ( not include head node )

   // Removes all elements from the set object (which are destroyed)
   void clear( TreeNode< Kty > *node );

   // rebalance for insertion
   void reBalance( TreeNode< Kty > *node );

   // rotate right at g, where p = g->left and node = p->left
   void LLRotation( TreeNode< Kty > *p );

   // rotate left at g, where p = g->right and node = p->right
   void RRRotation( TreeNode< Kty > *p );

   // erase node provided that the degree of node is at most one
   void eraseDegreeOne( TreeNode< Kty > *node );

   // rebalance for deletion
   void fixUp( TreeNode< Kty > *N, TreeNode< Kty > *P );

   void twoTraversals(); // preorder traversal and inorder traversal

   void preorder( TreeNode< Kty > *node ); // preorder traversal

   void inorder( TreeNode< Kty > *node ); // inorder traversal
}; // end class template set

// Constructs an empty set object, with head node.
template< typename Kty >
set< Kty >::set()
   : myHead( new TreeNode< Kty > ),
     mySize( 0 )
{
   myHead->left = myHead;
   myHead->parent = myHead;
   myHead->right = myHead;
   myHead->color = Black;
   myHead->isNil = true;
}

// Destroys the set object.
template< typename Kty >
set< Kty >::~set()
{
   clear( myHead->parent );
   delete myHead;
}

// Extends the container by inserting a new element,
// effectively increasing the container size by one.
template< typename Kty >
void set< Kty >::insert( const Kty& val )
{
	if (dEbUg && master_dEbUg)
	{
		cout << endl << endl;
		cout << "Before" << endl;
		twoTraversals();
	}

	TreeNode<Kty> *ptr = myHead->parent;
	bool direction; // True for right, False for left
	while (ptr != myHead)
	{
		if (val == ptr->myval)
		{
			return;
		}
		else if (val < ptr->myval)
		{
			if (!ptr->left->isNil)
			{
				ptr = ptr->left;
			}
			else {
				direction = false;
				break;
			}
		}
		else if (val > ptr->myval)
		{
			if (!ptr->right->isNil)
			{
				ptr = ptr->right;
			}
			else {
				direction = true;
				break;
			}
		}
	}

	// Init New Node
	TreeNode<Kty> *newNode = new TreeNode<Kty>;
	newNode->left = newNode->right = myHead;
	newNode->parent = ptr;
	newNode->color = Red;
	newNode->isNil = false;
	newNode->myval = val;

	// Place the newNode
	if (ptr == myHead)
	{
		myHead->parent = myHead->left = myHead->right = newNode;
	}
	else if (direction)
	{
		ptr->right = newNode;
	}
	else {
		ptr->left = newNode;
	}

	reBalance(newNode);
	mySize++;
	if (dEbUg && master_dEbUg)
	{
		cout << "After" << endl;
		twoTraversals();
		cout << endl << endl;
	}
}

// Removes from the set container a single element whose value is val
// This effectively reduces the container size by one, which are destroyed.
// Returns the number of elements erased.
template< typename Kty >
unsigned int set< Kty >::erase( const Kty &val )
{
	if (dEbUg && master_dEbUg)
	{
		cout << endl << endl;
		cout << "Before" << endl;
		twoTraversals();
	}

	// Find The Node
	TreeNode<Kty> *ptr = myHead->parent;
	while (ptr != myHead)
	{
		if (val == ptr->myval)
		{
			break;
		}
		ptr = (val < ptr->myval) ? ptr->left : ptr->right;
	}
	if (ptr == myHead)
	{
		return 0;
	}

	// Find Replacement
	if (!ptr->right->isNil)
	{
		TreeNode<Kty> *rep = ptr->right;
		while (!rep->left->isNil)
		{
			rep = rep->left;
		}
		ptr->myval = rep->myval;
		ptr = rep;
	}
	else if (!ptr->left->isNil)
	{
		TreeNode<Kty> *rep = ptr->left;
		while (!rep->right->isNil)
		{
			rep = rep->right;
		}
		ptr->myval = rep->myval;
		ptr = rep;
	}
	eraseDegreeOne(ptr);
	mySize--;
	if (dEbUg && master_dEbUg)
	{
		cout << "After" << endl;
		twoTraversals();
		cout << endl << endl;
	}
	return 1;
}

// Removes all elements from the set object (which are destroyed)
template< typename Kty >
void set< Kty >::clear( TreeNode< Kty > *node )
{
   if( !node->isNil ) // node is not an external node
   {
      clear( node->left );
      clear( node->right );
      delete node;
   }
}

// rebalance for insertion
template< typename Kty >
void set< Kty >::reBalance( TreeNode< Kty > *node )
{  // node->parent cannot be the root

	TreeNode<Kty> *pu = node->parent, *gu = pu->parent;

	if (pu->isNil)
	{
		node->color = Black;
		return;
	}
	
	if (gu->isNil)
	{
		return;
	}

	if (pu->color != Red)
	{
		return;
	}

	if (pu == gu->left) // L??
	{
		if (gu->right->color == Red) // color change
		{
			// LLr & LRr
			pu->color = gu->right->color = Black;
			if (gu != myHead->parent)
			{
				gu->color = Red;
				reBalance(gu);
			}
		}
		else // rotation
		{
			if (node == pu->left)
			{
				// LLb
				LLRotation(pu);
				std::swap(pu->color, gu->color);
			}
			else
			{
				// LRb
				RRRotation(node);
				LLRotation(node);
				std::swap(node->color, gu->color);
			}
		}
	}
	else if (pu == gu->right) // R??
	{
		if (gu->left->color == Red) // color change
		{
			// RLr & RRr
			pu->color = gu->left->color = Black;
			if (gu != myHead->parent)
			{
				gu->color = Red;
				reBalance(gu);
			}
		}
		else // rotation
		{
			if (node == pu->right)
			{
				// RRb
				RRRotation(pu);
				std::swap(pu->color, gu->color);
			}
			else
			{
				// RLb
				LLRotation(node);
				RRRotation(node);
				std::swap(node->color, gu->color);
			}
		}
	}
}

// rotate right at g, where p = g->left and node = p->left
template< typename Kty >
//void set< Kty >::LLbRotation( TreeNode< Kty > *node )
void set< Kty >::LLRotation( TreeNode< Kty > *p )
{
	p->parent->left = p->right;
	if (p->parent->parent == myHead)
	{
		myHead->parent = p;
	}
	else if (p->parent == p->parent->parent->left)
	{
		p->parent->parent->left = p;
	}
	else if (p->parent == p->parent->parent->right)
	{
		p->parent->parent->right = p;
	}
	if (p->right != myHead)
	{
		p->right->parent = p->parent;
	}
	p->right = p->parent;
	p->parent = p->right->parent;
	p->right->parent = p;
}

// rotate left at g, where p = g->right and node = p->right
template< typename Kty >
void set< Kty >::RRRotation( TreeNode< Kty > *p )
{
	p->parent->right = p->left;
	if (p->parent->parent == myHead)
	{
		myHead->parent = p;
	}
	else if (p->parent == p->parent->parent->left)
	{
		p->parent->parent->left = p;
	}
	else if (p->parent == p->parent->parent->right)
	{
		p->parent->parent->right = p;
	}
	if (p->left != myHead)
	{
		p->left->parent = p->parent;
	}
	p->left = p->parent;
	p->parent = p->left->parent;
	p->left->parent = p;
}

// erase node provided that the degree of node is at most one
template< typename Kty >
void set< Kty >::eraseDegreeOne( TreeNode< Kty > *node )
{
	bool direction = node->left->isNil; // True for right, False for left
	TreeNode<Kty> *onlyChild = (direction) ? node->right : node->left;

	// Simple Case 3
	if (node->color == Black && onlyChild->color == Black && node == myHead->parent)
	{
		myHead->parent = myHead->left = myHead->right = onlyChild;
		if (onlyChild != myHead)
		{
			onlyChild->parent = myHead;
		}
		delete node;
		return;
	}

	// Simple Case 1
	if (node->color == Red)
	{
		if (node == node->parent->left)
		{
			node->parent->left = onlyChild;
		}
		else {
			node->parent->right = onlyChild;
		}
		if (onlyChild != myHead)
		{
			onlyChild->parent = node->parent;
		}
		delete node;
		return;
	}

	// Simple Case 2
	if (node->color == Black && onlyChild->color == Red)
	{
		if (node == node->parent->left)
		{
			node->parent->left = onlyChild;
		}
		else {
			node->parent->right = onlyChild;
		}
		if (onlyChild != myHead)
		{
			onlyChild->parent = node->parent;
			onlyChild->color = Black;
		}
		delete node;
		return;
	}

	fixUp(node, node->parent);
	if (node == node->parent->left)
	{
		node->parent->left = onlyChild;
	}
	else {
		node->parent->right = onlyChild;
	}
	if (onlyChild != myHead)
	{
		onlyChild->parent = node->parent;
	}
	delete node;
}

// rebalance for deletion
template< typename Kty >
void set< Kty >::fixUp( TreeNode< Kty > *N, TreeNode< Kty > *P )
{
	bool N_pos_of_P = N == P->right; // True for right, False for left
	TreeNode<Kty> *Sibling = (N_pos_of_P) ? P->left : P->right;

	// Case 1
	if (Sibling->color == Red)
	{
		std::swap(P->color, Sibling->color);
		if (N_pos_of_P)
		{
			LLRotation(Sibling);
		}
		else {
			RRRotation(Sibling);
		}
		Sibling = (N == P->left) ? P->right : P->left;
		//return;
	}

	// Case 2
	if (Sibling->color == Black && ((Sibling->right->color == Red && !N_pos_of_P) || (Sibling->left->color == Red && N_pos_of_P)))
	{
		if (N_pos_of_P)
		{
			LLRotation(Sibling);
		}
		else {
			RRRotation(Sibling);
		}
		std::swap(P->color, Sibling->color);
		if (N_pos_of_P)
		{
			Sibling->left->color = Black;
		}
		else {
			Sibling->right->color = Black;
		}
		Sibling = (N == P->left) ? P->right : P->left;
		return;
	}

	// Case 3
	if (Sibling->color == Black && ((Sibling->left->color == Red && !N_pos_of_P) || (Sibling->right->color == Red && N_pos_of_P)))
	{
		if (N_pos_of_P)
		{
			RRRotation(Sibling->right);
		}
		else {
			LLRotation(Sibling->left);
		}
		std::swap(Sibling->color, Sibling->parent->color);
		Sibling = (N == P->left) ? P->right : P->left;
		fixUp(N, N->parent);
		return;
	}

	// Case 4
	if (Sibling->color == Black && Sibling->right->color == Black && Sibling->left->color == Black && P->color == Red)
	{
		std::swap(P->color, Sibling->color);
		return;
	}

	// Case 5
	if (Sibling->color == Black && Sibling->right->color == Black && Sibling->left->color == Black && P->color == Black)
	{
		Sibling->color = Red;
		if (P != myHead->parent)
		{
			fixUp(P, P->parent);
		}
		return;
	}
}

// preorder traversal and inorder traversal
template< typename Kty >
void set< Kty >::twoTraversals()
{
   cout << "Preorder sequence:\n";
   preorder( myHead->parent );

   cout << "\nInorder sequence:\n";
   inorder( myHead->parent );
   cout << endl;
}

// preorder traversal
template< typename Kty >
void set< Kty >::preorder( TreeNode< Kty > *node )
{
   if( node != myHead )
   {
      cout << setw( 7 ) << node->myval << ( node->color == Red ? "R" : "B" );
      preorder( node->left );
      preorder( node->right );
   }
}

// inorder traversal
template< typename Kty >
void set< Kty >::inorder( TreeNode< Kty > *node )
{
   if( node != myHead )
   {
      inorder( node->left );
      cout << setw( 7 ) << node->myval << ( node->color == Red ? "R" : "B" );
      inorder( node->right );
   }
}

#endif