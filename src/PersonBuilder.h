#ifndef PERSON_BUILDER_H
#define PERSON_BUILDER_H

#include "Person.h"
#include "PersonEnums.h"
#include <string>
#include <unordered_set>


class PersonBuilder {
public:
    PersonBuilder();
    
    //return reference for chaining
    PersonBuilder& setId(const std::string& id);
    PersonBuilder& setGraduationYear(int year);
    PersonBuilder& setRegion(Region region);
    PersonBuilder& setRegion(const std::string& regionStr);
    PersonBuilder& setPrimaryOS(PrimaryOS os);
    PersonBuilder& setPrimaryOS(const std::string& osStr);
    PersonBuilder& setEngineeringFocus(EngineeringFocus focus);
    PersonBuilder& setEngineeringFocus(const std::string& focusStr);
    PersonBuilder& setStudyTime(StudyTime time);
    PersonBuilder& setStudyTime(const std::string& timeStr);
    PersonBuilder& setCourseLoad(int load);
    
    // Set collections
    PersonBuilder& setFavoriteColors(const std::unordered_set<std::string>& colors);
    PersonBuilder& addFavoriteColor(const std::string& color);
    PersonBuilder& setHobbies(const std::unordered_set<std::string>& hobbies);
    PersonBuilder& addHobby(const std::string& hobby);
    PersonBuilder& setLanguages(const std::unordered_set<std::string>& languages);
    PersonBuilder& addLanguage(const std::string& language);
    
    // Parse comma-separated strings
    PersonBuilder& setColorsFromString(const std::string& colorsStr);
    PersonBuilder& setHobbiesFromString(const std::string& hobbiesStr);
    PersonBuilder& setLanguagesFromString(const std::string& langsStr);
    
    //Person object
    Person build() const;
    
    // reset to default values
    void reset();
    
    //copy values from existing Person to edit
    PersonBuilder& copyFrom(const Person& person);

private:
    std::string build_id;
    int build_graduationYear;
    Region build_region;
    PrimaryOS build_primaryOS;
    EngineeringFocus build_engineeringFocus;
    StudyTime build_studyTime;
    int build_courseLoad;
    std::unordered_set<std::string> build_favoriteColors;
    std::unordered_set<std::string> build_hobbies;
    std::unordered_set<std::string> build_languages;
    
    //parse comma-separated string
    static std::unordered_set<std::string> parseCommaSeparated(const std::string& str);
};

#endif 
