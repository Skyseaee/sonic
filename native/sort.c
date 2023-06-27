#include <stdio.h>

#include "native.h"

typedef struct MapPair{
    GoString  key;
    void*     value;
} MapPair;

// swap elems in MapPair
void swap(MapPair* lhs, MapPair* rhs) {
    MapPair temp;
    temp = *lhs;
    *lhs = *rhs;
    *rhs = temp;
}

static always_inline size_t min(size_t x, size_t y) {
    return x > y ? y : x;
}

static always_inline bool less(MapPair lhs, MapPair rhs) {
    size_t index = 0;
    size_t size = min(lhs.key.len, rhs.key.len);
    while(index < size && *lhs.key.buf == *rhs.key.buf) {
        lhs.key.buf++, rhs.key.buf++;
        index++;
    }

    if(index == size) {
        if(lhs.key.len != rhs.key.len && index == lhs.key.len) {
            return true;
        } else {
            return false;
        }
    } 

    return *(unsigned char *)lhs.key.buf - *(unsigned char *)rhs.key.buf < 0;
}

void heap_adjust(MapPair* kvs, size_t root, size_t end) {
    for(size_t child=2*root + 1; child<=end; child = child*2 + 1) {
        if(child < end && less(kvs[child], kvs[child+1])) {
            child++;
        }

        if(!less(kvs[root], kvs[child])) {
            break;
        } else {
            swap(&kvs[root], &kvs[child]);
            root = child;
        }
    }
}

void heap_sort(MapPair* kvs, size_t n) {
    if(n <= 1) {
        return;
    }

    for(int i=(n-1)/2; i>=0; i--) {
        heap_adjust(kvs, i, n-1);
    }   

    for(int i=n-1; i>0; i--) {
        swap(&kvs[i], &kvs[0]);
        heap_adjust(kvs, 0, i-1);
    }
}

void insert_sort(MapPair* kvs, size_t n) {
    if(n <= 1) {
        return;
    }

    for(size_t i=1; i<n; i++) {
        MapPair temp = kvs[i];
        int j=i-1;
        while(j>=0 && less(temp, kvs[j])) {
            kvs[j + 1] = kvs[j];
            j--;
        }
        kvs[j+1] = temp;
    }

}