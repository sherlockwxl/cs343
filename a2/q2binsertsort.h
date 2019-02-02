template<typename T> _Coroutine Binsertsort {
    T value;                        // communication: value being passed down/up the tree
    void main();                    // YOU WRITE THIS ROUTINE
    int insertdone = 0;
    int set = 0;
  public:
    _Event Sentinel {};
    void sort( T value ) {          // value to be sorted
        Binsertsort::value = value;
        resume();
    }
    T retrieve() {                  // retrieve sorted value
        resume();
        return value;
    }
};

