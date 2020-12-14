#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#include "../log/log.h"

class barcode {
private:
	int fd;
	const char trigreq[3] = {0x16, 0x54, 0x0d};

public:
	barcode(const char *port, int baudrate);

	char* getcode();
	bool trigger();
	
	~barcode();
};