#ifndef INSIGHT_GENERATOR_H
#define INSIGHT_GENERATOR_H

#include "Insight.h"
#include "Person.h"

#include <string>
#include <unordered_set>
#include <vector>

/**
 * Produces English language insights from a collection of Person records/dataset.
 // generates the highest/most confident x -> y insight
 */

enum class InsightPairType {
    OsStudy,
    ColorHobby,
    RegionLanguage,
    FocusCourse
};

class InsightGenerator {
public:
    std::vector<Insight> generate(
        const std::vector<Person>& persons,
        const std::unordered_set<std::string>& suppressedKeys) const;

    std::vector<Insight> generatePair(
        const std::vector<Person>& persons,
        const std::unordered_set<std::string>& suppressedKeys,
        InsightPairType which) const;

    // insights for any combination
    std::vector<Insight> generateGeneric(
        const std::vector<Person>& persons,
        const std::unordered_set<std::string>& suppressedKeys,
        const std::string& attrX,
        const std::string& attrY) const;

private:
    // suppressed keys will be the insights that user rejects; they get added to a csv file we will create and these functions will check
    // over that file so it doesn't display an insight the user has already rejected
    std::vector<Insight> generatePrimaryOsToStudyTime(
        const std::vector<Person>& persons,
        const std::unordered_set<std::string>& suppressedKeys) const;

    std::vector<Insight> generateFavoriteColorToHobby(
        const std::vector<Person>& persons,
        const std::unordered_set<std::string>& suppressedKeys) const;

    std::vector<Insight> generateRegionToLanguage(
        const std::vector<Person>& persons,
        const std::unordered_set<std::string>& supressedKeys) const;

    std::vector<Insight> generateEngineeringFocusToCourseLoad(
        const std::vector<Person>& persons,
        const std::unordered_set<std::string>& supressedKeys) const;

    // scores the insight from 0-100 considering how strong it is from within the group and overall
    static int scoreFromCounts(std::size_t support,
                               std::size_t cohortSize,
                               std::size_t globalPopulation);

    // not necessary but a nice helper for natural sounding/looking english sentences
    static std::string describeStudyTime(StudyTime studyTime);
};

#endif // INSIGHT_GENERATOR_H