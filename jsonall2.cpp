#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <json/cJSON.h>

class CJsonReader {
public:
    explicit CJsonReader(const std::string& path) : filePath(path), root(nullptr) {}
    ~CJsonReader() {
        if (root) cJSON_Delete(root);
    }

    bool load() {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open: " << filePath << "\n";
            return false;
        }

        std::stringstream ss;
        ss << file.rdbuf();
        root = cJSON_Parse(ss.str().c_str());
        if (!root) {
            std::cerr << "Invalid JSON\n";
            return false;
        }

        return true;
    }

    std::string getString(const std::string& key) const {
        cJSON* item = cJSON_GetObjectItem(root, key.c_str());
        return (cJSON_IsString(item)) ? item->valuestring : "";
    }

    int getIntFromString(const std::string& key) const {
        try {
            return std::stoi(getString(key));
        } catch (...) {
            return 0;
        }
    }

    double getFloatFromString(const std::string& key) const {
        try {
            return std::stod(getString(key));
        } catch (...) {
            return 0.0;
        }
    }

    std::vector<double> getFloatArrayFromString(const std::string& key) const {
        std::vector<double> result;
        std::string s = getString(key);
        size_t start = s.find('['), end = s.find(']');
        if (start == std::string::npos || end == std::string::npos || end <= start) return result;

        std::string inner = s.substr(start + 1, end - start - 1);
        std::stringstream ss(inner);
        std::string token;
        while (std::getline(ss, token, ',')) {
            try {
                result.push_back(std::stod(trim(token)));
            } catch (...) {}
        }
        return result;
    }

    std::vector<std::string> getStringArrayFromSingleQuotedString(const std::string& key) const {
        std::vector<std::string> result;
        std::string s = getString(key);

        // Replace single quotes with double quotes
        for (auto& ch : s) {
            if (ch == '\'') ch = '"';
        }

        cJSON* array = cJSON_Parse(s.c_str());
        if (!array || !cJSON_IsArray(array)) return result;

        cJSON* element = nullptr;
        cJSON_ArrayForEach(element, array) {
            if (cJSON_IsString(element)) {
                result.emplace_back(element->valuestring);
            }
        }

        cJSON_Delete(array);
        return result;
    }

    void printAll() const {
        std::cout << "Name: " << getString("name") << "\n";
        std::cout << "Age: " << getIntFromString("age") << "\n";
        std::cout << "Height: " << getFloatFromString("height") << "\n";

        std::cout << "Float Array: ";
        for (double val : getFloatArrayFromString("float_array")) {
            std::cout << val << " ";
        }
        std::cout << "\n";

        std::cout << "Skills: ";
        for (const auto& skill : getStringArrayFromSingleQuotedString("skills")) {
            std::cout << skill << " ";
        }
        std::cout << "\n";
    }

private:
    std::string filePath;
    cJSON* root;

    std::string trim(const std::string& str) const {
        const char* ws = " \t\n\r";
        size_t start = str.find_first_not_of(ws);
        size_t end = str.find_last_not_of(ws);
        return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
    }
};