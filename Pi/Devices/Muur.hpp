#ifndef MUUR_HPP
#define MUUR_HPP

#include "../Device.hpp"

class Muur: public Device
{
    public:
        Muur(int, Appartement*);
        ~Muur();

        void operator()();
		void updateWaardes();
		void RGBsterkte(potmeter);
		void LCDdimmen();
		void LCDdoorlaten();
		void ledBinnenAan();
		void ledBinnenUit();
        int getStatus();
	private:
		int lichtsterkte;
		int potmeter;
};
#endif