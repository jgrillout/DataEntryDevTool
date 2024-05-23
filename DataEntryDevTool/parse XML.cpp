#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Function to validate the XML file
bool validateXml(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return false;
    }

    std::string line;
    bool dataDictionaryFound = false;
    bool fieldOpenTagFound = false;
    std::vector<std::string> fieldTags = {"name", "type", "mask", "length", "row", "col", "defaultValue", "label_column", "label_text"};

    while (std::getline(file, line)) {
        // Check if line contains the opening tag for dataDictionary
        if (line.find("<dataDictionary>") != std::string::npos) {
            dataDictionaryFound = true;
        }

        // Check if line contains the opening tag for field
        if (line.find("<field>") != std::string::npos) {
            fieldOpenTagFound = true;
        }

        // Check if line contains the closing tag for field
        if (line.find("</field>") != std::string::npos) {
            fieldOpenTagFound = false;
        }

        // Check if line contains any field tags
        for (const auto& tag : fieldTags) {
            if (line.find("<" + tag + ">") != std::string::npos && !fieldOpenTagFound) {
                std::cerr << "Error: Found tag <" << tag << "> outside <field> element" << std::endl;
                return false;
            }
        }

        // Check if line contains the closing tag for dataDictionary
        if (line.find("</dataDictionary>") != std::string::npos) {
            if (!dataDictionaryFound) {
                std::cerr << "Error: Found </dataDictionary> without opening <dataDictionary> tag" << std::endl;
                return false;
            }
            else {
                return true; // XML validation successful
            }
        }
    }

    std::cerr << "Error: Missing </dataDictionary> tag" << std::endl;
    return false;
}

int main() {
    std::string filename = "your_xml_file.xml";
    if (validateXml(filename)) {
        std::cout << "XML file is correctly formatted." << std::endl;
    }
    else {
        std::cerr << "XML file validation failed." << std::endl;
    }
    return 0;
}
