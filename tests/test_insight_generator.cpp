#include <gtest/gtest.h>

#include "InsightGenerator.h"
#include "Person.h"
#include "PersonEnums.h"

#include <string>
#include <unordered_set>
#include <vector>

TEST(InsightGeneratorTest, GeneratesOsToStudyTimeInsight) {
    std::vector<Person> persons;

    const std::unordered_set<std::string> noColors;
    const std::unordered_set<std::string> noHobbies;
    const std::unordered_set<std::string> noLanguages;

    // 4 Windows users: 3 Night, 1 Morning → 3/4 = 0.75 >= 0.5 threshold
    persons.emplace_back(
        "w_n1",
        2025,
        Region::US_Northeast,
        PrimaryOS::Windows,
        EngineeringFocus::Neural_Engineering,
        StudyTime::Night,
        4,
        noColors,
        noHobbies,
        noLanguages
    );

    persons.emplace_back(
        "w_n2",
        2025,
        Region::US_Northeast,
        PrimaryOS::Windows,
        EngineeringFocus::Neural_Engineering,
        StudyTime::Night,
        4,
        noColors,
        noHobbies,
        noLanguages
    );

    persons.emplace_back(
        "w_n3",
        2025,
        Region::US_Northeast,
        PrimaryOS::Windows,
        EngineeringFocus::Neural_Engineering,
        StudyTime::Night,
        4,
        noColors,
        noHobbies,
        noLanguages
    );

    persons.emplace_back(
        "w_m",
        2025,
        Region::US_Northeast,
        PrimaryOS::Windows,
        EngineeringFocus::Neural_Engineering,
        StudyTime::Morning,
        4,
        noColors,
        noHobbies,
        noLanguages
    );

    // some non-Windows users
    persons.emplace_back(
        "m_n",
        2025,
        Region::US_Northeast,
        PrimaryOS::MacOS,
        EngineeringFocus::Neural_Engineering,
        StudyTime::Night,
        4,
        noColors,
        noHobbies,
        noLanguages
    );

    InsightGenerator gen;
    std::unordered_set<std::string> suppressed;

    auto insights = gen.generate(persons, suppressed);

    ASSERT_FALSE(insights.empty()) << "Expected at least one OS → StudyTime insight";

    const std::string expectedKey =
        "primary_os = " + to_string(PrimaryOS::Windows) +
        " -> study_time = " + to_string(StudyTime::Night);

    const std::string expectedDescription =
        "People whose primary OS is Windows tend to study in the nights.";

    const Insight* found = nullptr;
    for (const auto& ins : insights) {
        if (ins.key == expectedKey) {
            found = &ins;
            break;
        }
    }

    ASSERT_NE(found, nullptr) << "Did not find expected Windows→Night insight";

    EXPECT_EQ(found->key, expectedKey);
    EXPECT_EQ(found->description, expectedDescription);
    EXPECT_EQ(found->support, 3u);
    EXPECT_EQ(found->population, 4u);
}

TEST(InsightGeneratorTest, RespectsSuppressedKeys) {
    std::vector<Person> persons;

    const std::unordered_set<std::string> noColors;
    const std::unordered_set<std::string> noHobbies;
    const std::unordered_set<std::string> noLanguages;

    persons.emplace_back(
        "w_n1",
        2025,
        Region::US_Northeast,
        PrimaryOS::Windows,
        EngineeringFocus::Neural_Engineering,
        StudyTime::Night,
        4,
        noColors,
        noHobbies,
        noLanguages
    );

    persons.emplace_back(
        "w_n2",
        2025,
        Region::US_Northeast,
        PrimaryOS::Windows,
        EngineeringFocus::Neural_Engineering,
        StudyTime::Night,
        4,
        noColors,
        noHobbies,
        noLanguages
    );

    persons.emplace_back(
        "w_n3",
        2025,
        Region::US_Northeast,
        PrimaryOS::Windows,
        EngineeringFocus::Neural_Engineering,
        StudyTime::Night,
        4,
        noColors,
        noHobbies,
        noLanguages
    );

    persons.emplace_back(
        "w_m",
        2025,
        Region::US_Northeast,
        PrimaryOS::Windows,
        EngineeringFocus::Neural_Engineering,
        StudyTime::Morning,
        4,
        noColors,
        noHobbies,
        noLanguages
    );

    InsightGenerator gen;

    const std::string suppressedKey =
        "primary_os = " + to_string(PrimaryOS::Windows) +
        " -> study_time = " + to_string(StudyTime::Night);

    std::unordered_set<std::string> suppressed = { suppressedKey };

    auto insights = gen.generate(persons, suppressed);

    for (const auto& ins : insights) {
        EXPECT_NE(ins.key, suppressedKey)
            << "Suppressed key still appeared in generated insights";
    }
}
