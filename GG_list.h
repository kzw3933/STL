#ifndef __GG_VECTOR_H
#define __GG_VECTOR_H

#include <stddef.h>
#include "GG_config.h"
#include "GG_iterator.h"
#include "GG_allocator.h"
#include "GG_alloc.h"
#include "GG_construct.h"

__GG_BEGIN_NAMESPACE

template <class T>
struct __list_node {
    typedef void* void_pointer;
    void_pointer    prev;
    void_pointer    next;
    T    data;
};

template<class T, class Ref, class Ptr>
struct __list_iterator {
    typedef __list_iterator<T, T&, T*> iterator;
    typedef __list_iterator<T, Ref, Ptr>    self;

    typedef bidirectional_iterator_tag iterator_category;
    typedef T       value_type;
    typedef Ptr     pointer;
    typedef Ref     reference;
    typedef __list_node<T>* link_type;
    typedef size_t  size_type;
    typedef ptrdiff_t   difference_type;

    link_type   node;

    __list_iterator(link_type x): node(x) {}
    __list_iterator()   {}
    __list_iterator(const iterator& iter): node(iter.node) {}

    bool operator==(const self& x) const { return node == x.node; }
    bool operator!=(const self& x) const { return node != x.node; }
    reference operator*() const {return (*node).data; }
    pointer operator->() const { return &(operator*()); }

    self& operator++() {
        node = (link_type) ((*node).next);
        return *this;
    }

    self& operator++(int) {
        self tmp = *this;
        ++ *this;
        return tmp;
    }

    self& operator--() {
        node = (link_type) ((*node).prev);
        return *this;
    }

    self& operator--(int) {
        self tmp = *this;
        -- *this;
        return tmp;
    }
};

template <class T, class Ref, class Ptr>
inline bidirectional_iterator_tag iterator_category(const __list_iterator<T, Ref, Ptr>&) {
    return bidirectional_iterator_tag();
}

template <class T, class Ref, class Ptr>
inline T* value_type(const __list_iterator<T, Ref, Ptr>&) {
    return 0;
}

template <class T, class Ref, class Ptr>
inline ptrdiff_t* distance_type(const __list_iterator<T, Ref, Ptr>&) {
    return 0;
}


template <class T, class Alloc=alloc>
class list {
    protected:
        typedef void*   void_pointer;
        typedef __list_node<T> list_node;
        typedef allocator<list_node, Alloc> list_node_allocator;
    public:
        typedef T               value_type;
        typedef T*              pointer;
        typedef const T*        const_pointer;
        typedef const T         reference;
        typedef const T&        const_reference;
        typedef list_node*      link_type;
        typedef size_t          size_type;
        typedef ptrdiff_t       difference_type;

    public:
        typedef __list_iterator<T, T&, T*> iterator;
        typedef __list_iterator<T, const T&, const T*> const_iterator;

    protected:
        link_type get_node() { return list_node_allocator::allocate(); }
        void put_node(link_type node) { list_node_allocator::deallocate(node); }

        link_type create_node(const T& val) {
            link_type p = get_node();
            construct(&(p->data), val);
            return p;
        }

        void destroy_node(link_type p) {
            destroy(&(p->data));
            put_node(p);
        }

        void empty_initialize() {
            node = get_node();
            node->next = node;
            node->prev = node;
        }

    public:
        list() { empty_initialize(); }



    protected:
        link_type   node;

    public:
        iterator begin() { return (link_type)((*node).next);}
        iterator end() { return node; }
        bool empty() const { return node->next == node; }
        size_type size() const {
            size_type result = 0;
            distance(begin(), end(), result);
            return result;
        }

        reference front() { return *begin(); }
        reference back() { return *(--end()); } 


        iterator insert(iterator pos, const T& val) {
            link_type tmp = create_node(val);
            tmp->next = pos.node;
            tmp->prev = pos.node->prev;
            (link_type(pos.node->prev))->next = tmp;
            pos.node->prev = tmp;
            return tmp;
        }

        void push_front(const T& x) { insert(begin(), x); }
        void push_back(const T& x) { insert(end(), x); }

        iterator erase(iterator pos) {
            link_type next_node = link_type(pos.node->next);
            link_type prev_node = link_type(pos.node->prev);
            prev_node->next = next_node;
            next_node->prev = prev_node;

            destroy_node(pos.node);
            return iterator(next_node);
        }

        // 移除头结点
        void pop_front() { erase(begin()); }

        // 移除尾结点
        void pop_back() {
            iterator tmp = end();
            erase(--tmp);
        }
        void remove(const T& val);
        void clear();

    
};


// 清除所有结点
template<class T,class Alloc>
void list<T, Alloc>::clear() {
    link_type cur = (link_type) node->next;
    while(cur != node) {
        link_type tmp = cur;
        cur = (link_type) cur->next;
        destroy_node(tmp);
    }

    node->next = node;
    node->prev = node;
}

// 将值为val的所有元素移除
template<class T,class Alloc>
void list<T, Alloc>::remove(const T& val) {
    iterator first = bein();
    iterator last = end();
    while (first != last) {
        iterator next = first;
        ++next;
        if(*first == val) erase(first);
        first = next;
    }
}



__GG_END_NAMESPACE
#endif