#include "settings.h"
#include "file_utils.h"

#include <memory>

static std::unique_ptr<Settings> _instance = nullptr;

Settings::Settings() {
    json config;
    FileUtils::ParseConfigFile(DEFAULT_CONFIG_FILE, config);
    m_tasks_file = FileUtils::GetDefaultTasksFile(config);
    m_interface = FileUtils::GetDefaultInterface(config);
    m_port = FileUtils::GetDefaultPort(config);
    m_save_period = FileUtils::GetSavePeriod(config);
    m_save_on_request = FileUtils::GetSaveOnRequest(config);
}

Settings* Settings::Instance() {
    if (_instance == nullptr) {
        _instance.reset(new Settings());
    }
    return _instance.get();
}
