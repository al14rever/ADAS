#include <unistd.h>
#include <wiringPiI2C.h>
#include <cstring>
#include <string>
 

//ÊÎÄÈĞÎÂÊÀ ÒÎËÜÊÎ ANSI CP1251
class lcdi2c {
private:
	int address, backlight, cursor, index[9] = {0,0,0,0,0,0,0,0,0};

	void sendI2C(int data);
    void sendCommand(int data);
    void sendData(int data);
	int createChar(const char *bitmap, char ch);
	int searchChar(char ch);
	void printchar(char ch);
	
public:
	lcdi2c();
	lcdi2c(int address);
	//~lcdi2c();

	void print(const char *str, bool cl);
    void print(std::string str, bool cl);

	void print(const char *str1, const char *str2, int ofset = 0);
    void print(std::string str1, std::string str2, int ofset = 0);

    void print(std::string str1, std::string str2, std::string str3, std::string str4);
    void print(const char *str1, const char *str2, const char *str3, const char *str4);

    void print(const char *str, int col, int row);

	void setcursor(int col, int row);

	void clear();
	
	void init();

	void backlightturn(bool turn);
};