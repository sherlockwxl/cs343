//
// Created by xilingwu on 2/23/19.
//

#ifndef A3_PRO_CON_H
#define A3_PRO_CON_H


_Task Producer {
        BoundedBuffer<int> &buffer;
        const int Produce;
        const int Delay;
        void main();

    public:
        Producer( BoundedBuffer<int> &buffer, const int Produce, const int Delay );
};

_Task Consumer {
        BoundedBuffer<int> &buffer;
        const int Delay;
        const int Sentinel;
        int &sum;
        void main();
    public:
        Consumer( BoundedBuffer<int> &buffer, const int Delay, const int Sentinel, int &sum );
};

#endif //A3_PRO_CON_H
