#ifndef BED_HPP
#define BED_HPP

#include "../Device.hpp"

class Bed: public Device
{
    public:
        Bed(int, Appartement*);
        ~Bed();

        void operator()();
		void updateWaardes();
		void activeerBewegingssensor();
		void deactiveerBewegingssensor();
		void toggleLed();
        int getStatus();
	private:
		int ledStatus = 0;
		int drukknop;
		int druksensor;
};

#endif
