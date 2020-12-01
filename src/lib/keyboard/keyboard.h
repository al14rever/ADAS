#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <cstring>
#include <exception>
#include <linux/input.h>

#include "../log/log.h"

struct end_time : public std::exception {
   const char * what () const throw () {
      return "Timeout";
   }
};

class keyboard {
private:
	const char *input_dev;
	int timeout_ms;
	int pushmode;
	struct pollfd fds[1];
	struct input_event event;

	int input_size = sizeof(event);
	char *input_data = new char[input_size];

public:
	keyboard(const char *input_dev, int timeout_ms = 5000, int pushmode = 0);
	~keyboard();
	int getkey(int timeout = 0);
	int waitforkey(int code);
};