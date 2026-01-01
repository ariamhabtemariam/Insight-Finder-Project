#ifndef PERSON_JSON_READER_H
#define PERSON_JSON_READER_H

#include "PersonReader.h"
#include "Person.h"
#include <string>
#include <vector>

/**
 * reads person objects from a JSON URL via HTTP request.
 */
class PersonJsonReader : public PersonReader {
public:
    /**
     * constructor with URL parameter the URL to fetchez JSON data
     */
    explicit PersonJsonReader(const std::string& url);

    /**
     * gets JSON from URL and parses into Person objects then returns vector of Person objects parsed from JSON
     */
    std::vector<Person> read() override;

private:
    std::string url_;





    // make HTTP GET request
    static std::string httpGet(const std::string& url);

    // parse JSON string into Person objects
    std::vector<Person> parseJson(const std::string& jsonStr);


    // split hyphen-separated values
    static std::vector<std::string> splitHyphen(const std::string& str);

    //extract string value from JSON
    static std::string extractJsonString(const std::string& json, const std::string& key);



    //extract int value from JSON
    static int extractJsonInt(const std::string& json, const std::string& key);
};

#endif 

