#include "lib/prog/prog.h"
#include <fstream>
#include <algorithm>
#include <vector>

int main(int argc, char const *argv[]) {
    std::vector<std::string*> config;

    std::string *cfgpat;
    std::ifstream cFile ("adas.config");
    if (cFile.is_open())
    {
        std::string line;
        while(getline(cFile, line)){
            line.erase(std::remove_if(line.begin(), line.end(), isspace),
                       line.end());
            if(line[0] == '#' || line.empty())
                continue;
            auto delimiterPos = line.find("=");
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);
            cfgpat = new std::string[2];
            cfgpat[0] = name;
            cfgpat[1] = value;
            config.push_back(cfgpat);
        }
    }
    else {
        //can't read config
        return -1;
    }

    std::string finalconfig[6];
    /*
     * 0 - lcd address
     * 1 - key address (path to event file ex /dev/input/event0)
     * 2 - mongodb url
     * 3 - mongodb database name
     * 4 - uart address (path, ex /dev/ttyS3)
     * 5 - uart baud rate (9600, 1920, 57600, 115200 etc.)
     */

    if (config.size() > 6) {
        //wrong config length
        return -2;
    }

    for (int i = 0; i < 6; ++i) {
        if(config[i][0].compare("lcd_address") == 0) {
            finalconfig[0] = config[i][1];
        } else if(config[i][0].compare("keyboard_address") == 0) {
            finalconfig[1] = config[i][1];
        } else if(config[i][0].compare("mongodb_url") == 0) {
            finalconfig[2] = config[i][1];
        } else if(config[i][0].compare("mongo_db_name") == 0) {
            finalconfig[3] = config[i][1];
        } else if(config[i][0].compare("uart_address") == 0) {
            finalconfig[4] = config[i][1];
        } else if(config[i][0].compare("uart_baud_rate") == 0) {
            finalconfig[5] = config[i][1];
        }
    }

    prog prg = prog(std::stoi(finalconfig[0]), finalconfig[1].c_str(), finalconfig[2].c_str(), finalconfig[3].c_str(), finalconfig[4].c_str(), std::stoi(finalconfig[5]));
    prg.start();
    return 0;
}
