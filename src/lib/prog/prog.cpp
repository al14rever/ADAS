#include "prog.h"

prog::prog(int lcdadr, const char *keyadr, int keydelay, const char *serialadr, int baudrate) {
    mongocxx::instance instance{};
    this->client = new mongocxx::client(mongocxx::uri(this->url));
    this->db = this->client->database(this->databasename);

    this->lcd = new lcdi2c(lcdadr);
    this->key = new keyboard(keyadr, keydelay);
    this->bar = new barcode(serialadr, baudrate);
};

void prog::start() {
    bool stop1 = false, cont = false;
    int mode;

    while (!stop1) {
        stop1 = false; cont = true; mode = 0;
        this->user.code.clear();
        this->user.username.clear();
        this->user.shortname.clear();
        this->user.id.clear();

        if (this->placeholder("Õ¿∆Ã»“≈ ENTER", "ƒÀﬂ ¬’Œƒ¿", 1, 96)) {
            this->lcd->print("— ¿Õ»–”…“≈ »À»", "¬¬≈ƒ»“≈  Œƒ");
            try {
                this->user.code = this->getcode(120000, 50);
            } catch (time_out) {
                cont = false;
                this->placeholder("¬–≈Ãﬂ", "¬€ÿÀŒ", 2, 1000);
            }
            this->lcd->print("œŒƒŒ∆ƒ»“≈", "");
            if (cont && this->checkuser()) {
                this->placeholder("«ƒ–¿¬—“¬”…“≈", "", 2, 1000);
                this->placeholder(this->user.shortname, "", 2, 1000);
                while (true) {
                    this->device.code.clear();
                    this->device.taken.clear();
                    this->device.id.clear();
                    mode = selectmode(120000, 50);
                    if (mode == 0) {
                        break;
                    }
                    try {
                        this->device.code = this->getcode(120000, 50);
                    } catch (time_out) {
                        this->placeholder("¬–≈Ãﬂ", "¬€ÿÀŒ", 2, 1000);
                        break;
                    }
                    this->lcd->print("œŒ∆¿À”…—“¿", "œŒƒŒ∆ƒ»“≈");
                    if (this->checkdevice()) {
                        if (mode == 1) {
                            if (this->device.taken == "0") {
                                this->updatedevice("1");
                                this->createlogentry();
                                this->placeholder("¬€ ¬«ﬂÀ»", "”—“–Œ…—“¬Œ", 2, 2000);
                                if (!this->anotherdevice(120000, 50)) {
                                    break;
                                }
                            } else {
                                this->placeholder("”—“–Œ…—“¬Œ ”∆≈", " ≈Ã-“Œ ¬«ﬂ“Œ", 2, 1500);
                                this->placeholder("≈—À» ›“Œ Œÿ»¡ ¿", "Œ¡–¿“»“≈—‹  ", 2, 1500);
                                this->placeholder("¿ƒÃ»Õ»—“–¿“Œ–”", "", 2, 1500);
                            }
                        } else if (mode == 2) {
                            if (this->device.taken == "1") {
                                if (this->checkdeviceref()) {
                                    this->updatedevice("0");
                                    this->updatelogentry();
                                    this->placeholder("¬€ ¬≈–Õ”À»", "”—“–Œ…—“¬Œ", 2, 2000);
                                    if (!this->anotherdevice(120000, 50)) {
                                        break;
                                    }
                                } else {
                                    this->placeholder("¬€ Õ≈ ¡–¿À»", " ›“Œ ”—“–Œ…—“¬Œ", 2, 2000);
                                }
                            } else {
                                this->placeholder("”—“–Œ…—“¬Œ ≈Ÿ≈", "Õ≈ ¬«ﬂ“Œ", 2, 1500);
                                this->placeholder("≈—À» ›“Œ Œÿ»¡ ¿", "Œ¡–¿“»“≈—‹  ", 2, 1500);
                                this->placeholder("¿ƒÃ»Õ»—“–¿“Œ–”", "", 2, 1500);
                            }
                        }
                    } else {
                        if (!this->user.code.empty()) {
                            this->placeholder(" Œƒ Õ≈ Õ¿…ƒ≈Õ", "œŒœ–Œ¡”…“≈ —ÕŒ¬¿", 2, 2000);
                        }
                    }
                }
                this->placeholder("ƒŒ —¬»ƒ¿Õ»ﬂ", "", 2, 2000);
            } else {
                this->placeholder(" Œƒ Õ≈ Õ¿…ƒ≈Õ", "œŒœ–Œ¡”…“≈ —ÕŒ¬¿", 2, 2000);
            }
        }
    }
};

std::string prog::getcode(int timeout, int step) {
    int time = 0;
    const int col = 16, row = 1;
    int key;
    std::string ret, bar;

    auto update_screen = [](std::string str, prog *me) {
        std::string text;
        if (str.length() > 16) {
            for (int i = str.length()-16; i < str.length(); ++i) {
                text += str[i];
            }
            me->lcd->init();
            me->lcd->print(" Œƒ:", text);
        } else {
            me->lcd->print(" Œƒ:", str);
        }
    };

    //◊ËÒÚËÏ ·ÛÙÙÂ ÓÚ ÏÛÒÓ‡
    try {
        this->bar->getcode();
    } catch(bool) {}
        this->lcd->init();

    /*
        1 - 79 4 - 75 7 - 71
        2 - 80 5 - 76 8 - 72
        3 - 81 6 - 77 9 - 73
    */

    this->lcd->print("— ¿Õ»–”…“≈ »À»", "¬¬≈ƒ»“≈  Œƒ");

    while (time < timeout) {
        try {
            key = this->key->getkey(step);

            switch(key) {
                case 79:
                    ret += "1";
                    break;
                case 80:
                    ret += "2";
                    break;
                case 81:
                    ret += "3";
                    break;
                case 75:
                    ret += "4";
                    break;
                case 76:
                    ret += "5";
                    break;
                case 77:
                    ret += "6";
                    break;
                case 71:
                    ret += "7";
                    break;
                case 72:
                    ret += "8";
                    break;
                case 73:
                    ret += "9";
                    break;
                case 82:
                    ret += "0";
                    break;
                case 83:
                    if (ret.length() > 0) {
                        ret.erase(ret.length()-1);
                    }
                    break;
                case 96:
                    return ret;
            }
            update_screen(ret, this);
            time = 0;
        } catch(end_time) {
            time += step;
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

bool prog::checkuser() {
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = this->db["users"].find_one(document{} << "code" << this->user.code << finalize);
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

        return true;
    }
    return false;
};

bool prog::checkdevice() {
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = this->db["accountings"].find_one(document{} << "code" << this->device.code << finalize);
    if(maybe_result) {
        this->device.id = maybe_result->view()["_id"].get_oid().value.to_string();
        this->device.taken = maybe_result->view()["taken"].get_utf8().value.to_string();
        return true;
    }
    return false;
};

bool prog::checkdeviceref() {
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = this->db["logs"].find_one(document{} << "user_id" << this->user.id << "device_id" << this->device.id << "returned" << 0 << finalize);
    if(maybe_result) {
        return true;
    }
    return false;
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
            document{} << "user_id" << this->user.id << "device_id" << this->device.id << "returned" << 0 << finalize,
            document{} << "$set" << open_document << "returned" << bsoncxx::types::b_date(std::chrono::system_clock::now()) << close_document << finalize);
    return true;
};

bool prog::anotherdevice(int timeout, int step) {
    int time = 0;
    int key;
    int screen = 0;

    while (time < timeout) {
        //¬˚‚Ó‰ËÏ Ì‡ ˝Í‡Ì ÚÂÍÒÚ
        if((time/3500 % 2) == 0) {
            if (screen != 1) {
                this->lcd->init();
                this->lcd->print("≈Ÿ≈ ŒƒÕŒ", "”—“–Œ…—“¬Œ?");
                screen = 1;
            }
        } else {
            if (screen != 2) {
                this->lcd->print("[1]ƒ¿", "[2]¬€’Œƒ");
                screen = 2;
            }
        }

        try {
            key = this->key->getkey(step);
            switch(key) {
                case 79:
                    return true;
                    break;
                case 80:
                    return false;
                    break;
                case 96:
                    return false;
                    break;
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

    while (time < timeout) {
        //¬˚‚Ó‰ËÏ Ì‡ ˝Í‡Ì ÚÂÍÒÚ
        if((time/3500 % 2) == 0) {
            if (screen != 1) {
                this->lcd->init();
                this->lcd->print("[1]¬«ﬂ“‹", "[2]¬≈–Õ”“‹");
                screen = 1;
            }
        } else {
            if (screen != 2) {
                this->lcd->print("[ENTER]¬€’Œƒ", "");
                screen = 2;
            }
        }

        try {
            key = this->key->getkey(step);
            switch(key) {
                case 79:
                    return 1;
                    break;
                case 80:
                    return 2;
                    break;
                case 96:
                    return 0;
                    break;
            }
        } catch(end_time) {
            time += step;
        }
    }
    return 0;
};

bool prog::placeholder(std::string str1, std::string str2, int mode, int arg) {
    this->lcd->init();
    this->lcd->print(str1, str2);
    if (mode == 1) {
        this->key->waitforkey(arg);
    } else {
        usleep(arg*1000);
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
