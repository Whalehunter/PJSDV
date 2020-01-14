/*
 * Koelkast.hpp
 *
 *  Created on: Jan 8, 2020
 *      Author: Fabian
 */

#ifndef DEVICES_KOELKAST_HPP_
#define DEVICES_KOELKAST_HPP_

#include "../Device.hpp"
#include "../nlohmann/json.hpp"
#include <ctime>


class Koelkast: public Device{
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
	private:
		std::clock_t timer;

		int koelkastDeur{};
		int fan{};
		int timerAanUit{};
		int NTC1{};
		int NTC2{};
		int koelAlarm{};

};


#endif /* DEVICES_KOELKAST_HPP_ */
