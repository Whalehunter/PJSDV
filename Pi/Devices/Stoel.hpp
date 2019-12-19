#ifndef STOEL_HPP
#define STOEL_HPP

class Stoel: public Device
{
    public:
        Stoel(int, Appartement*);
        ~Stoel();

        void operator()();
};

#endif
