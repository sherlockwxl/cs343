#ifndef automaticSignal
#define automaticSignal

#define AUTOMATIC_SIGNAL uCondition groupFormed

#define WAITUNTIL( pred, before, after ) \
    if(!pred){\
        before;\
        groupFormed.wait();\                                // wait for the ucondition
        if(!groupFormed.empty()){groupFormed.signal();}\    // wake up all waiting thread
        after;\
    }

#define RETURN( expr ) \
    while(!groupFormed.empty()){groupFormed.signal();} return expr;\ // signal all and return




#endif