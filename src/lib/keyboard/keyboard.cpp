#include "keyboard.h"

//pushmode 1 for push 0 for release
keyboard::keyboard(const char *input_dev, int timeout_ms, int pushmode) {
	this->timeout_ms = timeout_ms;
	this->input_dev = input_dev;
	this->pushmode = pushmode;

	this->fds[0].fd = open(input_dev, O_RDONLY|O_NONBLOCK);

	if (this->fds[0].fd<0) {
		log("[ERROR] (keyboard 7:5) Unable to open device!");
        throw std::invalid_argument("Unable to open device!");
        return;
    }
}

keyboard::~keyboard() {
	close(fds[0].fd);
}

int keyboard::getkey(int timeout) {
	if (timeout == 0) {
		timeout = this->timeout_ms;
	}

	this->fds[0].events = POLLIN;
	memset(this->input_data,0,this->input_size);
	int ret;
	while (true) {
		ret = poll(this->fds, 1, timeout);
		if (ret > 0) {
			if (this->fds[0].revents) {
				ssize_t r = read(this->fds[0].fd, this->input_data, this->input_size);
				if (r < this->input_size) {
					log("[ERROR] (keyboard 26:17) Wrong structure!");
				} else {
					memcpy(&this->event, this->input_data, this->input_size);
					if (event.type == 1 && event.value == pushmode) {
						break;
					}
				}
			}
		} else {
			throw end_time();
		}
	}
	return event.code;
}


int keyboard::waitforkey(int code) {
	this->fds[0].events = POLLIN;
	memset(this->input_data,0,this->input_size);
	while (true) {
		poll(this->fds, 1, this->timeout_ms);
		if (this->fds[0].revents) {
			ssize_t r = read(this->fds[0].fd, this->input_data, this->input_size);
			if (r < this->input_size) {
				log("[ERROR] (keyboard 26:17) Wrong structure!");
			} else {
				memcpy(&this->event, this->input_data, this->input_size);
				if (event.type == 1 && event.value == pushmode) {
					if (event.code == code) {
						break;
					}
				}
			}
		}
	}
	return 0;
}