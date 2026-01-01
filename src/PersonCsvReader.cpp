#include "PersonCsvReader.h"
#include "PersonEnums.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

// helper functions 

std::string PersonCsvReader::trim(const std::string& s) {
    const char* ws = " \t\r\n";
    std::size_t start = s.find_first_not_of(ws);
    if (start == std::string::npos) return "";
    std::size_t end = s.find_last_not_of(ws);
    return s.substr(start, end - start + 1);
}

std::unordered_set<std::string>
PersonCsvReader::splitHyphenSeparated(const std::string& raw) {
    std::unordered_set<std::string> result;
    std::stringstream ss(raw);
    std::string token;

    while (std::getline(ss, token, '-')) {
        std::string t = trim(token);
        if (!t.empty()) {
            result.insert(t);
        }
    }
    return result;
}

// constructor

PersonCsvReader::PersonCsvReader(const std::string& filePath)
    : m_filePath(filePath) {}

// read() 

std::vector<Person> PersonCsvReader::read() {
    std::ifstream in(m_filePath);
    if (!in.is_open()) {
        throw std::runtime_error("Could not open CSV file: " + m_filePath);
    }

    std::vector<Person> people;
    std::string line;

    // 1) Read header row
    if (!std::getline(in, line)) {
        // Empty file
        return people;
    }

    std::vector<std::string> headers;
    {
        std::stringstream ss(line);
        std::string col;
        while (std::getline(ss, col, ',')) {
            headers.push_back(trim(col));
        }
    }

    auto indexOf = [&](const std::string& name) -> int {
        for (int i = 0; i < static_cast<int>(headers.size()); ++i) {
            if (headers[i] == name) return i;
        }
        return -1;
    };

    int idxId              = indexOf("id");
    int idxGradYear        = indexOf("graduationYear");
    int idxRegion          = indexOf("region");
    int idxPrimaryOs       = indexOf("primaryOS");
    int idxEngFocus        = indexOf("engineeringFocus");
    int idxStudyTime       = indexOf("studyTime");
    int idxCourseLoad      = indexOf("courseLoad");
    int idxFavoriteColors  = indexOf("favoriteColors");
    int idxHobbies         = indexOf("hobbies");
    int idxLanguages       = indexOf("languages");

    // Required columns check
    if (idxId == -1 || idxGradYear == -1 || idxRegion == -1 ||
        idxPrimaryOs == -1 || idxEngFocus == -1 || idxStudyTime == -1 ||
        idxCourseLoad == -1) {
        throw std::runtime_error("CSV missing one or more required columns.");
    }

    // 2) Read data rows
    while (std::getline(in, line)) {
        if (line.empty()) continue;

        std::vector<std::string> cells;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            cells.push_back(trim(cell));
        }

        if (static_cast<int>(cells.size()) < static_cast<int>(headers.size())) {
            // Malformed row; skip or handle as needed
            continue;
        }

        // scalar fields

        std::string id = cells[idxId];

        int graduationYear = 0;
        if (!cells[idxGradYear].empty()) {
            graduationYear = std::stoi(cells[idxGradYear]);
        }

        int courseLoad = 0;
        if (!cells[idxCourseLoad].empty()) {
            courseLoad = std::stoi(cells[idxCourseLoad]);
        }

        // enums using PersonEnums helpers

        Region region = Region::Unknown;
        if (!cells[idxRegion].empty()) {
            region = parse_region(cells[idxRegion]);
        }

        PrimaryOS primaryOS = PrimaryOS::Unknown;
        if (!cells[idxPrimaryOs].empty()) {
            primaryOS = parse_primary_os(cells[idxPrimaryOs]);
        }

        EngineeringFocus engineeringFocus = EngineeringFocus::Unknown;
        if (!cells[idxEngFocus].empty()) {
            engineeringFocus = parse_engineering_focus(cells[idxEngFocus]);
        }

        StudyTime studyTime = StudyTime::Unknown;
        if (!cells[idxStudyTime].empty()) {
            studyTime = parse_study_time(cells[idxStudyTime]);
        }

        // multi-value hyphen-separated sets (colors, hobbies, languages)

        std::unordered_set<std::string> favoriteColors;
        if (idxFavoriteColors != -1 && !cells[idxFavoriteColors].empty()) {
            favoriteColors = splitHyphenSeparated(cells[idxFavoriteColors]);
        }

        std::unordered_set<std::string> hobbies;
        if (idxHobbies != -1 && !cells[idxHobbies].empty()) {
            hobbies = splitHyphenSeparated(cells[idxHobbies]);
        }

        std::unordered_set<std::string> languages;
        if (idxLanguages != -1 && !cells[idxLanguages].empty()) {
            languages = splitHyphenSeparated(cells[idxLanguages]);
        }

        // construct immutable Person 

        Person person(
            id,
            graduationYear,
            region,
            primaryOS,
            engineeringFocus,
            studyTime,
            courseLoad,
            favoriteColors,
            hobbies,
            languages
        );

        people.push_back(std::move(person));
    }

    return people;
}
