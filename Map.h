#include <cassert>

#pragma once

template<class K, class V>
class Map {
	enum Color{ BLACK, RED };

	struct Node {
		Node() 
		{
			l = 0;
			r = 0; p = 0;
		}
		
		
		Node(K _key, V _value) : key( _key ), value( _value ) 
		{
			l = 0;
			r = 0;
			p = 0;
		}

		~Node() {
			if( l != nil ) {
				delete l;
			}
			if( r != nil ) {
				delete r;
			}
		}

		K key;
		V value;
		Node* l;
		Node* r;
		Node* p;
		Color c;
	};

public:
	Map() { 
		if( nil == 0 ) { 
			nil = new Node(); 
			nil->p = 0; 
			nil->l = nil; 
			nil->r = nil; 
			nil->c = BLACK; 
		}
		root = nil; 
	}
	~Map() { if( root != nil ) { delete root; } }
	
	void add( const K& key, const V& value );
	void remove( const K& key );
	V get( const K& key );

private:
	Node* find( const K& key );
	void transplant( Node* u, Node* v );

	void rotate_left( Node* x );
	void rotate_right( Node* x );

	Node* tree_minimum( Node* x );
	
	void add_fix( Node* z );
	void remove_fix( Node* x );

	Node* root;
	static Node* nil;
};

template<class K, class V>
typename Map<K,V>::Node* Map<K,V>::nil = 0;

template<class K, class V>
typename Map<K,V>::Node* Map<K, V>::find( const K& key ) 
{
	Node* curr = root;
	while( true ) {
		if( curr == nil || key == curr->key ) {
			return curr;
		} else if( key < curr->key ) {
			curr = curr->l;
		} else {
			curr = curr->r;
		}
	}
	return 0;
}

template<class K, class V>
void Map<K, V>::add( const K& key, const V& value )
{
	Node* z = new Node(key, value);
	Node* y = 0;
	Node* x = root;
	while( x != nil ) {
		y = x;
		if( z->key < x->key ) {
			x = x->l;
		} else {
			x = x->r;
		}
	}
	z->p = y;
	if( y == 0 ) {
		root = z;
	} else if( z->key < y->key ) {
		y->l = z;
	} else {
		y->r = z;
	}
	z->l = nil;
	z->r = nil;
	z->c = RED;
	add_fix( z );
}

template<class K, class V>
void Map<K,V>::remove( const K& key )
{
	Node* z = find( key );
	assert( z != nil );
	
	Node* y = z;
	Color orig = y->c;
	Node* x = 0;

	if( z->l == nil ) {
		x = z->r;
		transplant( z, z->r );
	} else if( z->r == nil ) {
		x = z->l;
		transplant( z, z->l );
	} else {
		y = tree_minimum( z->r );
		orig = y->c;
		x = y->r;
		if( y->p == z ) {
			x->p = y;
		} else {
			transplant( y, y->r );
			y->r = z->r;
			y->r->p = y;
		}
		transplant( z, y );
		y->l = z->l;
		y->l->p = y;
		y->c = z->c;
	}
	if( orig == BLACK ) {
		remove_fix( x );
	}
	z->l = 0;
	z->r = 0;
	delete( z );
}

template<class K, class V>
V Map<K,V>::get( const K& key )
{
	return find( key )->value;
}

template<class K, class V>
void Map<K,V>::transplant( Node* u, Node* v )
{
	if( u->p == 0 ) {
		root = v;
	} else if( u == u->p->l ) {
		u->p->l = v;
	} else {
		u->p->r = v;
	}
	v->p = u->p;
}

template<class K, class V>
void Map<K,V>::rotate_left( Node* x )
{
	assert( x->r != 0 );
	
	Node* y = x->r;
	// left rotation can be done only if right child is not a leaf
	x->r = y->l;

	if( y->l != nil ) {
		y->l->p = x;
	}
	y->p = x->p;

	if( x->p == 0 ) {
		root = y;
	} else if( x == x->p->l ) {
		x->p->l = y;
	} else {
		x->p->r = y;
	}
	y->l = x;
	x->p = y;
}

template<class K, class V>
void Map<K,V>::rotate_right( Node* x )
{
	assert( x->l != nil );
	Node* y = x->l;
	x->l = y->r;

	if( y->r != nil ) {
		y->r->p = x;
	}
	y->p = x->p;

	if( x->p == 0 ) {
		root = y;
	} else if( x == x->p->l ) {
		x->p->l = y;
	} else {
		x->p->r = y;
	}
	y->r = x;
	x->p = y;
}

template<class K, class V>
typename Map<K,V>::Node* Map<K,V>::tree_minimum( Node* x )
{
	while( x->l != nil ) {
		x = x->l;
	}
	return x;
}

template<class K, class V>
void Map<K,V>::add_fix( Node* z )
{
	while( z->p != 0 && z->p->c == RED ) {
		if( z->p == z->p->p->l ) {
			Node* y = z->p->p->r;
			if( y->c == RED ) {
				z->p->c = BLACK;
				y->c = BLACK;
				z->p->p->c = RED;
				z = z->p->p;
			} else {
				if( z == z->p->r ) {
					z = z->p;
					rotate_left( z );
				}
				z->p->c = BLACK;
				z->p->p->c = RED;
				rotate_right( z->p->p );
			} 
		} else {
			Node* y = z->p->p->l;
			if( y->c == RED ) {
				z->p->c = BLACK;
				y->c = BLACK;
				z->p->p->c = RED;
				z = z->p->p;
			} else {
				if( z == z->p->l ) {
					z = z->p;
					rotate_right( z );
				}
				z->p->c = BLACK;
				z->p->p->c = RED;
				rotate_left( z->p->p );
			}
		}
	} root->c = BLACK;
}

template<class K, class V>
void Map<K,V>::remove_fix( Node* x )
{
	while( x != root && x->c == BLACK ) {
		if( x == x->p->l ) {
			Node* w = x->p->r;
			if( w->c == RED ) {
				w->c = BLACK;
				x->p->c = RED;
				rotate_left( x->p );
				w = x->p->r;
			}
			if( w->l->c == BLACK && w->r->c == BLACK ) {
				w->c = RED;
				x = x->p;
			} else {
				if( w->r->c == BLACK ) {
					w->l->c = BLACK;
					w->c = RED;
					rotate_right( w );
					w = x->p->r;
				}
				w->c = x->p->c;
				x->p->c = BLACK;
				w->r->c = BLACK;
				rotate_left( x->p );
				x = root;
			}
		} else {
			Node* w = x->p->l;
			if( w->c == RED ) {
				w->c = BLACK;
				x->p->c = RED;
				rotate_right( x->p );
				w = x->p->l;
			}
			if( w->l->c == BLACK && w->r->c == BLACK ) {
				w->c = RED;
				x = x->p;
			} else {
				if( w->l->c == BLACK ) {
					w->r->c = BLACK;
					w->c = RED;
					rotate_left( w );
					w = x->p->l;
				}
				w->c = x->p->c;
				x->p->c = BLACK;
				w->l->c = BLACK;
				rotate_right( x->p );
				x = root;
			}
		}
	}
	x->c = BLACK;
}
