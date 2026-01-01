// tests/test_person_repository.cpp

#include <gtest/gtest.h>
#include <string>
#include <unordered_set>
#include <cstdio>      

#include "PersonRepository.h"
#include "PersonCsvReader.h"
#include "Person.h"
#include "PersonEnums.h"

TEST(PersonRepositoryTest, SaveToCsvAndReloadWithCsvReader) {
    //Build a small dataset
    std::vector<Person> original;

    original.emplace_back(
        "p1",
        2027,
        Region::US_Northeast,
        PrimaryOS::MacOS,
        EngineeringFocus::Neural_Engineering,
        StudyTime::Night,
        5,
        std::unordered_set<std::string>{"blue", "green"},
        std::unordered_set<std::string>{"reading", "running"},
        std::unordered_set<std::string>{"english"}
    );

    original.emplace_back(
        "p2",
        2026,
        Region::US_West,
        PrimaryOS::Windows,
        EngineeringFocus::Electronics,
        StudyTime::Afternoon,
        4,
        std::unordered_set<std::string>{"red"},
        std::unordered_set<std::string>{"gaming"},
        std::unordered_set<std::string>{"english", "spanish"}
    );

    //Put them into a repository
    PersonRepository repo;
    repo.setPersons(original);

    //Save to CSV
    const std::string path = "test_people.csv";
    bool ok = repo.saveToCsv(path);
    ASSERT_TRUE(ok) << "saveToCsv should return true when file opens successfully";

    //Use PersonCsvReader to read the CSV back into a new vector<Person>
    PersonCsvReader reader(path);
    std::vector<Person> reloaded = reader.read();

    ASSERT_EQ(reloaded.size(), original.size())
        << "Reloaded dataset should have same number of people";

    //Compare basic scalar fields (ignoring potential unordered_set ordering)
    for (std::size_t i = 0; i < original.size(); ++i) {
        const Person& a = original[i];
        const Person& b = reloaded[i];

        EXPECT_EQ(a.getId(), b.getId());
        EXPECT_EQ(a.getGraduationYear(), b.getGraduationYear());
        EXPECT_EQ(a.getRegion(), b.getRegion());
        EXPECT_EQ(a.getPrimaryOS(), b.getPrimaryOS());
        EXPECT_EQ(a.getEngineeringFocus(), b.getEngineeringFocus());
        EXPECT_EQ(a.getStudyTime(), b.getStudyTime());
        EXPECT_EQ(a.getCourseLoad(), b.getCourseLoad());

        // sets: compare as sets (order-independent)
        EXPECT_EQ(a.getFavoriteColors(), b.getFavoriteColors());
        EXPECT_EQ(a.getHobbies(),        b.getHobbies());
        EXPECT_EQ(a.getLanguages(),      b.getLanguages());
    }

    std::remove(path.c_str());
}

TEST(PersonRepositoryTest, SaveToCsvHandlesEmptyRepository) {
    PersonRepository repo;  // empty

    const std::string path = "empty_people.csv";
    bool ok = repo.saveToCsv(path);
    ASSERT_TRUE(ok);

    //Read it back: should either be empty or just header row, reader returns 0 people
    PersonCsvReader reader(path);
    std::vector<Person> reloaded = reader.read();
    EXPECT_TRUE(reloaded.empty());

    std::remove(path.c_str());
}