#include "lib/prog/prog.h"

#include <iostream>

int main(int argc, char const *argv[]) {
    prog prg = prog(0x3F, "/dev/input/event1", 120000, "/dev/ttyS3", 115200);
    prg.start();
    return 0;
}
