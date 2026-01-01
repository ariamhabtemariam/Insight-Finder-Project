#include "InsightGenerator.h"

#include "PersonEnums.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>

namespace {
constexpr std::size_t MIN_OS_SUPPORT = 3; //at least 3 people in the set need to exist
constexpr double MIN_OS_CONFIDENCE = 0.50; //at least 50% of the OS cohort need to study at specific time

constexpr std::size_t MIN_COLOR_SUPPORT = 2; //we added our colors, so only 2 ppl existing should be fine
constexpr double MIN_COLOR_CONFIDENCE = 0.50; // at least 50% of people w this color like that hobby

constexpr std::size_t MIN_REGION_SUPPORT = 3;
constexpr double MIN_REGION_CONFIDENCE = 0.50;

constexpr std::size_t MIN_FOCUS_SUPPORT = 3;
constexpr double MIN_FOCUS_CONFIDENCE = 0.50;

std::string lowercase(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}
} // namespace

std::vector<Insight> InsightGenerator::generate(
    const std::vector<Person>& persons,
    const std::unordered_set<std::string>& suppressedKeys) const {
    std::vector<Insight> insights;

    auto osInsights = generatePrimaryOsToStudyTime(persons, suppressedKeys);
    insights.insert(insights.end(), osInsights.begin(), osInsights.end());

    auto colorInsights = generateFavoriteColorToHobby(persons, suppressedKeys);
    insights.insert(insights.end(), colorInsights.begin(), colorInsights.end());

    auto regionInsights = generateRegionToLanguage(persons, suppressedKeys);
    insights.insert(insights.end(), regionInsights.begin(), regionInsights.end());

    auto focusInsights = generateEngineeringFocusToCourseLoad(persons, suppressedKeys);
    insights.insert(insights.end(), focusInsights.begin(), focusInsights.end());

    std::sort(insights.begin(), insights.end(), [](const Insight& lhs, const Insight& rhs) {
        if (lhs.score != rhs.score) {
            return lhs.score > rhs.score; //insights generated in descending order by score
        }
        if (lhs.support != rhs.support) {
            return lhs.support > rhs.support; //if they have the same score, then return the one with higher support first
        }
        return lhs.description < rhs.description;
    });

    return insights;
}

std::vector<Insight> InsightGenerator::generatePair(
    const std::vector<Person>& persons,
    const std::unordered_set<std::string>& suppressedKeys,
    InsightPairType which) const
{
    switch (which) {
        case InsightPairType::OsStudy:
            return generatePrimaryOsToStudyTime(persons, suppressedKeys);

        case InsightPairType::ColorHobby:
            return generateFavoriteColorToHobby(persons, suppressedKeys);

        case InsightPairType::RegionLanguage:
            return generateRegionToLanguage(persons, suppressedKeys);

        case InsightPairType::FocusCourse:
            return generateEngineeringFocusToCourseLoad(persons, suppressedKeys);
    }

    return {};
}


//eligible population counts the number of people that can even be considered; their attributes for aren't empty

std::vector<Insight> InsightGenerator::generatePrimaryOsToStudyTime(
    const std::vector<Person>& persons,
    const std::unordered_set<std::string>& suppressedKeys) const {
    struct Distribution {
        std::size_t cohortSize = 0;
        std::map<StudyTime, std::size_t> studyCounts;
    };

    std::map<PrimaryOS, Distribution> osDistributions;
    std::size_t eligiblePopulation = 0; // number of people who have knwon OS and known study time

    for (const Person& person : persons) {
        PrimaryOS os = person.getPrimaryOS();
        StudyTime study = person.getStudyTime();
        if (os == PrimaryOS::Unknown || study == StudyTime::Unknown) { //uses 'unknown' because os and study times are enums
            continue;
        }

        eligiblePopulation++;
        Distribution& dist = osDistributions[os];
        dist.cohortSize++;
        dist.studyCounts[study]++;
    }

    std::vector<Insight> insights;
    if (eligiblePopulation == 0) {
        return insights;
    }

    for (const auto& [os, dist] : osDistributions) {
        if (dist.cohortSize < MIN_OS_SUPPORT) {
            continue;
        }

        auto best = std::max_element(
            dist.studyCounts.begin(), dist.studyCounts.end(),
            [](const auto& lhs, const auto& rhs) { return lhs.second < rhs.second; });

        if (best == dist.studyCounts.end()) {
            continue;
        }

        std::size_t support = best->second;
        double confidence = static_cast<double>(support) / static_cast<double>(dist.cohortSize);
        if (confidence < MIN_OS_CONFIDENCE) {
            continue;
        }

        std::string key = "primary_os = " + to_string(os) + " -> study_time = " + to_string(best->first);
        if (suppressedKeys.count(key) > 0) {
            continue;
        }

        Insight insight;
        insight.key = key;
        insight.support = support;
        insight.population = dist.cohortSize;
        insight.score = scoreFromCounts(support, dist.cohortSize, eligiblePopulation);

        std::ostringstream sentence;
        sentence << "People whose primary OS is " << to_string(os)
                 << " tend to study in the " << describeStudyTime(best->first) << ".";
        insight.description = sentence.str();

        insights.push_back(std::move(insight));
    }

    return insights;
}

std::vector<Insight> InsightGenerator::generateFavoriteColorToHobby(
    const std::vector<Person>& persons,
    const std::unordered_set<std::string>& suppressedKeys) const {
    struct ColorDistribution {
        std::size_t cohortSize = 0;
        std::unordered_map<std::string, std::size_t> hobbyCounts;
    };

    std::unordered_map<std::string, ColorDistribution> colorDistributions;
    std::size_t eligiblePopulation = 0;

    for (const Person& person : persons) {
        const auto& colors = person.getFavoriteColors();
        const auto& hobbies = person.getHobbies();
        if (colors.empty() || hobbies.empty()) { //uses .empty() because colors and hobbies are sets
            continue;
        }

        eligiblePopulation++;
        for (const std::string& color : colors) {
            ColorDistribution& dist = colorDistributions[color];
            dist.cohortSize++;
            for (const std::string& hobby : hobbies) {
                dist.hobbyCounts[hobby]++;
            }
        }
    }

    std::vector<Insight> insights;
    if (eligiblePopulation == 0) {
        return insights;
    }

    for (auto& [color, dist] : colorDistributions) {
        if (dist.cohortSize < MIN_COLOR_SUPPORT) {
            continue;
        }

        auto best = std::max_element(
            dist.hobbyCounts.begin(), dist.hobbyCounts.end(),
            [](const auto& lhs, const auto& rhs) { return lhs.second < rhs.second; });

        if (best == dist.hobbyCounts.end()) {
            continue;
        }

        std::size_t support = best->second;
        double confidence = static_cast<double>(support) / static_cast<double>(dist.cohortSize);
        if (confidence < MIN_COLOR_CONFIDENCE) {
            continue;
        }

        std::string key = "favorite_color = " + lowercase(color) + " -> hobby = " + lowercase(best->first);
        if (suppressedKeys.count(key) > 0) {
            continue;
        }

        Insight insight;
        insight.key = key;
        insight.support = support;
        insight.population = dist.cohortSize;
        insight.score = scoreFromCounts(support, dist.cohortSize, eligiblePopulation);

        std::ostringstream sentence;
        sentence << "People whose favorite color is " << color
                 << " tend to have a hobby of " << best->first << ".";
        insight.description = sentence.str();

        insights.push_back(std::move(insight));
    }

    return insights;
}


std::vector<Insight> InsightGenerator::generateRegionToLanguage(
    const std::vector<Person>& persons,
    const std::unordered_set<std::string>& suppressedKeys) const {
    struct RegionLangDistribution {
        std::size_t cohortSize = 0;                         // # people in this region (with known languages)
        std::unordered_map<std::string, std::size_t> languageCounts;
    };

    std::map<Region, RegionLangDistribution> regionDistributions;
    std::size_t eligiblePopulation = 0; // people with known region AND at least one language

    for (const Person& person : persons) {
        Region region = person.getRegion();
        const auto& languages = person.getLanguages();

        if (region == Region::Unknown || languages.empty()) { //region is an enum while langues are a set
            continue;
        }

        eligiblePopulation++;
        RegionLangDistribution& dist = regionDistributions[region];
        dist.cohortSize++;

        for (const std::string& lang : languages) {
            dist.languageCounts[lang]++;
        }
    }

    std::vector<Insight> insights;
    if (eligiblePopulation == 0) {
        return insights;
    }

    for (auto& [region, dist] : regionDistributions) {
        if (dist.cohortSize < MIN_REGION_SUPPORT) {
            continue;
        }

        auto best = std::max_element(
            dist.languageCounts.begin(), dist.languageCounts.end(),
            [](const auto& lhs, const auto& rhs) { return lhs.second < rhs.second; });

        if (best == dist.languageCounts.end()) {
            continue;
        }

        std::size_t support = best->second;
        double confidence = static_cast<double>(support) / static_cast<double>(dist.cohortSize);
        if (confidence < MIN_REGION_CONFIDENCE) {
            continue;
        }

        std::string key = "region = " + to_string(region) +
                          " -> language = " + lowercase(best->first);
        if (suppressedKeys.count(key) > 0) {
            continue;
        }

        Insight insight;
        insight.key = key;
        insight.support = support;
        insight.population = dist.cohortSize;
        insight.score = scoreFromCounts(support, dist.cohortSize, eligiblePopulation);

        std::ostringstream sentence;
        sentence << "People from " << to_string(region)
                 << " tend to speak " << best->first << ".";
        insight.description = sentence.str();

        insights.push_back(std::move(insight));
    }

    return insights;
}

std::vector<Insight> InsightGenerator::generateEngineeringFocusToCourseLoad(
    const std::vector<Person>& persons,
    const std::unordered_set<std::string>& suppressedKeys) const {
    struct FocusLoadDistribution {
        std::size_t cohortSize = 0;             // # people with this focus and valid courseLoad
        std::map<int, std::size_t> loadCounts;  // courseLoad value -> count
    };

    std::map<EngineeringFocus, FocusLoadDistribution> focusDistributions;
    std::size_t eligiblePopulation = 0; // people with known focus and positive courseLoad

    for (const Person& person : persons) {
        EngineeringFocus focus = person.getEngineeringFocus();
        int load = person.getCourseLoad();

        if (focus == EngineeringFocus::Unknown || load <= 0) { //eng focuses are enums while load is a number
            continue;
        }

        eligiblePopulation++;
        FocusLoadDistribution& dist = focusDistributions[focus];
        dist.cohortSize++;
        dist.loadCounts[load]++;
    }

    std::vector<Insight> insights;
    if (eligiblePopulation == 0) {
        return insights;
    }

    for (const auto& [focus, dist] : focusDistributions) {
        if (dist.cohortSize < MIN_FOCUS_SUPPORT) {
            continue;
        }

        auto best = std::max_element(
            dist.loadCounts.begin(), dist.loadCounts.end(),
            [](const auto& lhs, const auto& rhs) { return lhs.second < rhs.second; });

        if (best == dist.loadCounts.end()) {
            continue;
        }

        std::size_t support = best->second;
        double confidence = static_cast<double>(support) / static_cast<double>(dist.cohortSize);
        if (confidence < MIN_FOCUS_CONFIDENCE) {
            continue;
        }

        std::string key = "engineering_focus = " + to_string(focus) +
                          " -> course_load = " + std::to_string(best->first);
        if (suppressedKeys.count(key) > 0) {
            continue;
        }

        Insight insight;
        insight.key = key;
        insight.support = support;
        insight.population = dist.cohortSize;
        insight.score = scoreFromCounts(support, dist.cohortSize, eligiblePopulation);

        std::ostringstream sentence;
        sentence << "People whose engineering focus is " << to_string(focus)
                 << " tend to take about " << best->first << " courses.";
        insight.description = sentence.str();

        insights.push_back(std::move(insight));
    }

    return insights;
}


int InsightGenerator::scoreFromCounts(std::size_t support,
                                      std::size_t cohortSize,
                                      std::size_t globalPopulation) {
    if (support == 0 || cohortSize == 0 || globalPopulation == 0) {
        return 0;
    }

    double confidence = static_cast<double>(support) / static_cast<double>(cohortSize);
    double coverage = static_cast<double>(support) / static_cast<double>(globalPopulation);
    double rawScore = (confidence * 0.7 + coverage * 0.3) * 100.0;
    if (rawScore < 0.0) {
        rawScore = 0.0;
    } else if (rawScore > 100.0) {
        rawScore = 100.0;
    }

    return static_cast<int>(std::round(rawScore));
}

std::string InsightGenerator::describeStudyTime(StudyTime studyTime) {
    switch (studyTime) {
        case StudyTime::Morning:
            return "mornings";
        case StudyTime::Afternoon:
            return "afternoons";
        case StudyTime::Night:
            return "nights";
        default:
            return "day";
    }
}





//supports any topic combination

namespace {
    // helper that makes sure it's all lowercase and consistent
    std::string normalizeAttrName(const std::string& attr) {
        std::string normalized = lowercase(attr);
        
        // mapping synonyms to the main names to prevent derrors
        if (normalized == "os" || normalized == "primary_os" || normalized == "primaryos")
            return "os";
        if (normalized == "study" || normalized == "studytime" || normalized == "study_time")
            return "study";
        if (normalized == "color" || normalized == "favoritecolor" || normalized == "favourite_color" || normalized == "favoritecolors")
            return "color";
        if (normalized == "hobby" || normalized == "hobbies")
            return "hobby";
        if (normalized == "region" || normalized == "area")
            return "region";
        if (normalized == "language" || normalized == "lang" || normalized == "languages")
            return "language";
        if (normalized == "focus" || normalized == "major" || normalized == "engineering" || normalized == "engfocus" || normalized == "engineeringfocus")
            return "focus";
        if (normalized == "course" || normalized == "courseload" || normalized == "load" || normalized == "courses")
            return "course";
        if (normalized == "graduation" || normalized == "gradyear" || normalized == "year")
            return "graduation";
        
        return normalized;
    }

    // gets the values from a Person
    std::vector<std::string> extractAttributeValues(const Person& person, const std::string& attrName) {
        std::string attr = normalizeAttrName(attrName);
        
        if (attr == "os") {
            PrimaryOS os = person.getPrimaryOS();
            if (os == PrimaryOS::Unknown) return {};
            return {to_string(os)};
        }
        else if (attr == "study") {
            StudyTime st = person.getStudyTime();
            if (st == StudyTime::Unknown) return {};
            return {to_string(st)};
        }
        else if (attr == "color") {
            const auto& colors = person.getFavoriteColors();
            return std::vector<std::string>(colors.begin(), colors.end());
        }
        else if (attr == "hobby") {
            const auto& hobbies = person.getHobbies();
            return std::vector<std::string>(hobbies.begin(), hobbies.end());
        }
        else if (attr == "region") {
            Region r = person.getRegion();
            if (r == Region::Unknown) return {};
            return {to_string(r)};
        }
        else if (attr == "language") {
            const auto& langs = person.getLanguages();
            return std::vector<std::string>(langs.begin(), langs.end());
        }
        else if (attr == "focus") {
            EngineeringFocus f = person.getEngineeringFocus();
            if (f == EngineeringFocus::Unknown) return {};
            return {to_string(f)};
        }
        else if (attr == "course") {
            int load = person.getCourseLoad();
            if (load <= 0) return {};
            return {std::to_string(load)};
        }
        else if (attr == "graduation") {
            int year = person.getGraduationYear();
            if (year <= 0) return {};
            return {std::to_string(year)};
        }
        
        return {};
    }

    // what gets shown to the user
    std::string getAttributeDisplayName(const std::string& attrName) {
        std::string attr = normalizeAttrName(attrName);
        
        if (attr == "os") return "primary OS";
        if (attr == "study") return "study time";
        if (attr == "color") return "favorite color";
        if (attr == "hobby") return "hobby";
        if (attr == "region") return "region";
        if (attr == "language") return "language";
        if (attr == "focus") return "engineering focus";
        if (attr == "course") return "course load";
        if (attr == "graduation") return "graduation year";
        
        return attrName;
    }

    constexpr std::size_t MIN_GENERIC_SUPPORT = 2;
    constexpr double MIN_GENERIC_CONFIDENCE = 0.50; //loser bounds but some insights are still under 50 
}

std::vector<Insight> InsightGenerator::generateGeneric(
    const std::vector<Person>& persons,
    const std::unordered_set<std::string>& suppressedKeys,
    const std::string& attrX,
    const std::string& attrY) const {
    
    //for each X  count occurrences of Y 
    struct Distribution {
        std::size_t cohortSize = 0;
        std::unordered_map<std::string, std::size_t> yCounts;
    };
    
    std::unordered_map<std::string, Distribution> distributions;
    std::size_t eligiblePopulation = 0;
    
    // Nnames for x and y
    std::string normX = normalizeAttrName(attrX);
    std::string normY = normalizeAttrName(attrY);
    



    // goes thriough csv and build distributions
    for (const Person& person : persons) {
        std::vector<std::string> xValues = extractAttributeValues(person, normX);
        std::vector<std::string> yValues = extractAttributeValues(person, normY);
        
        if (xValues.empty() || yValues.empty()) {
            continue;
        }
        
        eligiblePopulation++;
        
        //for each X this person has
        for (const std::string& xVal : xValues) {
            Distribution& dist = distributions[xVal];
            dist.cohortSize++;
            
            // count each Y 
            for (const std::string& yVal : yValues) {
                dist.yCounts[yVal]++;
            }
        }
    }
    
    std::vector<Insight> insights;
    
    if (eligiblePopulation == 0) {
        return insights;
    }
    
    // generates insights for stornger patterns/relationships
    for (const auto& [xValue, dist] : distributions) {
        if (dist.cohortSize < MIN_GENERIC_SUPPORT) {
            continue;
        }
        
        // finds most common Y value for this X
        auto best = std::max_element(
            dist.yCounts.begin(), dist.yCounts.end(),
            [](const auto& lhs, const auto& rhs) { return lhs.second < rhs.second; });
        
        if (best == dist.yCounts.end()) {
            continue;
        }
        
        std::size_t support = best->second;
        double confidence = static_cast<double>(support) / static_cast<double>(dist.cohortSize);
        
        if (confidence < MIN_GENERIC_CONFIDENCE) {
            continue;
        }
        
        // generatea key
        std::string key = normX + " = " + lowercase(xValue) + " -> " + normY + " = " + lowercase(best->first);
        
        if (suppressedKeys.count(key) > 0) {
            continue;
        }
        
        // creates insight
        Insight insight;
        insight.key = key;
        insight.support = support;
        insight.population = dist.cohortSize;
        insight.score = scoreFromCounts(support, dist.cohortSize, eligiblePopulation);
        
        // user facing description
        std::ostringstream sentence;
        sentence << "People whose " << getAttributeDisplayName(normX) 
                 << " is " << xValue
                 << " tend to have " << getAttributeDisplayName(normY)
                 << " of " << best->first << ".";
        insight.description = sentence.str();
        
        insights.push_back(std::move(insight));
    }
    
    // orders insight output by score
    std::sort(insights.begin(), insights.end(), [](const Insight& lhs, const Insight& rhs) {
        if (lhs.score != rhs.score) {
            return lhs.score > rhs.score;
        }
        if (lhs.support != rhs.support) {
            return lhs.support > rhs.support;
        }
        return lhs.description < rhs.description;
    });
    
    return insights;
}

