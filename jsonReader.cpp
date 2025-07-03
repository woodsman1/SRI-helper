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
            std::cerr << "Cannot open file: " << filePath << std::endl;
            return false;
        }

        std::stringstream ss;
        ss << file.rdbuf();
        std::string content = ss.str();

        root = cJSON_Parse(content.c_str());
        if (!root) {
            std::cerr << "Error parsing JSON\n";
            return false;
        }

        return true;
    }

    std::string getString(const std::string& key) const {
        cJSON* item = cJSON_GetObjectItem(root, key.c_str());
        if (cJSON_IsString(item)) {
            return item->valuestring;
        }
        return "";
    }

    int getInt(const std::string& key) const {
        cJSON* item = cJSON_GetObjectItem(root, key.c_str());
        if (cJSON_IsNumber(item)) {
            return item->valueint;
        }
        return 0;
    }

    std::vector<std::string> getArray(const std::string& key) const {
        std::vector<std::string> result;
        cJSON* array = cJSON_GetObjectItem(root, key.c_str());
        if (cJSON_IsArray(array)) {
            cJSON* element = nullptr;
            cJSON_ArrayForEach(element, array) {
                if (cJSON_IsString(element)) {
                    result.emplace_back(element->valuestring);
                }
            }
        }
        return result;
    }

    void printAll() const {
        std::cout << "Name: " << getString("name") << "\n";
        std::cout << "Age: " << getInt("age") << "\n";
        std::cout << "Skills: ";
        for (const auto& skill : getArray("skills")) {
            std::cout << skill << " ";
        }
        std::cout << "\n";
    }

private:
    std::string filePath;
    cJSON* root;
};