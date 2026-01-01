#include "PersonBuilder.h"
#include <sstream>

PersonBuilder::PersonBuilder() {
    reset();
}

void PersonBuilder::reset() {
    build_id = "";
    build_graduationYear = 0;
    build_region = Region::Unknown;
    build_primaryOS = PrimaryOS::Unknown;
    build_engineeringFocus = EngineeringFocus::Unknown;
    build_studyTime = StudyTime::Unknown;
    build_courseLoad = 0;
    build_favoriteColors.clear();
    build_hobbies.clear();
    build_languages.clear();
}

PersonBuilder& PersonBuilder::copyFrom(const Person& person) {
    build_id = person.getId();
    build_graduationYear = person.getGraduationYear();
    build_region = person.getRegion();
    build_primaryOS = person.getPrimaryOS();
    build_engineeringFocus = person.getEngineeringFocus();
    build_studyTime = person.getStudyTime();
    build_courseLoad = person.getCourseLoad();
    build_favoriteColors = person.getFavoriteColors();
    build_hobbies = person.getHobbies();
    build_languages = person.getLanguages();
    return *this;
}

PersonBuilder& PersonBuilder::setId(const std::string& id) {
    build_id = id;
    return *this;
}

PersonBuilder& PersonBuilder::setGraduationYear(int year) {
    build_graduationYear = year;
    return *this;
}

PersonBuilder& PersonBuilder::setRegion(Region region) {
    build_region = region;
    return *this;
}

PersonBuilder& PersonBuilder::setRegion(const std::string& regionStr) {
    build_region = parse_region(regionStr);
    return *this;
}

PersonBuilder& PersonBuilder::setPrimaryOS(PrimaryOS os) {
    build_primaryOS = os;
    return *this;
}

PersonBuilder& PersonBuilder::setPrimaryOS(const std::string& osStr) {
    build_primaryOS = parse_primary_os(osStr);
    return *this;
}

PersonBuilder& PersonBuilder::setEngineeringFocus(EngineeringFocus focus) {
    build_engineeringFocus = focus;
    return *this;
}

PersonBuilder& PersonBuilder::setEngineeringFocus(const std::string& focusStr) {
    build_engineeringFocus = parse_engineering_focus(focusStr);
    return *this;
}

PersonBuilder& PersonBuilder::setStudyTime(StudyTime time) {
    build_studyTime = time;
    return *this;
}

PersonBuilder& PersonBuilder::setStudyTime(const std::string& timeStr) {
    build_studyTime = parse_study_time(timeStr);
    return *this;
}

PersonBuilder& PersonBuilder::setCourseLoad(int load) {
    build_courseLoad = load;
    return *this;
}

PersonBuilder& PersonBuilder::setFavoriteColors(const std::unordered_set<std::string>& colors) {
    build_favoriteColors = colors;
    return *this;
}

PersonBuilder& PersonBuilder::addFavoriteColor(const std::string& color) {
    if (!color.empty()) {
        build_favoriteColors.insert(color);
    }
    return *this;
}

PersonBuilder& PersonBuilder::setHobbies(const std::unordered_set<std::string>& hobbies) {
    build_hobbies = hobbies;
    return *this;
}

PersonBuilder& PersonBuilder::addHobby(const std::string& hobby) {
    if (!hobby.empty()) {
        build_hobbies.insert(hobby);
    }
    return *this;
}

PersonBuilder& PersonBuilder::setLanguages(const std::unordered_set<std::string>& languages) {
    build_languages = languages;
    return *this;
}

PersonBuilder& PersonBuilder::addLanguage(const std::string& language) {
    if (!language.empty()) {
        build_languages.insert(language);
    }
    return *this;
}

std::unordered_set<std::string> PersonBuilder::parseCommaSeparated(const std::string& str) {
    std::unordered_set<std::string> result;
    std::stringstream ss(str);
    std::string item;
    
    while (std::getline(ss, item, ',')) {
        // Trim whitespace
        size_t start = item.find_first_not_of(" \t");
        size_t end = item.find_last_not_of(" \t");
        if (start != std::string::npos && end != std::string::npos) {
            result.insert(item.substr(start, end - start + 1));
        }
    }
    return result;
}

PersonBuilder& PersonBuilder::setColorsFromString(const std::string& colorsStr) {
    build_favoriteColors = parseCommaSeparated(colorsStr);
    return *this;
}

PersonBuilder& PersonBuilder::setHobbiesFromString(const std::string& hobbiesStr) {
    build_hobbies = parseCommaSeparated(hobbiesStr);
    return *this;
}

PersonBuilder& PersonBuilder::setLanguagesFromString(const std::string& langsStr) {
    build_languages = parseCommaSeparated(langsStr);
    return *this;
}

Person PersonBuilder::build() const {
    return Person(
        build_id,
        build_graduationYear,
        build_region,
        build_primaryOS,
        build_engineeringFocus,
        build_studyTime,
        build_courseLoad,
        build_favoriteColors,
        build_hobbies,
        build_languages
    );
}

