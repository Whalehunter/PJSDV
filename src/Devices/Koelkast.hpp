/*
 * Koelkast.hpp
 *
 *  Created on: Jan 8, 2020
 *      Author: Fabian
 */

#ifndef DEVICES_KOELKAST_HPP_
#define DEVICES_KOELKAST_HPP_

#include "../Device.hpp"


class Koelkast: public Device{

	private:
		std::clock_t openTimer;

		int koelkastDeur{};
		int fan{};
		int timerAanUit{};
		int NTC1{};
		int NTC2{};
		int koelAlarm{};
	public:
		Koelkast(int, Appartement*);
		~Koelkast();
		nlohmann::json getStatus();
		void operator()();
		void disableKoelAlarm();
		void fanAan();
		void fanUit();
		void peltierAan();
		void peltierUit();
};


#endif /* DEVICES_KOELKAST_HPP_ */
