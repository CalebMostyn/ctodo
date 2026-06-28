#ifndef _SETTINGS__H_
#define _SETTINGS__H_

#include "json.hpp"

using std::string;
using json = nlohmann::json;

class Settings {
private:
    Settings();
public:
    static Settings* Instance();
    string m_tasks_file;
    string m_interface;
    int m_port;
    float m_save_period;
    bool m_save_on_request;
};

#endif //_SETTINGS__H_
