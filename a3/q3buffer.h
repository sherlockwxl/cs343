//
// Created by xilingwu on 2/23/19.
//

#ifndef A3_Q3MAIN_H
#define A3_Q3MAIN_H
#include <queue>
#include <iostream>
#include "q3buffer.h"
using namespace std;



template<typename T> class BoundedBuffer {

    uOwnerLock ownerLock;                                  // owner lock
    uCondLock emptyLock;                                   // lock to acquire when buffer is empty(for consumer)
    uCondLock fullLock;                                    // lock to acquire when buffer is full(for producer)
    const unsigned int maxSize;                            // buffer size limit
    queue<T> buffer;                                       // buffer store queue(for easy pop and push operation)

#if defined(NOBUSY)
    bool avail = true;                                     // conditional variable for no busy waiting
    uCondLock bargeLock;                                   // lock for barging
#endif

public:
    BoundedBuffer( const unsigned int size = 10 );
    void insert( T elem );
    T remove();



};


#endif //A3_Q3MAIN_H
