#ifndef automaticSignal
#define automaticSignal

#define AUTOMATIC_SIGNAL uCondition groupFormed

#define WAITUNTIL( pred, before, after ) \
    if(!pred){\
        before;\
        groupFormed.wait();\
        if(!groupFormed.empty()){groupFormed.signal();}\
        after;\
    }
#define RETURN( expr ) \
    while(!groupFormed.empty()){groupFormed.signal();} return expr;\




#endif