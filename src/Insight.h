#ifndef INSIGHT_H
#define INSIGHT_H

#include <cstddef>
#include <string>

/**
 * Represents a single generated insight in natural language (not full english sentence).
 * The key uniquely identifies the insight so it can be tracked
 * across multiple generation runs for the same dataset.
 */
struct Insight {
    std::string key;
    std::string description;
    int score = 0;                // 0-100 quality score
    std::size_t support = 0;      // number of matching records / the number of people such that both details/attributes holds true for

    std::size_t population = 0;   // size of the cohort evaluated

    double confidence() const {
        if (population == 0) {
            return 0.0;
        }
        return static_cast<double>(support) / static_cast<double>(population);
    }
};

#endif // INSIGHT_H

