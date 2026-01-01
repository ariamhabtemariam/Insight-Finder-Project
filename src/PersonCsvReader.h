#ifndef PERSONCSVREADER_H
#define PERSONCSVREADER_H

#include "PersonReader.h"
#include <string>
#include <unordered_set>

/**
 * CSV implementation of PersonReader.
 * Reads a CSV file and returns a collection of Person objects.
 *
 * Expected header columns (can be adjusted):
 * id,graduation_year,region,primary_os,engineering_focus,
 * study_time,course_load,favorite_colors,hobbies,languages
 */
class PersonCsvReader : public PersonReader {
public:
    /**
     * Construct a CSV reader that will read from the given file path.
     * Example: PersonCsvReader reader("class_data_set.csv");
     */
    explicit PersonCsvReader(const std::string& filePath);

    /**
     * Read all Person rows from the CSV file.
     */
    std::vector<Person> read() override;

private:
    std::string m_filePath;

    static std::string trim(const std::string& s);
    static std::unordered_set<std::string> splitHyphenSeparated(const std::string& raw);
};

#endif // PERSONCSVREADER_H
