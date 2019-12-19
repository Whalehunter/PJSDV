#ifndef GUI_HPP
#define GUI_HPP

class Gui: public Device
{
    public:
        Gui(int, Appartement*);
        ~Gui();

        void operator()();
};

#endif
