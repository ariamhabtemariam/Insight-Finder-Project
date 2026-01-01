#include "Person.h"

#include <unordered_set>
#include <string>

// Helper function to convert unordered_set<string> to a comma-separated string
static std::string setToString(const std::unordered_set<std::string>& s) {
    std::string result;
    bool first = true;
    for (std::unordered_set<std::string>::const_iterator it = s.begin(); it != s.end(); ++it) {
        if (!first) result += ", ";
        result += *it;
        first = false;
    }
    return result;
}

std::string Person::toString() const {
    std::string out;
    out += "ID: " + getId() + "\n";
    out += "Graduation Year: " + std::to_string(getGraduationYear()) + "\n";
    out += "Region: " + to_string(getRegion()) + "\n";
    out += "Primary OS: " + to_string(getPrimaryOS()) + "\n";
    out += "Engineering Focus: " + to_string(getEngineeringFocus()) + "\n";
    out += "Study Time: " + to_string(getStudyTime()) + "\n";
    out += "Course Load: " + std::to_string(getCourseLoad()) + "\n";
    out += "Favorite Colors: " + setToString(getFavoriteColors()) + "\n";
    out += "Hobbies: " + setToString(getHobbies()) + "\n";
    out += "Languages: " + setToString(getLanguages()) + "\n";
    return out;
}

// ...existing code...
