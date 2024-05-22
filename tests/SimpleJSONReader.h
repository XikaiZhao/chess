#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

class SimpleJSONReader {
public:
    SimpleJSONReader(const std::string& line) {
        std::cout << "SimpleJSONReader: " << line << std::endl;
        parse(line);
    }

    // Access parsed data
    std::string getString(const std::string& key) {
        if (stringData.find(key) != stringData.end()) {
            return stringData[key];
        } else {
            throw std::runtime_error("String Key not found");
        }
    }

    std::vector<std::string> getStringArray(const std::string& key) {
        if (arrayData.find(key) != arrayData.end()) {
            return arrayData[key];
        } else {
            throw std::runtime_error("Array Key not found");
        }
    }

    int getInteger(const std::string& key) {
        if (intData.find(key) != intData.end()) {
            return intData[key];
        } else {
            throw std::runtime_error("Integer Key not found");
        }
    }

private:
    std::map<std::string, std::string> stringData;
    std::map<std::string, std::vector<std::string>> arrayData;
    std::map<std::string, int> intData;

    void parse(const std::string& json) {
        std::size_t pos = 0;
        while (pos < json.length()) {
            pos = json.find('"', pos);
            if (pos == std::string::npos) break;

            auto end_key = json.find('"', pos + 1);
            if (end_key == std::string::npos) break;

            std::string key = json.substr(pos + 1, end_key - pos - 1);
            pos = json.find(':', end_key);
            if (pos == std::string::npos) break;

            pos = json.find_first_not_of(" \t\n\r\f\v:", pos);
            if (pos == std::string::npos) break;

            char value_indicator = json[pos];
            if (value_indicator == '"') { // String value
                auto start_val = pos + 1;
                auto end_val = json.find('"', start_val);
                if (end_val == std::string::npos) break;
                stringData[key] = json.substr(start_val, end_val - start_val);
                pos = end_val + 1;
            } else if (value_indicator == '[') { // Array of strings
                pos++;
                std::vector<std::string> values;
                while (json[pos] != ']') {
                    if (json[pos] == '"') {
                        auto start_val = pos + 1;
                        auto end_val = json.find('"', start_val);
                        if (end_val == std::string::npos) break;
                        values.push_back(json.substr(start_val, end_val - start_val));
                        pos = end_val + 1;
                    }
                    if (pos == std::string::npos || json[pos] == ']') {
                        pos++; break;
                    }
                    pos++;
                }
                arrayData[key] = values;
                pos++;
            } else { // Number
                auto start_val = pos;
                while (isdigit(json[pos]) || json[pos] == '-') pos++;
                intData[key] = std::stoi(json.substr(start_val, pos - start_val));
            }
        }
    }
};

