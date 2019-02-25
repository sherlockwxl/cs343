//
// Created by xilingwu on 2/23/19.
//

#include "q3buffer.h"
#include "q3pro_con.h"
#include "MPRNG.h"
using namespace std;




MPRNG *mprng;


//BoundedBuffer implementation for no busy waiting
#if defined(NOBUSY)
template<typename T>
BoundedBuffer<T>::BoundedBuffer(const unsigned int size):
        maxSize(size){};


template<typename T>
void BoundedBuffer<T>::insert(T elem) {

    ownerLock.acquire();           //acquire ownerlock

    if(!avail){                    //if conditional variable is not availiable, acquire the bargelock
        bargeLock.wait(ownerLock);
        if(bargeLock.empty()){     //if barge lock is empty, do a conditional reset
            avail = true;
        }
    }

    if (buffer.size() == maxSize){ // check if current insert need to wait for full lock

        if(bargeLock.empty()){     // if wait will be performed, reset the conditional variable first
            avail = true;          // if barge lock is empty, do a conditional reset
        }else{
            bargeLock.signal();    // otherwise signal
        }
        fullLock.wait(ownerLock);  // wait for remove done
    }

    buffer.push(elem);             // push the element into the buffer


    if(!emptyLock.empty()){        // check if anyone is waiting for the empty lock if yes, means still barging
        avail = false;
    }else{
        if(!bargeLock.empty()){    // check if anyone is waiting for the barge lock if yes, means still barging
            avail = false;
        }else{
            avail = true;          // if no one is waiting, allow barging
        }
    }

    emptyLock.signal();            // release the empty lock, since we already push at least one elemnt
    bargeLock.signal();            // release the barge lock, since we have finished buffer insertation
    ownerLock.release();           // release the owner lock.
}


template<typename T>
T BoundedBuffer<T>::remove() {
    ownerLock.acquire();           //acquire ownerlock

    if(!avail){                    //if conditional variable is not availiable, acquire the bargelock
        bargeLock.wait(ownerLock);
        if(bargeLock.empty()){     //if barge lock is empty, do a conditional reset
            avail = true;
        }
    }

    if (buffer.size() == 0){       // check if current remove need to wait for empty lock

        if(bargeLock.empty()){     // if wait will be performed, reset the conditional variable first
            avail = true;          // if barge lock is empty, do a conditional reset
        }else{
            bargeLock.signal();    // otherwise signal
        }
        emptyLock.wait(ownerLock); // wait for insert done
    }

    T res = buffer.front();        // get the first element in the buffer
    buffer.pop();                  // remove the first element from the buffer

    if(!fullLock.empty()){         // check if anyone is waiting for the full lock if yes, means still barging
        avail = false;
    }else{
        if(!bargeLock.empty()){    // check if anyone is waiting for the barge lock if yes, means still barging
            avail = false;
        }else{
            avail = true;          // if no one is waiting, allow barging
        }
    }

    fullLock.signal();             // release the full lock, since we already removed at least one elemnt
    bargeLock.signal();            // release the barge lock, since we have finished buffer insertation
    ownerLock.release();           // release the owner lock.
    return res;
}
#endif


//BoundedBuffer implementation for busy waiting
#if defined(BUSY)
template<typename T>
BoundedBuffer<T>::BoundedBuffer(const unsigned int size):
        maxSize(size){};


template<typename T>
void BoundedBuffer<T>::insert(T elem) {
    ownerLock.acquire();              // acquire ownerlock
    while (buffer.size() == maxSize){ // busy waiting for remove done when buffer size at max size
        fullLock.wait(ownerLock);     // wait for remove done
    }
    buffer.push(elem);                // insert element to the back of buffer
    emptyLock.signal();               // release the empty lock, since we already push at least one elemnt
    ownerLock.release();              // release the owner lock.
}

template<typename T>
T BoundedBuffer<T>::remove() {
    ownerLock.acquire();           // acquire ownerlock
    while (buffer.size() == 0){    // busy waiting for insert done when no element in the buffer
        emptyLock.wait(ownerLock); // wait for insert done
    }
    T res = buffer.front();        // get the first element in the buffer
    buffer.pop();                  // remove the first element from the buffer
    fullLock.signal();             // release the full lock, since we already removed at least one elemnt
    ownerLock.release();           // release the owner lock.

    return res;
}
#endif

//implementation of producer and consumer
Producer::Producer( BoundedBuffer<int> &buffer, const int Produce, const int Delay ):
    buffer(buffer), Produce(Produce), Delay(Delay){};


void Producer::main(){
    for(int i = 1; i <= Produce; i++){
        yield((*mprng)(Delay));
        buffer.insert(i);
    }// for
}

Consumer::Consumer( BoundedBuffer<int> &buffer, const int Delay, const int Sentinel, int &sum ):
        buffer(buffer), Delay(Delay), Sentinel(Sentinel), sum(sum){};


void Consumer::main(){
    for(;;){
        yield((*mprng)(Delay));
        int res = buffer.remove();
        if ( res == Sentinel ){
            break;
        } //if
        sum =  sum + res;
    }// for
}




//program main function
int main( int argc, char * argv[] ) {


    //set up default and ini variables
    int cons = 5;
    int prods = 3;
    int produce = 10;
    int bufferSize = 10;
    int delay = 8;

    //create random number generator
    mprng = new MPRNG();

    try {                                        // process command-line arguments
        switch ( argc ) {
            case 6:
                delay = stoi(argv[5]); if ( delay <= 0 ) throw 1;

            case 5:
                bufferSize = stoi( argv[4] ); if ( bufferSize <= 0 ) throw 1;

            case 4:
                produce = stoi( argv[3] ); if ( produce <= 0) throw 1;
            case 3:
                prods = stoi( argv[2] ); if ( prods <= 0 ) throw 1;
            case 2:
                cons = stoi( argv[1] ); if ( cons < 0 ) throw 1;
            case 1: break;                                // use all defaults
            default: throw 1;
        } // switch
    } catch( ... ) {
        cerr << "Usage: " << argv[0] << "[ Cons (> 0) [ Prods (> 0) [ Produce (> 0) [ BufferSize (> 0)"
                                        "[ Delay (> 0) ] ] ] ] ]" << endl;
        exit( EXIT_FAILURE );
    } // try

    if( argc != 6 ){                             // if delay is not specified let delay be cons + prods
        delay = cons + prods;
    }

#ifdef __U_MULTI__
    uProcessor p[3] __attribute__(( unused ));   // create 3 kernel thread for a total of 4
#endif // __U_MULTI__


    //set up done;

    BoundedBuffer<int> buffer(bufferSize);       // create buffer of given size
    Producer *producerList[prods];               // create producer list
    Consumer *consumerList[cons];                // create consumer list
    int res[cons];                               // create the result list

    for(int i = 0 ; i < cons; i++){              // ini the result list to be zero
        res[i] = 0;
    }

    for(int i = 0 ; i < prods; i++){             // initialize producer list
        producerList[i] = new Producer(buffer, produce, delay);
    }

    for(int i = 0 ; i < cons; i++){              // initialize consumer list
        consumerList[i] = new Consumer(buffer, delay, SENTINEL, res[i]);
    }


    for(int i = 0 ; i < prods; i++){             // remove producer
        delete producerList[i];
    }

    for(int i = 0 ; i < cons; i++){              // insert SENTINEL to stop the consumers
        buffer.insert(SENTINEL);
    }

    for(int i = 0 ; i < cons; i++){              // remove consumer
        delete consumerList[i];
    }

    int total = 0;
    for(int i = 0; i < cons; i++){               // calculate the result
        total += res[i];
    }

    cout<<"total: "<<total<<endl;
    delete mprng;


}