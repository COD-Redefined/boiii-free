#include "std_include.hpp"
#include "zone_monitor.hpp"
#include "console_log.hpp"

namespace zone_monitor {
    void component::post_unpack() {
        console_log::component::log_info("Zone monitor initialized");
    }

    std::vector<std::string> component::scan_zone_directories() {
        std::vector<std::string> available_zones;
        
        const auto data_folder = game::get_appdata_path();
        const auto zone_path = (data_folder / "data/zone").string();
        const auto usermap_path = (data_folder / "data/usermaps").string();

        // Scan zone directory
        WIN32_FIND_DATAA find_data;
        HANDLE find_handle = FindFirstFileA((zone_path + "\\*.ff").c_str(), &find_data);
        
        if (find_handle != INVALID_HANDLE_VALUE) {
            do {
                if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    std::string filename = find_data.cFileName;
                    filename = filename.substr(0, filename.length() - 3); // Remove .ff
                    available_zones.push_back(filename);
                }
            } while (FindNextFileA(find_handle, &find_data));
            FindClose(find_handle);
        }

        // Scan usermaps directory
        find_handle = FindFirstFileA((usermap_path + "\\*.ff").c_str(), &find_data);
        if (find_handle != INVALID_HANDLE_VALUE) {
            do {
                if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    std::string filename = find_data.cFileName;
                    filename = filename.substr(0, filename.length() - 3); // Remove .ff
                    available_zones.push_back(filename);
                }
            } while (FindNextFileA(find_handle, &find_data));
            FindClose(find_handle);
        }

        return available_zones;
    }

    bool component::is_valid_zone_file(const std::string& path) {
        return utils::io::file_exists(path) && path.ends_with(".ff");
    }
}

REGISTER_COMPONENT(zone_monitor::component)
