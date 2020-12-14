#include "barcode.h"

barcode::barcode(const char *port, int baudrate) {
	if ((fd = serialOpen (port, baudrate)) < 0) {
		log("[ERROR] (barcode) Unable to open serial device");
		return;
	}

	if (wiringPiSetup () == -1) {
		log("[ERROR] (barcode) Unable to start wiringPi");
		return;
	}
};

char* barcode::getcode() {
	std::string str;
	char *ret; 

	if (serialDataAvail (fd)) {
		while (serialDataAvail (fd)) {
			str += serialGetchar(fd);
		}

		ret = new char[str.length()];
		for (int i = 0; i < str.length(); ++i) {
			ret[i] = str[i];
		}
		return ret;
	} else {
		throw false;
	}
};

bool barcode::trigger() {
    serialPuts(fd, this->trigreq);
    return true;
};