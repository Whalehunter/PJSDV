#include <iostream>
#include <cstring> // strcpy
#include <unistd.h>
#include "communicator.h"

using namespace std;

int main()
{
	Communicator a;
	char x[64], target[] = "wemos", data[64];
	int potValue;

	while(1)
	{
		strcpy(data, "g");
		strcpy(x, a.retrieveValue(target, data));

		potValue = atoi(x);

//    cout << potValue << endl;

		if(potValue >= 400) {
			strcpy(data, "h");
			a.retrieveValue(target, data);
		}
		else {
			strcpy(data, "j");
			a.retrieveValue(target, data);
		}
    usleep(100000);

	}

	return 0;
}
