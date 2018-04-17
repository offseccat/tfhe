#include <stdlib.h>
#include "TFHEAllocator.h"
#include "allocator.h"

void *AllocatorImpl<TFHE_ALLOCATOR>::allocate(size_t alignment, size_t byte_size) {
    if (beginAddress == 0ul) {
        //malloc mode
        return aligned_alloc(alignment, byte_size); //TODO: fix strange bug!!!
    } else {
        //stack mode
        size_t newBegin = ceilalign(beginAddress, alignment);
        beginAddress = newBegin + byte_size;
        return (void *) newBegin;
    }
}

void AllocatorImpl<TFHE_ALLOCATOR>::deallocate(void *ptr) {
    if (beginAddress == 0ul) {
        //malloc mode
        free(ptr);
    } else {
        //stack mode
        //nothing
    }
}

AllocatorImpl<TFHE_ALLOCATOR>::~AllocatorImpl() {
    if (father != nullptr) {
        //first stack mode
        free((void*)father->beginAddress);
        father->beginAddress=0ul;
    } else {
        //malloc mode
        //nothing
    }
}

AllocatorImpl<TFHE_ALLOCATOR> AllocatorImpl<TFHE_ALLOCATOR>::createStackChildAllocator(const size_t expected_size) {
    if (beginAddress == 0ul) {
        //malloc mode
        beginAddress = size_t(aligned_alloc(1, expected_size));
        return AllocatorImpl(this, beginAddress);
    } else {
        //stack mode
        return AllocatorImpl(nullptr, beginAddress);
    }
}

AllocatorImpl<TFHE_ALLOCATOR>::AllocatorImpl(AllocatorImpl<TFHE_ALLOCATOR> *const father, const size_t beginAddr) :
        father(father), beginAddress(beginAddr) {
}

AllocatorImpl<TFHE_ALLOCATOR>::AllocatorImpl(AllocatorImpl<TFHE_ALLOCATOR> &&origin) :
        father(origin.father), beginAddress(origin.beginAddress) {
    origin.father = nullptr;
    origin.beginAddress = 0;
}
