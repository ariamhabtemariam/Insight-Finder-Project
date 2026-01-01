#include "PersonJsonReader.h"
#include "PersonEnums.h"

#include <curl/curl.h>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <iostream>


//libcurl 
static size_t WriteCallback(void* receivedContents, size_t elementSize, size_t numberOfElements, std::string* outputString) {
    size_t totalBytesReceived = elementSize * numberOfElements;
    outputString->append(static_cast<char*>(receivedContents), totalBytesReceived);
    return totalBytesReceived;
}



PersonJsonReader::PersonJsonReader(const std::string& url) : url_(url) {}

std::vector<Person> PersonJsonReader::read() {
    std::string jsonString = httpGet(url_);
    if (jsonString.empty()) {
        std::cerr << "Failed to fetch JSON from URL: " << url_ << std::endl;
        return {};
    }
    return parseJson(jsonString);
}


std::string PersonJsonReader::httpGet(const std::string& url) {
    std::string httpResponse;
    CURL* curlHandle = curl_easy_init();
    if (!curlHandle) {
        std::cerr << "Failed to initialize libcurl" << std::endl;
        return "";
    }



    // set libcurl 
    curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &httpResponse);
    curl_easy_setopt(curlHandle, CURLOPT_FOLLOWLOCATION, 1L); // follow redirects
    curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT, 30L); // 30 second timeout
    curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYPEER, 0L); // skip SSL verification 
    curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYHOST, 0L);  // dkip host verification
    // perform HTTP GET request
    CURLcode curlResult = curl_easy_perform(curlHandle);




    if (curlResult != CURLE_OK) {
        std::cerr << "HTTP request failed: " << curl_easy_strerror(curlResult) << std::endl;
        httpResponse = "";
    }
    curl_easy_cleanup(curlHandle);
    return httpResponse;
}




std::vector<std::string> PersonJsonReader::splitHyphen(const std::string& inputString) {
    std::vector<std::string> resultList;
    std::stringstream stringStream(inputString);
    std::string currentItem;
    


//splitting/handling
    while (std::getline(stringStream, currentItem, '-')) {
        // trim whitespace from beginning
        currentItem.erase(0, currentItem.find_first_not_of(" \t\r\n"));
        // trim whitespace from end
        currentItem.erase(currentItem.find_last_not_of(" \t\r\n") + 1);
        if (!currentItem.empty()) {
            resultList.push_back(currentItem);
        }
    }
    return resultList;
}



//extracts string value from JSON/ error handling
std::string PersonJsonReader::extractJsonString(const std::string& jsonContent, const std::string& keyName) {
    std::string formattedSearchKey = "\"" + keyName + "\"";
    size_t keyPosition = jsonContent.find(formattedSearchKey);
    // if key not found return empty string
    if (keyPosition == std::string::npos) {
        return "";
    }
    // finds the colon after key
    size_t colonPosition = jsonContent.find(':', keyPosition + formattedSearchKey.length());
    if (colonPosition == std::string::npos) {
        return "";
    }
    // skip whitespace to find value start
    size_t valueStartPosition = colonPosition + 1;
    while (valueStartPosition < jsonContent.length() && std::isspace(jsonContent[valueStartPosition])) {
        valueStartPosition++;
    }
// if value start position is greater than or equal to json content length return empty string
    if (valueStartPosition >= jsonContent.length()) {
        return "";
    }
    
    // checks if string starts with quote
    if (jsonContent[valueStartPosition] == '"') {
        size_t stringStartPosition = valueStartPosition + 1;
        size_t stringEndPosition = jsonContent.find('"', stringStartPosition);
        // if string end position not found return empty string
        if (stringEndPosition != std::string::npos) {
            return jsonContent.substr(stringStartPosition, stringEndPosition - stringStartPosition);
        }
    }
    return "";
}




//extracts int value
int PersonJsonReader::extractJsonInt(const std::string& jsonContent, const std::string& keyName) {
    std::string formattedSearchKey = "\"" + keyName + "\"";
    size_t keyPosition = jsonContent.find(formattedSearchKey);
    // if key not found return 0
    if (keyPosition == std::string::npos) {
        return 0;
    }
    // find colon after key
    size_t colonPosition = jsonContent.find(':', keyPosition + formattedSearchKey.length());
    if (colonPosition == std::string::npos) {
        return 0;
    }
    // skip whitespace to find value start
    size_t valueStartPosition = colonPosition + 1;
    while (valueStartPosition < jsonContent.length() && std::isspace(jsonContent[valueStartPosition])) {
        valueStartPosition++;
    }
// if value start position is greater than or equal to json content length return 0
    std::string numberString;
    while (valueStartPosition < jsonContent.length() && 
    // checks if value start position is a digit or hyphen
           (std::isdigit(jsonContent[valueStartPosition]) || jsonContent[valueStartPosition] == '-')) {
        numberString += jsonContent[valueStartPosition];
        valueStartPosition++;
    }
    // if number string is not empty return integer value of number string
    if (!numberString.empty()) {
        return std::stoi(numberString);
    }
    return 0;
}







//parses JSON string into Person objects
std::vector<Person> PersonJsonReader::parseJson(const std::string& jsonString) {
    std::vector<Person> personsList;
    // find people array in the JSON
    size_t peopleArrayPosition = jsonString.find("\"people\"");
    if (peopleArrayPosition == std::string::npos) {
        // if people array not found return empty list
        std::cerr << "JSON missing 'people' array" << std::endl;
        return personsList;
    }
    // find the opening bracket of the array
    size_t arrayStartPosition = jsonString.find('[', peopleArrayPosition);
    if (arrayStartPosition == std::string::npos) {
        return personsList;
    }


//error handling brackets 
    // parse each person object in the array
    size_t currentPosition = arrayStartPosition;
    while (currentPosition < jsonString.length()) {
        // finds the next person object, its adn opening bracket
        size_t objectStartPosition = jsonString.find('{', currentPosition);
        if (objectStartPosition == std::string::npos) {
            break;
        }
        // matches closing brace by counting braces
        int braceDepthCounter = 1;
        size_t objectEndPosition = objectStartPosition + 1;
        while (objectEndPosition < jsonString.length() && braceDepthCounter > 0) {
            if (jsonString[objectEndPosition] == '{') braceDepthCounter++;
            else if (jsonString[objectEndPosition] == '}') braceDepthCounter--;
            objectEndPosition++;
        }
// if brace depth counter is not 0 return empty list
        if (braceDepthCounter != 0) {
            break;  // json not right
        }





        // extracts the single person's JSON object
        std::string singlePersonJson = jsonString.substr(objectStartPosition, objectEndPosition - objectStartPosition);

        // parse individual fields from the person object
        std::string personId = extractJsonString(singlePersonJson, "id");
        int graduationYear = extractJsonInt(singlePersonJson, "graduationYear");
        std::string regionString = extractJsonString(singlePersonJson, "region");
        std::string operatingSystemString = extractJsonString(singlePersonJson, "primaryOS");
        std::string engineeringFocusString = extractJsonString(singlePersonJson, "engineeringFocus");
        std::string studyTimeString = extractJsonString(singlePersonJson, "studyTime");
        int courseLoad = extractJsonInt(singlePersonJson, "courseLoad");
        std::string favoriteColorsString = extractJsonString(singlePersonJson, "favoriteColors");
        std::string hobbiesString = extractJsonString(singlePersonJson, "hobbies");
        std::string languagesString = extractJsonString(singlePersonJson, "languages");

        //parse enum values using helper functions
        Region parsedRegion = parse_region(regionString);
        PrimaryOS parsedOperatingSystem = parse_primary_os(operatingSystemString);
        EngineeringFocus parsedEngineeringFocus = parse_engineering_focus(engineeringFocusString);
        StudyTime parsedStudyTime = parse_study_time(studyTimeString);

        //parse hyphen-separated sets into unordered_sets
        std::unordered_set<std::string> favoriteColorsSet;
        for (const auto& colorItem : splitHyphen(favoriteColorsString)) {
            favoriteColorsSet.insert(colorItem);
        }

        //parses hobbies hyphen-separated sets into unordered_sets
        std::unordered_set<std::string> hobbiesSet;
        for (const auto& hobbyItem : splitHyphen(hobbiesString)) {
            hobbiesSet.insert(hobbyItem);
        }

        //parses languages hyphen-separated sets into unordered_sets
        std::unordered_set<std::string> languagesSet;
        for (const auto& languageItem : splitHyphen(languagesString)) {
            languagesSet.insert(languageItem);
        }

    





        //creates Person object using constructor, Person class is immutable
        Person newPerson(
            personId,
            graduationYear,
            parsedRegion,
            parsedOperatingSystem,
            parsedEngineeringFocus,
            parsedStudyTime,
            courseLoad,
            favoriteColorsSet,
            hobbiesSet,
            languagesSet
        );
//adds new person to list
        personsList.push_back(std::move(newPerson));
        currentPosition = objectEndPosition;
    }

    return personsList;
}
