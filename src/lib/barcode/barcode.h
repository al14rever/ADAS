#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#include "../log/log.h"

class barcode {
private:
	int fd;

public:
	barcode(const char *port, int baudrate);

	char* getcode();
	
	~barcode();
};