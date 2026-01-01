#include "PersonRepository.h"
#include "PersonEnums.h"   
#include <stdexcept>
#include <fstream>         
#include <unordered_set>   

// Initialize / replace whole dataset (copy)
void PersonRepository::setPersons(const std::vector<Person>& persons) {
    m_persons = persons;
}

// Initialize / replace whole dataset (move)
void PersonRepository::setPersons(std::vector<Person>&& persons) {
    m_persons = std::move(persons);
}

// Read only access to all persons
const std::vector<Person>& PersonRepository::getAll() const {
    return m_persons;
}

std::size_t PersonRepository::size() const {
    return m_persons.size();
}

const Person& PersonRepository::get(std::size_t index) const {
    if (index >= m_persons.size()) {
        throw std::out_of_range("PersonRepository::get - index out of range");
    }
    return m_persons[index];
}

// Mutators

void PersonRepository::addPerson(const Person& person) {
    m_persons.push_back(person);
}

void PersonRepository::updatePerson(std::size_t index, const Person& person) {
    if (index >= m_persons.size()) {
        throw std::out_of_range("PersonRepository::updatePerson - index out of range");
    }
    // Person is immutable so entire object is replaced
    m_persons[index] = person;
}

void PersonRepository::removePerson(std::size_t index) {
    if (index >= m_persons.size()) {
        throw std::out_of_range("PersonRepository::removePerson - index out of range");
    }
    m_persons.erase(m_persons.begin() + static_cast<std::ptrdiff_t>(index));
}

// convert an unordered_set<string> to a hyphen-separated string,
static std::string joinSet(const std::unordered_set<std::string>& s) {
    std::string result;
    bool first = true;
    for (const auto& value : s) {
        if (!first) result += "-";   // match class_data_set.csv format
        result += value;
        first = false;
    }
    return result;
}

bool PersonRepository::saveToCsv(const std::string& filePath) const {
    std::ofstream out(filePath);
    if (!out.is_open()) {
        return false; // could also throw, but bool is fine for now
    }

    // Header row has to match what PersonCsvReader expects
    out << "id,graduationYear,region,primaryOS,engineeringFocus,studyTime,courseLoad,"
           "favoriteColors,hobbies,languages\n";

    for (const Person& p : m_persons) {
        out << p.getId() << ','
            << p.getGraduationYear() << ','
            << to_string(p.getRegion()) << ','
            << to_string(p.getPrimaryOS()) << ','
            << to_string(p.getEngineeringFocus()) << ','
            << to_string(p.getStudyTime()) << ','
            << p.getCourseLoad() << ','
            << joinSet(p.getFavoriteColors()) << ','
            << joinSet(p.getHobbies()) << ','
            << joinSet(p.getLanguages()) << '\n';
    }

    return true;
}