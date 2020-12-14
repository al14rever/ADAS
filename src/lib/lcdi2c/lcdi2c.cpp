#include "lcdi2c.h"
#include "lcd_rus_font.h"

lcdi2c::lcdi2c() {}

lcdi2c::lcdi2c(int address) {
    this->backlight = 0x08;
    this->cursor = 0;

    this->address = wiringPiI2CSetup(address);

	this->init();
}

void lcdi2c::init() {
	//Сбросим индексы символов
	memset(this->index, 0, sizeof(this->index));

	//Переключаемся в четырёхбитный режим
	this->sendI2C(0x34);
	usleep(10);
	this->sendI2C(0x30);
	usleep(50);

	this->sendI2C(0x34);
	usleep(10);
	this->sendI2C(0x30);
	usleep(50);

	this->sendI2C(0x34);
	usleep(10);
	this->sendI2C(0x30);
	usleep(50);

	this->sendI2C(0x24);
	usleep(10);
	this->sendI2C(0x20);
	usleep(50);

	//Отсылаем конфигурацию
	this->sendCommand(0x01);
	this->sendCommand(0x03);
	this->sendCommand(0x0C);

	this->sendI2C(this->backlight);
	usleep(100);
}

void lcdi2c::print(const char *str, bool cl = false) {
	if (cl)	{
		this->clear();
	}
	int size = strlen(str);

	this->sendCommand(this->cursor | 0x80);

	for (int i = 0; i < size; ++i) {
		this->printchar(str[i]);
		this->cursor = this->cursor+1;
	}
}

void lcdi2c::print(std::string str, bool cl = false) {
    if (cl)	{
        this->clear();
    }

    this->sendCommand(this->cursor | 0x80);

    for (int i = 0; i < str.length(); ++i) {
        this->printchar(str[i]);
        this->cursor = this->cursor+1;
    }
}

void lcdi2c::print(const char *str1, const char *str2, int ofset) {
	this->clear();
	this->setcursor(0+ofset, 0);
	this->print(str1);
	this->setcursor(0+ofset, 1);
	this->print(str2);
}

void lcdi2c::print(std::string str1, std::string str2, int ofset) {
    this->clear();
    this->setcursor(0+ofset, 0);
    this->print(str1);
    this->setcursor(0+ofset, 1);
    this->print(str2);
}

void lcdi2c::print(std::string str1, std::string str2, std::string str3, std::string str4) {
    this->clear();
    this->setcursor(0, 0);
    this->print(str1);
    this->setcursor(0, 1);
    this->print(str2);
    this->setcursor(0+20, 0);
    this->print(str3);
    this->setcursor(0+20, 1);
    this->print(str4);
};

void lcdi2c::print(const char *str1, const char *str2, const char *str3, const char *str4) {
    this->clear();
    this->setcursor(0, 0);
    this->print(str1);
    this->setcursor(0, 1);
    this->print(str2);
    this->setcursor(0+20, 0);
    this->print(str3);
    this->setcursor(0+20, 1);
    this->print(str4);
};

void lcdi2c::print(const char *str, int col, int row) {
	this->setcursor(col, row);
	this->print(str);
}

void lcdi2c::setcursor(int col, int row) {
	this->cursor = col + row * 0x40;
}

void lcdi2c::clear() {
	this->sendCommand(0x01);
	this->sendCommand(0x03);
	this->cursor = 0;
}

void lcdi2c::sendI2C(int data) {
	wiringPiI2CWrite(this->address, data);
}

void lcdi2c::sendCommand(int data) {
	this->sendI2C((data & 0xF0) | this->backlight | 0x04);
	usleep(10);
	this->sendI2C((data & 0xF0) | this->backlight);
	usleep(50);
	this->sendI2C((data << 4 & 0xF0) | this->backlight | 0x04);
	usleep(10);
	this->sendI2C((data << 4 & 0xF0) | this->backlight);
	usleep(50);
}

void lcdi2c::sendData(int data) {
	this->sendI2C((data & 0xF0) | this->backlight | 0x05);
	usleep(10);
	this->sendI2C((data & 0xF0) | this->backlight);
	usleep(50);
	this->sendI2C((data << 4 & 0xF0) | this->backlight | 0x05);
	usleep(10);
	this->sendI2C((data << 4 & 0xF0) | this->backlight);
	usleep(50);
}

void lcdi2c::printchar(char ch) {
	int out;
	if (ch < 192 && ch != 168 && ch != 184)	{
		this->sendData(ch);
	} else {
		if (ch != 168 && ch != 184) {
			out = askiifilter[(int)(ch-192)];
			if (out != 0) {
				this->sendData(out);
			} else {
				out = this->searchChar(ch);
				if (out != 0) {
					this->sendData(out);
				} else {
					out = askiitoru[(int)(ch-192)];
					this->sendData(this->createChar(ru_symbols[out].bitmap, ch));
				}
			}
		} else if (ch == 168) {
			/* code */
		} else if (ch == 184) {
			/* code */
		} else {
			this->sendData(ch);
		}
	}
}

int lcdi2c::createChar(const char *bitmap, char ch) {
	if (this->index[0] == 8) {
		this->index[0] = 0;
	}
	this->index[this->index[0]+1] = ch;

	this->sendCommand(0x40 | (this->index[0]*8));
	for (int i = 0; i < 8; ++i) {
		this->sendData(bitmap[i]);
	}
	this->sendCommand(this->cursor | 0x80);
	this->index[0] = this->index[0]+1;
	return (this->index[0]-1);

}

int lcdi2c::searchChar(char ch) {
	for (int i = 1; i < 9; ++i) {
		if (this->index[i] == ch) {
			return (i-1);
		}
	}
	return 0;
}