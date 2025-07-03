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
            std::cerr << "Error opening file: " << filePath << "\n";
            return false;
        }

        std::stringstream ss;
        ss << file.rdbuf();
        std::string content = ss.str();

        root = cJSON_Parse(content.c_str());
        if (!root) {
            std::cerr << "Failed to parse JSON\n";
            return false;
        }

        return true;
    }

    std::string getString(const std::string& key) const {
        cJSON* item = cJSON_GetObjectItem(root, key.c_str());
        return (cJSON_IsString(item)) ? item->valuestring : "";
    }

    int getInt(const std::string& key) const {
        cJSON* item = cJSON_GetObjectItem(root, key.c_str());
        return (cJSON_IsNumber(item)) ? item->valueint : 0;
    }

    double getFloat(const std::string& key) const {
        cJSON* item = cJSON_GetObjectItem(root, key.c_str());
        return (cJSON_IsNumber(item)) ? item->valuedouble : 0.0;
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
            } catch (...) {
                continue;
            }
        }
        return result;
    }

    std::vector<std::string> getStringArrayFromString(const std::string& key) const {
        std::vector<std::string> result;
        std::string s = getString(key);
        size_t start = s.find('['), end = s.find(']');
        if (start == std::string::npos || end == std::string::npos || end <= start) return result;

        std::string inner = s.substr(start + 1, end - start - 1);
        std::stringstream ss(inner);
        std::string token;
        while (std::getline(ss, token, ',')) {
            result.push_back(trim(token));
        }
        return result;
    }

    std::vector<std::string> parseStringArrayWithSingleQuotes(const std::string& key) const {
    std::vector<std::string> result;

    cJSON* item = cJSON_GetObjectItem(root, key.c_str());
        if (!cJSON_IsString(item)) return result;
    
        std::string s = item->valuestring;
    
        // Replace single quotes with double quotes
        for (auto& ch : s) {
            if (ch == '\'') ch = '"';
        }
    
        // Now try parsing it as a proper JSON array
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
        std::cout << "Age: " << getInt("age") << "\n";
        std::cout << "Height: " << getFloat("height") << "\n";

        std::cout << "Float Array: ";
        for (double val : getFloatArrayFromString("float_array")) {
            std::cout << val << " ";
        }
        std::cout << "\n";

        std::cout << "Skills: ";
        for (const auto& skill : getStringArrayFromString("skills")) {
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