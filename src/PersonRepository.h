/*
Subtasks: vector storage, replace Person by index, return all persons.
Purpose: Hold and manage the currently loaded dataset.
Work:
Store std::vector.
Functions to addPerson, updatePerson(index, Person), removePerson(index).
Return const reference to all persons for reading.
*/

#ifndef PERSONREPOSITORY_H
#define PERSONREPOSITORY_H

#include "Person.h"
#include <vector>
#include <cstddef> // for std::size_t

/**
 * PersonRepository
 *
 * Holds and manages the currently loaded dataset of Person objects.
 * Supports add, update (replace), and remove by index.
 */

class PersonRepository {
public:
    PersonRepository() = default;

    // Initialize or replace the whole dataset at once
    void setPersons(const std::vector<Person>& persons);
    void setPersons(std::vector<Person>&& persons); // move overload (optional but nice)

    // Read only access to all persons
    const std::vector<Person>& getAll() const;

    //  helpers
    std::size_t size() const;
    const Person& get(std::size_t index) const;

    // Mutators
    void addPerson(const Person& person);
    void updatePerson(std::size_t index, const Person& person); // replace at index
    void removePerson(std::size_t index);

    //Save dataset to the csv
    bool saveToCsv(const std::string& filePath) const;

private:
    std::vector<Person> m_persons;
};

#endif // PERSONREPOSITORY_H
