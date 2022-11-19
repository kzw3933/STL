#ifndef __GG_ALGOBASE_H
#define __GG_ALGOBASE_H

#include <string.h>
#include "GG_iterator.h"
#include "GG_config.h"
__GG_BEGIN_NAMESPACE

/*
** max
**
*/
template <class T>
inline const T& max(const T& a, const T& b) {
    return a < b ? b : a;
}


/*
** min
**
*/

template <class T>
inline const T& min(const T& a, const T& b) {
    return b < a ? b : a;
}


/*
** fill
**
*/

template <class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T& val){
    for(; first != last; ++first)
        *first = val;
};

template <class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& val){
    for(;n>0;--n,++first) 
        *first = val;
    return first;
};


/*
** equal
**
*/

template <class InputIterator1, class InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
    for(; first1 != last1; ++first1,++first2) {
        if(*first1 != *first2)
            return false;
    }
    return true;
}

/*
** copy
**
*/

template <class InputIterator, class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
    for(; first != last; ++result,++first) 
        *result = *first;
    return result;
}

template <class RandomAccessIterator, class OutputIterator, class Distance>
inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*) {
    for(Distance n=last - first; n>0; --n,++result,++first)
        *result = *first;
    return result;
}

template <class RandomIterator, class OutputIterator>
inline OutputIterator __copy(RandomIterator first, RandomIterator last, OutputIterator result, random_access_iterator_tag) {
    return __copy_d(first, last, result, distance_type(first));
}

template <class InputIterator, class OutputIterator>
struct __copy_dispatch {
    OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
        return __copy(first, last, result, iterator_category(first));
    }
};

template <class T>
inline T* __copy_t(const T* first, const T* last, T* result, __true_type) {
    memmove(result, first, sizeof(T)*(last-first));
    return result + (last-first);
}

template <class T>
inline T* __copy_t(const T* first, const T* last, T* result, __false_type) {
    return __copy_d(first, last, result, (ptrdiff_t*)0);
}

template <class T>
struct __copy_dispatch<T*, T*>{
    T* operator()(T* first, T* last, T* result) {
        typedef typename __type_traits<T>::has_trival_assignment_operator t;
        return __copy_t(first, last, result, t());
    }
};

template <class T>
struct __copy_dispatch<const T*, T*>{
    T* operator()(const T* first, const T* last, T* result) {
        typedef typename __type_traits<T>::has_trival_assignment_operator t;
        return __copy_t(first, last, result, t());
    }
};

template <class InputIterator, class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
    return __copy_dispatch<InputIterator,OutputIterator>()(first, last, result);
}

inline char* copy(const char* first, const char* last, char* result) {
    memmove(result, first, last - first);
    return result + (last-first);
}

inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
    memmove(result, first, sizeof(wchar_t) * (last - first));
    return result + (last-first);
}

/*
** copy_backward
**
*/

template <class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
    while (first != last) 
        *--result = *--last;
    return result;
}


template <class BidirectionalIterator1, class BidirectionalIterator2>
struct __copy_backward_dispatch {
    BidirectionalIterator2 operator()(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
        return __copy_backward(first, last, result);
    }
};

template <class T>
inline T* __copy_backward_t(const T* first, const T* last, T* result,__true_type) {
    const ptrdiff_t N = last - first;
    memmove(result - N, first,sizeof(T) * N);
    return result - N;
} 

template <class T>
inline T* __copy_backward_t(const T* first, const T* last, T* result,__false_type) {
    __copy_backward(first, last, result);
} 


template <class T>
struct __copy_backward_dispatch<T*, T*> {
    T* operator()(T* first, T* last, T* result) {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;  
        return __copy_backward(first, last, result, t());
    }
};

template <class T>
struct __copy_backward_dispatch<const T*, const T*> {
    T* operator()(const T* first,const T* last, T* result) {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;  
        return __copy_backward(first, last, result, t());
    }
};

template <class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
    return __copy_backward_dispatch<BidirectionalIterator1,BidirectionalIterator2>()(first, last, result);
}


__GG_END_NAMESPACE
#endif