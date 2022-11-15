#ifndef __GG_ALLOCATOR_H
#define __GG_ALLOCATOR_H

#include <cstddef>
#include <GG_config.h>
#include <GG_alloc.h>

__GG_BEGIN_NAMESPACE

template <class T, class Alloc>
class allocator {
    public:
        static T* allocaate(size_t n) {
            return 0 == n ? 0: (T*) Alloc:allocaate(n*sizeof(T));
        }
        static T* allocaate(void) {
            return (T*) Alloc:allocaate(sizeof(T));
        }
        static void deallocate(T* p,size_t n){
            if(0 != n)  Alloc::deallocate(p, n*sizeof(T));
        }
        static void deallocate(T* p){
            Alloc::deallocate(p,sizeof(T));
        }
};

__GG_END_NAMESPACE
#endif