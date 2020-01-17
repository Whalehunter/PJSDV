/*
 * Koelkast.hpp
 *
 *  Created on: Jan 8, 2020
 *      Author: Fabian
 */

#ifndef DEVICES_KOELKAST_HPP_
#define DEVICES_KOELKAST_HPP_

#include "../Device.hpp"
#include <iostream>
#include <iomanip>


class Koelkast: public Device{

	private:
		std::clock_t openTimer;

		int koelkastDeur{};
		int fan{};
		int timerAanUit{};
		float NTC1{};
		float NTC2{};
		int koelAlarm{};
		float tempOut{};
		float tempIn{};
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
		float calculateCelsius(float i);
};


#endif /* DEVICES_KOELKAST_HPP_ */
