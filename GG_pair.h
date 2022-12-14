#ifndef __GG_PAIR_H
#define __GG_PAIR_H

#include "GG_config.h"

__GG_BEGIN_NAMESPACE

template <class T1, class T2>
class pair {
    typedef T1 first_type;
    typedef T2 second_type;

    T1 first;
    T2 second;

    pair(): first(T1()), second(T2()) {}
    pair(const T1& a, const T2& b): first(a), second(b) {}
};

template <class T1, class T2>
bool operator==(const pair<T1, T2>& x, const pair<T1, T2>& y) {
    return x.first == y.first && x.second == y.second;
}

template <class T1, class T2>
bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y) {
    return x.first < y.first || (!(y.first < x.first) && x.second < y.second); 
}

template <class T1, class T2>
inline pair<T1, T2> make_pair(const T1& x, const T2& y) {
    return pair<T1, T2>(x, y); 
}

__GG_END_NAMESPACE

#endif