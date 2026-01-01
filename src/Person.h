#ifndef PERSON_H
#define PERSON_H

#include "PersonEnums.h"
#include <string>
#include <unordered_set>

// ------------------------------------------------------------
// Person
//
// This class models one person or student record.
// All fields are const, so once a Person is created,
// it cannot be changed. To "edit" a person, create
// a new Person with the updated values.
// ------------------------------------------------------------
class Person {
private:
    /**
     * A unique identifier for the person - can be a nickname or an auto-generated id.
     */
    std::string id;

    /**
     * Gradutaion year, e.g. 2025
     */
    int graduationYear;

    /**
     * Birthplace/Home region as specified by the Region enum.
     */
    Region region;

    /**
     * Primary development operating system as specified by the PrimaryOS enum.
     */
    PrimaryOS primaryOS;

    /**
     * Major engineering focus as specified by the EngineeringFocus enum.
     */
    EngineeringFocus engineeringFocus;

    /**
     * Preferred, most productive study time as specified by the StudyTime enum.
     */
    StudyTime studyTime;

    /**
     * Number of courses the person is currently taking.
     */
    int courseLoad;

    // Tag-like collections (immutable)

    /**
     * Zero or more favorite colors.
     */
    std::unordered_set<std::string> favoriteColors;

    /**
     * Zero or more hobbies of interest.
     */
    std::unordered_set<std::string> hobbies;

    /**
     * Zero or more spoken languages.
     */
    std::unordered_set<std::string> languages;
public:
    Person(const std::string& id,
           int graduationYear,
           Region region,
           PrimaryOS primaryOS,
           EngineeringFocus engineeringFocus,
           StudyTime studyTime,
           int courseLoad,
           const std::unordered_set<std::string>& favoriteColors = {},
           const std::unordered_set<std::string>& hobbies = {},
           const std::unordered_set<std::string>& languages = {})
        : id(id),
          graduationYear(graduationYear),
          region(region),
          primaryOS(primaryOS),
          engineeringFocus(engineeringFocus),
          studyTime(studyTime),
          courseLoad(courseLoad),
          favoriteColors(favoriteColors),
          hobbies(hobbies),
          languages(languages)
    {}

    // --- Getters (read-only access) ---
    const std::string& getId() const { return id; }
    int getGraduationYear() const { return graduationYear; }
    Region getRegion() const { return region; }
    PrimaryOS getPrimaryOS() const { return primaryOS; }
    EngineeringFocus getEngineeringFocus() const { return engineeringFocus; }
    StudyTime getStudyTime() const { return studyTime; }
    int getCourseLoad() const { return courseLoad; }

    const std::unordered_set<std::string>& getFavoriteColors() const { return favoriteColors; }
    const std::unordered_set<std::string>& getHobbies() const { return hobbies; }
    const std::unordered_set<std::string>& getLanguages() const { return languages; }

    std::string toString() const;
};

#endif // PERSON_H
