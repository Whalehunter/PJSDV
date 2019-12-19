#ifndef BED_HPP
#define BED_HPP

class Bed: public Device
{
    public:
        Bed(int, Appartement*);
        ~Bed();

        void operator()();
};

#endif
