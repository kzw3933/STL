#ifndef __GG_ALLOC_H
#define __GG_ALLOC_H

#include <cstddef>
#include <GG_config.h>

__GG_BEGIN_NAMESPACE

enum {__ALIGN = 8};                         // 小型区块上调边界
enum {__MAX_BYTES = 128};                   // 小型区块的上限
enum {__NFREELISTS = __MAX_BYTES/__ALIGN};  // free-lists个数
enum {__NOBJS = 20};                        // refill时从内存池中索取的新节点数缺省值

class alloc {
    private:
        // 将bytes上调至 __ALIGN 的倍数
        static size_t ROUND_UP(size_t bytes) {
            return (((bytes)+__ALIGN-1)& ~(__ALIGN -1));  
        }
    private:
        union obj {
            union obj* free_list_link;
            char client_data[1];
        };
    private:
        static obj* free_lists[__NFREELISTS];
        static size_t FREELIST_INDEX(size_t bytes){
            return (((bytes)+__ALIGN-1)/__ALIGN -1);
        }
        static void* refill(size_t n);
        static char* chunk_alloc(size_t size,int& nobjs);

        // chunk allocation state
        static char* start_free;
        static char* end_free;
        static size_t heap_size;

    public:
        static void* allocate(size_t bytes);
        static void deallocate(void* ptr,size_t bytes);
        static void* reallocate(void* ptr, size_t old_sz, size_t new_sz);
};

__GG_END_NAMESPACE

#endif