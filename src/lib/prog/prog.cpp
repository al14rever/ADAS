#include "prog.h"

prog::prog(int lcdadr, const char *keyadr, const char *databaseuri, const char *database, const char *serialadr, int baudrate, int timeout) {
    this->url = databaseuri;
    this->databasename = database;
    this->timeout = timeout;
    mongocxx::instance instance{};
    this->client = new mongocxx::client(mongocxx::uri(this->url));
    this->db = this->client->database(this->databasename);

    this->lcd = new lcdi2c(lcdadr);
    this->key = new keyboard(keyadr, this->timeout);
    this->bar = new barcode(serialadr, baudrate);
};

void prog::start() {
    bool stop1 = false, cont;
    int mode;

    while (!stop1) {
        stop1 = false; cont = true; mode = 0;
        this->user.code.clear();
        this->user.username.clear();
        this->user.shortname.clear();
        this->user.id.clear();

        if (this->placeholder("мюфлхре ENTER", "дкъ бундю", "", "", 2, 96, this->timeout*2, 50)) {
            try {
                this->user.code = this->getcode(this->timeout, 50, 1);
            } catch (time_out) {
                cont = false;
                this->placeholder("бпелъ бшькн", "", "", "йсдю?", 3, 1000);
            }
            int usrchk = 2;
            if (cont) {
                this->lcd->print("онднфдхре", "");
                usrchk = this->checkuser();
            }
            if (cont && usrchk == 1) {
                this->placeholder("гдпюбярбсире", this->user.shortname, "", "", 3, 1500);
                while (true) {
                    this->device.code.clear();
                    this->device.taken.clear();
                    this->device.id.clear();
                    mode = selectmode(this->timeout, 50);
                    if (mode == 0) {
                        break;
                    }
                    try {
                        this->device.code = this->getcode(this->timeout, 50, 2);
                    } catch (time_out) {
                        this->placeholder("бпелъ", "бшькн", "", "", 3, 1000);
                        break;
                    }
                    this->lcd->print("онднфдхре", "");
                    if (this->checkdevice()) {
                        if (mode == 1) {
                            if (this->device.taken == "0") {
                                this->updatedevice("1");
                                this->createlogentry();
                                this->placeholder("бш бгъкх сярпниярбн", "сярпниярбн", this->device.name, "", 3, 2000);
                                if (!this->anotherdevice(this->timeout, 50)) {
                                    break;
                                }
                            } else {
                                this->updatelogentry();
                                this->createlogentry();
                                this->placeholder("бш бгъкх сярпниярбн", "сярпниярбн", this->device.name, "", 3, 2000);
                                if (!this->anotherdevice(this->timeout, 50)) {
                                    break;
                                }
                            }
                        } else if (mode == 2) {
                            if (this->device.taken != "0") {
                                this->updatedevice("0");
                                this->updatelogentry();
                                this->placeholder("бш бепмскх", "сярпниярбн", this->device.name, "", 3, 2000);
                                if (!this->anotherdevice(this->timeout, 50)) {
                                    break;
                                }
                            } else {
                                this->placeholder("щрн сярпниярбн еые", "мхйрн ме бгък", "", "", 3, 1500);
                            }
                        }
                    } else {
                        if (!this->user.code.empty()) {
                            this->placeholder("йнд ме мюидем", "онопнасире ямнбю", "", "", 3, 2000);
                        }
                    }
                }
                this->placeholder("дн ябхдюмхъ", "", "", "", 3, 2000);
            } else if (usrchk == 0) {
                this->placeholder("йнд ме мюидем", "онопнасире ямнбю", "", "", 3, 2000);
            } else if (usrchk == -1) {
                this->placeholder("янедхмемхе я ад ме", "Yдюкняэ, онопнаYире", "еые пю3", "", 3, 2000);
                return;
            }
        }
    }
};

std::string prog::getcode(int timeout, int step, int mode) {
    int time = 0;
    int timeforscan = 0;
    int key;
    bool scan = true;
    std::string ret, bar;


    auto update_screen = [](std::string str, prog *me) {
        std::string text;
        if (str.length() > 20) {
            for (int i = str.length()-20; i < str.length(); ++i) {
                text += str[i];
            }
            me->lcd->init();
            me->lcd->print("йнд:", text);
        } else {
            me->lcd->print("йнд:", str);
        }
    };

    //вХЯРХЛ АСТТЕП НР ЛСЯНПЮ
    try {
        this->bar->getcode();
    } catch(bool) {}

    /*
        1 - 79 4 - 75 7 - 71
        2 - 80 5 - 76 8 - 72
        3 - 81 6 - 77 9 - 73
    */


    this->lcd->init();
    if (mode == 1) {
        this->lcd->print("яйюмхпсире хкх", "ббедхре йнд", "онкэ3нбюрекъ", "");
    } else if (mode == 2) {
        this->lcd->print("яйюмхпсире хкх", "ббедхре йнд", "сярпниярбю", "");
    }



    while (time < timeout) {
        try {
            key = this->key->getkey(step);

            switch(key) {
                case 2:
                case 79:
                    ret += "1";
                    break;
                case 3:
                case 80:
                    ret += "2";
                    break;
                case 4:
                case 81:
                    ret += "3";
                    break;
                case 5:
                case 75:
                    ret += "4";
                    break;
                case 6:
                case 76:
                    ret += "5";
                    break;
                case 7:
                case 77:
                    ret += "6";
                    break;
                case 8:
                case 71:
                    ret += "7";
                    break;
                case 9:
                case 72:
                    ret += "8";
                    break;
                case 10:
                case 73:
                    ret += "9";
                    break;
                case 11:
                case 82:
                    ret += "0";
                    break;
                case 14:
                case 52:
                case 83:
                    if (ret.length() > 0) {
                        ret.erase(ret.length()-1);
                    }
                    break;
                case 96:
                    return ret;
            }
            if (ret.length() > 16) {
                ret.erase(ret.length()-1);
            }
            if (scan) {
                if (ret.length() > 0) {
                    scan = false;
                }
            }
            update_screen(ret, this);
            time = 0;
        } catch(end_time) {
            time += step;
        }

        if (scan) {
            if (timeforscan > 500) {
                this->bar->trigger();
                timeforscan = 0;
            } else {
                timeforscan += step;
            }
        }

        try {
            bar = this->bar->getcode();
            for (int i = 0; i < bar.size(); ++i) {
                if (bar[i] == '\n' || bar[i] == '\r') {
                    bar.erase(i);
                }
            }
            return bar;
        } catch(bool) {}
    }
    throw time_out();
};

int prog::checkuser() {
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result;

    try {
        maybe_result = this->db["users"].find_one(document{} << "code" << this->user.code << finalize);
    }
    catch(const std::runtime_error& re) {
        log("[ERROR] (prog:218) ", re.what());
        return -1;
    }
    catch(const std::exception& ex) {
        log("[ERROR] (prog:222) ", ex.what());
        return -1;
    }
    catch(...) {
        log("[ERROR] (prog:226) Unknown failure occurred. Possible memory corruption");
        return -1;
    }

    if(maybe_result) {
        this->user.username = maybe_result->view()["about"].get_utf8().value.to_string();
        this->user.id = maybe_result->view()["_id"].get_oid().value.to_string();
        char *utf8 = new char[this->user.username.size()];
        char *cp1251 = new char[this->user.username.size()];
        memset(cp1251, 0, this->user.username.size());
        strcpy(utf8, this->user.username.c_str());
        this->convert_utf8_to_windows1251(utf8, cp1251, this->user.username.size());
        free(utf8);

        int cursor = 0;
        for (int i = 0; i < this->user.username.size(); ++i) {
            if (cp1251[i] != 0x20) {
                this->user.shortname.push_back(cp1251[i]);
            } else if(cp1251[i] == ',') {
                cursor = i;
                break;
            } else {
                this->user.shortname.push_back(cp1251[i]);
                cursor = i+1;
                this->user.shortname.push_back(cp1251[cursor]);
                this->user.shortname.push_back('.');
                break;
            }
        }

        for (int i = cursor; i < this->user.username.size(); ++i) {
            if (cp1251[i] == 0x20) {
                this->user.shortname.push_back(cp1251[i]);
                this->user.shortname.push_back(cp1251[i+1]);
                this->user.shortname.push_back('.');
                break;
            } else if(cp1251[i] == ',') {
                break;
            }
        }

        free(cp1251);

        for (int i = 0; i < this->user.shortname.length(); ++i) {
            if ((int)this->user.shortname[i] > 223) {
                this->user.shortname[i] = (char)((int)this->user.shortname[i]-32);
            }
            if ((int)this->user.shortname[i] == 168 || (int)this->user.shortname[i] == 184) {
                this->user.shortname[i] = (char)197;
            }
        }

        return 1;
    }
    return 0;
};

bool prog::checkdevice() {
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = this->db["accountings"].find_one(document{} << "code" << this->device.code << finalize);
    if(maybe_result) {
        this->device.id = maybe_result->view()["_id"].get_oid().value.to_string();
        this->device.dev_id = maybe_result->view()["device_id"].get_utf8().value.to_string();
        this->device.taken = maybe_result->view()["taken"].get_utf8().value.to_string();
    } else {
        return false;
    }
    maybe_result = this->db["devices"].find_one(document{} << "_id" << bsoncxx::oid{bsoncxx::stdx::string_view{this->device.dev_id}} << finalize);
    if(maybe_result) {
        this->device.name = maybe_result->view()["name"].get_utf8().value.to_string();
    } else {
        return false;
    }
    return true;
};

bool prog::updatedevice(std::string taken) {
    this->db["accountings"].update_one(
            document{} << "code" << this->device.code << finalize,
            document{} << "$set" << open_document << "taken" << taken << close_document << finalize);
    return true;
};

bool prog::createlogentry() {
    bsoncxx::document::value doc_value = bsoncxx::builder::stream::document{}
        << "user_id" << this->user.id
        << "device_id" << this->device.id
        << "return_user_id" << 0
        << "received" << bsoncxx::types::b_date(std::chrono::system_clock::now())
        << "returned" << 0
        << "__v" << 0
        << bsoncxx::builder::stream::finalize;
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result = this->db["logs"].insert_one(std::move(doc_value));
    if (result) {
        return true;
    }
    return false;
};

bool prog::updatelogentry() {
    this->db["logs"].update_one(
            document{} << "device_id" << this->device.id << "returned" << 0 << finalize,
            document{} << "$set" << open_document << "returned" << bsoncxx::types::b_date(std::chrono::system_clock::now()) << "return_user_id" << this->user.id << close_document << finalize);
    return true;
};

bool prog::anotherdevice(int timeout, int step) {
    int time = 0;
    int key;
    int screen = 0;

    this->lcd->init();
    this->lcd->print("еые ндмн сярпниярбн?", "[1]дю", "[2]бшунд", "");

    while (time < timeout) {
        //бШБНДХЛ МЮ ЩЙПЮМ РЕЙЯР
        /*
        if((time/3500 % 2) == 0) {
            if (screen != 1) {
                this->lcd->init();
                this->lcd->print("еые ндмн", "сярпниярбн?");
                screen = 1;
            }
        } else {
            if (screen != 2) {
                this->lcd->print("[1]дю", "[2]бшунд");
                screen = 2;
            }
        }
         */

        try {
            key = this->key->getkey(step);
            switch(key) {
                case 2:
                case 79:
                    return true;
                case 3:
                case 80:
                    return false;
                case 96:
                    return false;
            }
        } catch(end_time) {
            time += step;
        }
    }
    return false;
};

int prog::selectmode(int timeout, int step) {
    int time = 0;
    int key;
    int screen = 0;

    this->lcd->init();
    this->lcd->print("[1]бгърэ", "[2]бепмсрэ", "[ENTER]бшунд", "");

    while (time < timeout) {
        //бШБНДХЛ МЮ ЩЙПЮМ РЕЙЯР
        /*
        if((time/3500 % 2) == 0) {
            if (screen != 1) {
                this->lcd->init();
                this->lcd->print("[1]бгърэ", "[2]бепмсрэ");
                screen = 1;
            }
        } else {
            if (screen != 2) {
                this->lcd->print("[ENTER]бшунд", "");
                screen = 2;
            }
        }
         */

        try {
            key = this->key->getkey(step);
            switch(key) {
                case 2:
                case 79:
                    return 1;
                case 3:
                case 80:
                    return 2;
                case 96:
                    return 0;
            }
        } catch(end_time) {
            time += step;
        }
    }
    return 0;
};

bool prog::placeholder(std::string str1, std::string str2, std::string str3, std::string str4, int mode, int arg, int timeout, int step) {
    this->lcd->init();
    this->lcd->print(str1, str2, str3, str4);
    if (mode == 2) {
        int time = 0;
        int key;

        while (time < timeout) {
            try {
                key = this->key->getkey(50);
                if (key == arg) {
                    return true;
                }
            } catch (end_time) {
                time += step;
            }
        }

        this->lcd->clear();
        this->lcd->backlightturn(false);
        this->key->waitforkey(arg);
        this->lcd->backlightturn(true);
        this->placeholder("мюфлхре ENTER", "дкъ бундю", "", "", 2, 96, this->timeout*2, 50);
    } else if (mode == 1) {
        this->key->waitforkey(arg);
    } else {
        usleep(arg * 1000);
    }
    return true;
};

int prog::convert_utf8_to_windows1251(const char* utf8, char* windows1251, size_t n)
{
    int i = 0;
    int j = 0;
    for(; i < (int)n && utf8[i] != 0; ++i) {
        char prefix = utf8[i];
        char suffix = utf8[i+1];
        if ((prefix & 0x80) == 0) {
            windows1251[j] = (char)prefix;
            ++j;
        } else if ((~prefix) & 0x20) {
            int first5bit = prefix & 0x1F;
            first5bit <<= 6;
            int sec6bit = suffix & 0x3F;
            int unicode_char = first5bit + sec6bit;

            if ( unicode_char >= 0x410 && unicode_char <= 0x44F ) {
                windows1251[j] = (char)(unicode_char - 0x350);
            } else if (unicode_char >= 0x80 && unicode_char <= 0xFF) {
                windows1251[j] = (char)(unicode_char);
            } else if (unicode_char >= 0x402 && unicode_char <= 0x403) {
                windows1251[j] = (char)(unicode_char - 0x382);
            } else {
                int count = sizeof(g_letters) / sizeof(Letter);
                for (int k = 0; k < count; ++k) {
                    if (unicode_char == g_letters[k].unicode) {
                        windows1251[j] = g_letters[k].win1251;
                        goto NEXT_LETTER;
                    }
                }
                // can't convert this char
                return 0;
            }
            NEXT_LETTER:
            ++i;
            ++j;
        } else {
            // can't convert this chars
            return 0;
        }
    }
    windows1251[j] = 0;
    return 1;
}
