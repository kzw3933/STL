#ifndef __GG_TREE_H
#define __GG_TREE_H

#include <stddef.h>
#include "GG_config.h"
#include "GG_iterator.h"
#include "GG_alloc.h"
#include "GG_allocator.h"
#include "GG_construct.h"
#include "GG_pair.h"

__GG_BEGIN_NAMESPACE


typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;
const __rb_tree_color_type __rb_tree_black = true;

struct __rb_tree_node_base {
    typedef __rb_tree_color_type color_type;
    typedef __rb_tree_node_base* base_ptr;

    color_type color;
    base_ptr parent;
    base_ptr left;
    base_ptr right;

    static base_ptr minimum(base_ptr x) {
        while (x->left != 0) 
            x = x->left;
        return x;
    }

    static base_ptr maximum(base_ptr x) {
        while (x->right != 0) 
            x = x->right;
        return x;
    }
};


template <class Value>
struct __rb_tree_node : public __rb_tree_node_base {
    typedef __rb_tree_node<Value*> link_type;
    Value value_field;
};

struct __rb_tree_base_iterator {
    typedef __rb_tree_node_base::base_ptr base_ptr;
    typedef bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t   difference_type;

    base_ptr node;

    void increment() {
        if (node->right != 0) {
            node = node->right;
            while(node->left != 0) 
                node = node->left;
        }
        else {
            base_ptr y = node->parent;
            while(node == y->right) {
                node = y;
                y = y->parent;
            }
            if (node->right != y)       // 应对特殊情况: 寻找根节点的下一个节点,但此时根节点无右子节点
                node = y;
        }
    }

    void decrement() {

    }

};

template <class Value, class Ref, class Ptr>
struct __rb_tree_iterator: public __rb_tree_base_iterator {
    typedef Value   value_type;
    typedef Ref     reference;
    typedef Ptr     pointer;
    typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
    typedef __rb_tree_iterator<const Value, const Value&, const Value*> const_iterator;
    typedef __rb_tree_iterator<Value, Ref, Ptr> self;
    typedef __rb_tree_node<Value>* link_type;

    __rb_tree_iterator() {}
    __rb_tree_iterator(link_type x) { node = x; }
    __rb_tree_iterator(const iterator& it) { node = it.node; }
    reference operator*() const {
        return link_type(node)->value_field;
    }
    self& operator++() {
        increment();
        return *this;
    }
    self operator++(int) {
        self tmp = *this;
        increment();
        return tmp;
    }

    self& operator--() {
        decrement();
        return *this;
    }

    self operator--(int) {
        self tmp = *this;
        decrement();
        return tmp;
    }
};


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc=alloc>
class rb_tree {

    public:
        typedef Key     key_type;
        typedef Value   value_type;
        typedef value_type* pointer;
        typedef const value_type*   const_pointer;
        typedef value_type& reference;
        typedef const value_type&   const_reference;
        typedef const rb_tree_node* link_type;
        typedef size_t  size_type;
        typedef ptrdiff_t   difference_type;

        typedef __rb_tree_iterator<value_type, reference, pointer> iterator;

    protected:
        typedef void*   void_pointer;
        typedef __rb_tree_node_base*    base_ptr;
        typedef __rb_tree_node<Value>   rb_tree_node;
        typedef allocator<rb_tree_node, Alloc>  rb_tree_node_allocator;
        typedef __rb_tree_color_type    color_type;

    public:
        rb_tree(const Compare& comp = Compare()): node_count(0), key_compare(comp) { init(); }
        

        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& operate=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x);

        ~rb_tree() {
            clear();
            put_node(header);
        }

    public:
        Compare key_comp() const { return key_compare; }
        iterator begin() { return leftmost(); }
        iterator end() { return header; }
        bool empty() const { return node_count == 0; }
        size_type size() const { return node_count; }
        size_type max_size() const { return size_type(-1); }

        //!TODO: pair的实现
        pair<iterator, bool>insert_unique(const value_type& x);
        iterator insert_equal(const value_type& x);

        iterator find(const Key& x);

    

    protected:
        link_type   get_node() {
            return rb_tree_node_allocator::allocate();
        }
        void put_node(link_type p) {
            rb_tree_node_allocator::deallocate(p);
        }

        link_type   create_node(const value_type& x) {
            link_type   tmp = get_node();
            construct(&tmp->value_field, x);
            return tmp;
        }

        link_type   clone_node(link_type x) {
            link_type tmp = create_node(x->value_field);
            tmp->color = x->color;
            tmp->left = 0;
            tmp->right = 0;
            return tmp;
        } 

        void destroy_node(link_type p) {
            destroy(&p->value_field);
            put_node(p);
        }
        

        link_type&  root() const {
            return (link_type&) header->parent;
        }

        link_type& leftmost() const {
            return (link_type&)header->left;
        }

        link_type& rightmost() const {
            return (link_type&)header->right;
        }

        static link_type& left(link_type x) { return x->left; }
        static link_type& right(link_type x) { return x->right; }
        static link_type& parent(link_type x) { return x->parent; }
        static reference value(link_type x) { return x->value_field; }
        static const Key& key(link_type x) { return KeyOfValue()(value(x)); } 
        static color_type& color(link_type x) { return (color_type&)(x->color); }

        static link_type& left(base_ptr x) { 
            return (link_type&)(x->left); 
        }
        static link_type& right(base_ptr x) { 
            return (link_type&)(x->right); 
        }
        static link_type& parent(base_ptr x) { 
            return (link_type&)(x->parent); 
        }
        static reference value(base_ptr x) { 
            return ((link_type)x)->value_field; 
        }
        static const Key& key(base_ptr x) { 
            return KeyOfValue()(value(link_type(x))); 
        } 
        static color_type& color(base_ptr x) { 
            return (color_type&)(link_type(x)->color); 
        }

        static link_type minimum(link_type x) {
            return (link_type) __rb_tree_node_base::minimum(x);
        }

        static link_type maximum(link_type x) {
            return (link_type) __rb_tree_node_base::maximum(x);
        }  
    

    private:
        iterator __insert(base_ptr x, base_ptr y, const value_type& v);
        link_type __copy(link_type x, link_type p);
        void __erase(link_type x);
        void init() {
            header = get_node();
            color(header) = __rb_tree_red;
            root() = 0;
            leftmost() = header;
            rightmost() = header;  
        }

        inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
            __rb_tree_node_base* y = x->right;
            x->right = y->left;
            if (y->left != 0)
                y->left->parent = x; 
            y->parent = x->parent;
            if (x == root)
                root = y;
            else if (x == x->parent->left) 
                x->parent->left = y;
            else 
                x->parent->right = y;
            y->left = x;
            x->parent = y;
        }

        inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
            __rb_tree_node_base* y = x->left;
            x->left = y->right;
            if (y->right != 0)
                y->right->parent = x; 
            y->parent = x->parent;
            if (x == root)
                root = y;
            else if (x == x->parent->right) 
                x->parent->right = y;
            else 
                x->parent->left = y;
            y->right = x;
            x->parent = y;
        }

        inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
            x->color = __rb_tree_red;
            while(x != root && x->parent->color == __rb_tree_red) {
                if (x->parent == x->parent->parent->left) {
                    __rb_tree_node_base* y = x->parent->parent->right;
                    if(y && y->color == __rb_tree_red) {
                        x->parent->color = __rb_tree_black;
                        y->color = __rb_tree_black;
                        x->parent->parent->color = __rb_tree_red;
                        x = x->parent->parent;
                    }
                    else {
                        if(x == x->parent->right) {
                            x = x->parent;
                            __rb_tree_rotate_left(x, root);
                        }
                        x->parent->color = __rb_tree_black;
                        x->parent->parent->color = __rb_tree_red;
                        __rb_tree_rotate_right(x->parent->parent, root);
                    }
                }
                else {
                    __rb_tree_node_base* y = x->parent->parent->left;
                    if(y && y->color == __rb_tree_red) {
                        x->parent->color = __rb_tree_black;
                        y->color = __rb_tree_black;
                        x->parent->parent->color = __rb_tree_red;
                        x = x->parent->parent;
                    }
                    else {
                        if(x == x->parent->left) {
                            x = x->parent;
                            __rb_tree_rotate_right(x, root);
                        }
                        x->parent->color = __rb_tree_black;
                        x->parent->parent->color = __rb_tree_red;
                        __rb_tree_rotate_left(x->parent->parent, root);
                    }
                }
            }
            root->color = __rb_tree_black;
        }

    protected:
        size_type node_count;   // 追踪记录树的大小(节点数量)
        link_type header;       // 实现上的一个技巧
        Compare key_compare; 

};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const Value& v) {
    link_type y = header;
    link_type x = root();

    while( x != 0) {
        y = x;
        x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
    }
    return __insert(x,y,v);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const Value& x) {
    link_type y = header;
    link_type x = root();
    bool comp = true;
    while(x != 0) {
        y = x;
        comp = key_compare(KeyOfValue()(v), key(x));
        x = comp ? left(x) : right(x);
    }

    iterator j = iterator(y);
    if (comp)
        if (j == begin())
            return pair<iterator, bool>(__insert(x, y, v), true);
        else    
            --j;
    if(key_compare(key(j.node), KeyOfValue()(v)))
        return pair<iterator, bool>(__insert(x, y, v), true);

    return pair<iterator, bool>(j ,false);
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr x_, base_ptr y_, const Value& v) {
    link_type x = (link_type) x_;
    link_type y = (link_type) y_;

    link_type z;

    if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y))) {
        z = create_node(v);
        left(y) = z;
        if(y == header) {
            root() = z;
            rightmost() = z;
        }
        else if (y == leftmost())
            leftmost() = z;
    }
    else {
        z = create_node(v);
        right(y) = z;
        if(y == rightmost())
            rightmost() = z;
    }

    parent(z) = y;
    left(z) = 0;
    right(z) = 0;

    __rb_tree_rebalance(z, header->parent);
    ++node_count;
    return iterator(z);

}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) {
    link_type y = header;
    link_type x = root();

    while(x != 0) {
        if (!key_compare(key(x), k))
            y = x, x= left(x);
        else    
            x = right(x);
    }
    iterator j = iterator(y);

    return (j == end() || key_compare(k, key(j.node))) ? end() : j;  
}


__GG_END_NAMESPACE
#endif