/* 
#include <iostream>
#include "PersonCsvReader.h"
#include "PersonRepository.h"

int main() {
    try {
        // Point this to your CSV file (relative or absolute path)
        PersonCsvReader reader("class_data_set.csv");

        // Call the PersonReader interface
        std::vector<Person> people = reader.read();

        std::cout << "Loaded " << people.size() << " people from CSV.\n\n";

        // Print the first few people as a sanity check
        std::size_t maxToPrint = std::min<std::size_t>(people.size(), 5);
        for (std::size_t i = 0; i < maxToPrint; ++i) {
            std::cout << "---- Person " << i << " ----\n";
            std::cout << people[i].toString() << "\n";
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
*/
/*
#include <iostream>
#include <algorithm>    // std::min
#include <cstddef>      // std::size_t

#include "PersonCsvReader.h"
#include "PersonRepository.h"
#include "PersonEnums.h"

// Small helper to print a short summary of a Person
void printPersonSummary(const Person& p, std::size_t index) {
    std::cout << "Person[" << index << "]\n";
    std::cout << "  ID: " << p.getId() << "\n";
    std::cout << "  Graduation Year: " << p.getGraduationYear() << "\n";
    std::cout << "  Region: " << to_string(p.getRegion()) << "\n";
    std::cout << "  Primary OS: " << to_string(p.getPrimaryOS()) << "\n";
    std::cout << "  Engineering Focus: " << to_string(p.getEngineeringFocus()) << "\n";
    std::cout << "  Study Time: " << to_string(p.getStudyTime()) << "\n";
    std::cout << "  Course Load: " << p.getCourseLoad() << "\n";
    std::cout << "-----------------------------\n";
}

int main() {
    try {
        // 1. Load the dataset from CSV using your PersonCsvReader
        std::cout << "Loading CSV dataset...\n";
        PersonCsvReader csvReader("class_data_set.csv");   // adjust path if needed
        std::vector<Person> loaded = csvReader.read();

        std::cout << "Loaded " << loaded.size() << " people from CSV.\n\n";

        // 2. Put that data into the PersonRepository
        PersonRepository repo;
        repo.setPersons(std::move(loaded)); // move into the repository

        // --- CHECK: basic repository state ---
        std::cout << "[CHECK] Repository size after setPersons: " << repo.size() << "\n\n";

        // Print up to first 3 people as a sanity check
        std::size_t nToPrint = std::min<std::size_t>(repo.size(), 3);
        for (std::size_t i = 0; i < nToPrint; ++i) {
            printPersonSummary(repo.get(i), i);
        }

        // 3. Test addPerson
        std::cout << "\n[TEST] addPerson\n";
        std::size_t sizeBeforeAdd = repo.size();

        Person newPerson(
            "test_add_id",
            2030,
            Region::US_Northeast,
            PrimaryOS::MacOS,
            EngineeringFocus::Neural_Engineering,
            StudyTime::Night,
            5,
            {"blue", "green"},
            {"reading", "running"},
            {"english"}
        );

        repo.addPerson(newPerson);

        std::cout << "  Size before add: " << sizeBeforeAdd
                  << ", after add: " << repo.size() << "\n";

        if (repo.size() > sizeBeforeAdd) {
            std::size_t idxNew = repo.size() - 1;
            std::cout << "  Newly added person at index " << idxNew << ":\n";
            printPersonSummary(repo.get(idxNew), idxNew);
        }

        // 4. Test updatePerson (replace person at index 0, if exists)
        std::cout << "\n[TEST] updatePerson (index 0)\n";
        if (repo.size() > 0) {
            const Person& original = repo.get(0);
            std::cout << "  Original person[0] ID: " << original.getId() << "\n";

            Person updatedPerson(
                "updated_id_0",
                original.getGraduationYear(),        // keep some fields same
                original.getRegion(),
                PrimaryOS::Linux,                   // change OS for testing
                original.getEngineeringFocus(),
                StudyTime::Morning,                 // change study time
                original.getCourseLoad(),
                original.getFavoriteColors(),
                original.getHobbies(),
                original.getLanguages()
            );

            repo.updatePerson(0, updatedPerson);

            const Person& afterUpdate = repo.get(0);
            std::cout << "  Updated person[0] ID: " << afterUpdate.getId() << "\n";
            std::cout << "  Updated person[0] Primary OS: "
                      << to_string(afterUpdate.getPrimaryOS()) << "\n";
            std::cout << "  Updated person[0] Study Time: "
                      << to_string(afterUpdate.getStudyTime()) << "\n";
        } else {
            std::cout << "  Repository is empty, cannot test updatePerson.\n";
        }

        // 5. Test removePerson (remove index 1, if possible)
        std::cout << "\n[TEST] removePerson (index 1)\n";
        if (repo.size() > 1) {
            std::size_t sizeBeforeRemove = repo.size();
            std::string idToRemove = repo.get(1).getId();

            std::cout << "  Removing person[1] with ID: " << idToRemove << "\n";
            repo.removePerson(1);

            std::cout << "  Size before remove: " << sizeBeforeRemove
                      << ", after remove: " << repo.size() << "\n";

            // Quick check: print first 3 again (or fewer if smaller)
            nToPrint = std::min<std::size_t>(repo.size(), 3);
            for (std::size_t i = 0; i < nToPrint; ++i) {
                printPersonSummary(repo.get(i), i);
            }
            printPersonSummary(repo.get(45), 45);
        } else {
            std::cout << "  Not enough people to test removePerson (need at least 2).\n";
        }

        std::cout << "\nAll repository tests completed.\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "\n*** ERROR: " << ex.what() << " ***\n";
        return 1;
    }

    return 0;
}
*/


/*
#include <iostream>
#include <vector>
#include <unordered_set>

#include "Person.h"
#include "PersonEnums.h"
#include "Insight.h"
#include "InsightGenerator.h"

void printInsight(const Insight& ins, std::size_t index) {
    std::cout << "Insight[" << index << "]\n";
    std::cout << "  key:         " << ins.key << "\n";
    std::cout << "  description: " << ins.description << "\n";
    std::cout << "  score:       " << ins.score << "\n";
    std::cout << "  support:     " << ins.support << "\n";
    std::cout << "  population:  " << ins.population << "\n";
    std::cout << "  confidence:  " << ins.confidence() << "\n";
    std::cout << "-------------------------\n";
}

int main() {
    // 1) Build a tiny test dataset by hand
    std::vector<Person> persons;

    // We want a strong pattern:
    // - Most Mac users study in the morning
    // - Most "blue" fans like reading

    persons.emplace_back(
        "p1",
        2027,
        Region::US_Northeast,
        PrimaryOS::MacOS,
        EngineeringFocus::Neural_Engineering,
        StudyTime::Morning,
        5,
        std::unordered_set<std::string>{"blue"},
        std::unordered_set<std::string>{"reading"},
        std::unordered_set<std::string>{"english"}
    );

    persons.emplace_back(
        "p2",
        2026,
        Region::US_Northeast,
        PrimaryOS::MacOS,
        EngineeringFocus::Bioinformatics,
        StudyTime::Morning,
        4,
        std::unordered_set<std::string>{"blue"},
        std::unordered_set<std::string>{"reading"},
        std::unordered_set<std::string>{"english"}
    );

    persons.emplace_back(
        "p3",
        2025,
        Region::US_Northeast,
        PrimaryOS::MacOS,
        EngineeringFocus::Medical_Devices,
        StudyTime::Morning,
        4,
        std::unordered_set<std::string>{"blue"},
        std::unordered_set<std::string>{"reading"},
        std::unordered_set<std::string>{"spanish"}
    );

    // A couple of other people to make the dataset less trivial
    persons.emplace_back(
        "p4",
        2025,
        Region::US_West,
        PrimaryOS::Windows,
        EngineeringFocus::Electronics,
        StudyTime::Night,
        4,
        std::unordered_set<std::string>{"blue"},
        std::unordered_set<std::string>{"gaming"},
        std::unordered_set<std::string>{"english"}
    );

    persons.emplace_back(
        "p5",
        2024,
        Region::US_West,
        PrimaryOS::Windows,
        EngineeringFocus::Electronics,
        StudyTime::Night,
        4,
        std::unordered_set<std::string>{"red"},
        std::unordered_set<std::string>{"drawing"},
        std::unordered_set<std::string>{"english"}
    );

    std::cout << "Dataset size: " << persons.size() << "\n\n";

    // -------------------------------
    // TEST 1: basic InsightGenerator usage
    // -------------------------------
    std::cout << "=== TEST 1: generate with no suppressed keys ===\n\n";

    InsightGenerator generator;
    std::unordered_set<std::string> suppressed; // start empty

    std::vector<Insight> insights1 = generator.generate(persons, suppressed);

    std::cout << "Generated " << insights1.size() << " insights:\n\n";
    for (std::size_t i = 0; i < insights1.size(); ++i) {
        printInsight(insights1[i], i);
    }

    // -------------------------------
    // TEST 2: suppression (blocklist)
    // -------------------------------
    std::cout << "\n=== TEST 2: generate with one suppressed key ===\n\n";

    if (!insights1.empty()) {
        // Suppress the first insight's key
        std::string blockKey = insights1[0].key;
        std::cout << "Suppressing key: " << blockKey << "\n\n";

        std::unordered_set<std::string> suppressed2;
        suppressed2.insert(blockKey);

        std::vector<Insight> insights2 = generator.generate(persons, suppressed2);

        std::cout << "Generated " << insights2.size()
                  << " insights after suppression:\n\n";
        for (std::size_t i = 0; i < insights2.size(); ++i) {
            printInsight(insights2[i], i);
        }

        // Quick sanity check: make sure the suppressed key is gone
        bool stillPresent = false;
        for (const auto& ins : insights2) {
            if (ins.key == blockKey) {
                stillPresent = true;
                break;
            }
        }
        std::cout << "Suppressed key present after re-generate? "
                  << (stillPresent ? "YES (BUG)" : "NO (OK)") << "\n";
    } else {
        std::cout << "No insights generated in Test 1, cannot test suppression.\n";
    }

    return 0;
}
*/
/*
#include <iostream>
#include <fstream>
#include "InsightStore.h"

using namespace std;

// Utility to quickly write contents to a file
void writeFile(const string& filename, const string& contents) {
    ofstream out(filename);
    out << contents;
    out.close();
}

// Utility to delete files after tests
void removeFile(const string& filename) {
    remove(filename.c_str());
}

int main() {

    cout << "===== Running InsightStore Tests =====\n";

    string usefulFile = "test_output_useful.csv";
    string blockedFile = "test_output_blocked.txt";

    // ------------------------------------------
    // 1. Test saveUseful
    // ------------------------------------------
    {
        cout << "[TEST] saveUseful...\n";

        Insight i1;
        i1.key = "KEY1";
        i1.description = "Insight one example";
        i1.score = 90;
        i1.support = 10;
        i1.population = 30;

        Insight i2;
        i2.key = "KEY2";
        i2.description = "Insight two example";
        i2.score = 70;
        i2.support = 7;
        i2.population = 30;

        InsightStore store;

        vector<Insight> v = { i1, i2 };
        store.saveUseful(v, usefulFile);

        ifstream in(usefulFile);
        if (!in.is_open()) {
            cout << "FAIL: useful CSV file not created.\n";
            return 1;
        }

        int count = 0;
        string line;
        while (getline(in, line)) count++;
        in.close();

        if (count != 2) {
            cout << "FAIL: useful CSV file should contain 2 lines.\n";
            return 1;
        }

        cout << "PASS!\n";
    }

    // ------------------------------------------
    // 2. Test loadUseful
    // ------------------------------------------
    {
        cout << "[TEST] loadUseful...\n";

        InsightStore store;
        store.loadUseful(usefulFile);

        const vector<Insight>& loaded = store.getUseful();

        if (loaded.size() != 2) {
            cout << "FAIL: loadUseful did not load 2 insights.\n";
            return 1;
        }

        if (loaded[0].key != "KEY1") {
            cout << "FAIL: loaded insight key mismatch.\n";
            return 1;
        }

        cout << "PASS!\n";
    }

    // ------------------------------------------
    // 3. Test saveBlocked
    // ------------------------------------------
    {
        cout << "[TEST] saveBlocked...\n";

        InsightStore store;

        store.addBlockedKey("BAD1");
        store.addBlockedKey("BAD2");

        store.saveBlocked(blockedFile);

        ifstream in(blockedFile);
        if (!in.is_open()) {
            cout << "FAIL: blocked file not created.\n";
            return 1;
        }

        vector<string> lines;
        string line;
        while (getline(in, line)) lines.push_back(line);
        in.close();

        if (lines.size() != 2) {
            cout << "FAIL: blocked file should have 2 lines.\n";
            return 1;
        }

        cout << "PASS!\n";
    }

    // ------------------------------------------
    // 4. Test loadBlocked and isBlocked()
    // ------------------------------------------
    {
        cout << "[TEST] loadBlocked + isBlocked...\n";

        InsightStore store;
        store.loadBlocked(blockedFile);

        if (!store.isBlocked("BAD1") || !store.isBlocked("BAD2")) {
            cout << "FAIL: blocked keys not correctly loaded.\n";
            return 1;
        }

        if (store.isBlocked("GOODKEY")) {
            cout << "FAIL: falsely blocking good key.\n";
            return 1;
        }

        cout << "PASS!\n";
    }

    // ------------------------------------------
    // 5. Test filterBlocked()
    // ------------------------------------------
    {
        cout << "[TEST] filterBlocked...\n";

        Insight a;
        a.key = "BAD1";
        Insight b;
        b.key = "GOOD";

        InsightStore store;
        store.addBlockedKey("BAD1");

        vector<Insight> input = { a, b };
        vector<Insight> out = store.filterBlocked(input);

        if (out.size() != 1 || out[0].key != "GOOD") {
            cout << "FAIL: filterBlocked did not work correctly.\n";
            return 1;
        }

        cout << "PASS!\n";
    }

    cout << "\n===== ALL TESTS PASSED SUCCESSFULLY =====\n";

    cout << "\nCreated files:\n";
    cout << " - " << usefulFile << "\n";
    cout << " - " << blockedFile << "\n";

#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

#include "Person.h"
#include "PersonEnums.h"
#include "PersonCsvReader.h"
#include "Insight.h"
#include "InsightGenerator.h"

int main() {
    // 1) Load people from the CSV
    const std::string csvPath = "class_data_set.csv"; // adjust path if needed

    PersonCsvReader reader(csvPath);
    std::vector<Person> persons = reader.read();

    std::cout << "Loaded " << persons.size()
              << " people from " << csvPath << "\n\n";

    // ============================================================
    // 2) DEBUG: OS -> StudyTime distribution (what generator sees)
    // ============================================================
    {
        std::map<PrimaryOS, std::map<StudyTime, std::size_t>> counts;
        std::size_t eligible = 0;

        for (const Person& p : persons) {
            PrimaryOS os = p.getPrimaryOS();
            StudyTime st = p.getStudyTime();
            if (os == PrimaryOS::Unknown || st == StudyTime::Unknown) {
                continue;
            }
            eligible++;
            counts[os][st]++;
        }

        std::cout << "=== DEBUG: OS -> StudyTime ===\n";
        std::cout << "Eligible (OS + StudyTime known): " << eligible << "\n";

        for (const auto& osEntry : counts) {
            PrimaryOS os = osEntry.first;
            const auto& dist = osEntry.second;

            std::cout << "OS = " << to_string(os) << "\n";
            for (const auto& stEntry : dist) {
                StudyTime st = stEntry.first;
                std::size_t n = stEntry.second;

                std::cout << "  StudyTime = " << to_string(st)
                          << " : " << n << "\n";
            }
        }
        std::cout << "\n";
    }

    // ============================================================
    // 3) DEBUG: favoriteColor -> hobby distribution
    // ============================================================
    {
        std::size_t eligible = 0;
        std::unordered_map<std::string,
                           std::unordered_map<std::string, std::size_t>> colorHobbyCounts;

        for (const Person& p : persons) {
            const auto& colors  = p.getFavoriteColors();
            const auto& hobbies = p.getHobbies();

            // Only consider people who have at least one color AND at least one hobby
            if (colors.empty() || hobbies.empty()) {
                continue;
            }

            eligible++;

            for (const std::string& color : colors) {
                auto& hobbyMap = colorHobbyCounts[color];
                for (const std::string& hobby : hobbies) {
                    hobbyMap[hobby]++;   // count this hobby for this color
                }
            }
        }

        std::cout << "=== DEBUG: Favorite Color -> Hobby ===\n";
        std::cout << "Eligible (favoriteColors + hobbies known): " << eligible << "\n";

        for (const auto& colorEntry : colorHobbyCounts) {
            const std::string& color = colorEntry.first;
            const auto& hobbyMap = colorEntry.second;

            std::cout << "Color = " << color << "\n";
            for (const auto& hobbyEntry : hobbyMap) {
                const std::string& hobby = hobbyEntry.first;
                std::size_t n = hobbyEntry.second;

                std::cout << "  Hobby = " << hobby
                          << " : " << n << "\n";
            }
        }
        std::cout << "\n";
    }

    // ============================================================
    // 4) Generate all insights using InsightGenerator
    // ============================================================
    InsightGenerator generator;
    std::unordered_set<std::string> suppressed; // empty for now

    std::vector<Insight> insights = generator.generate(persons, suppressed);

    // ============================================================
    // 5a) Print OS -> StudyTime insights
    // ============================================================
    std::cout << "=== Generated Insights: Primary OS -> Study Time ===\n";
    bool anyOsInsights = false;

    for (const auto& ins : insights) {
        if (ins.key.find("primary_os") != std::string::npos) {
            anyOsInsights = true;
            std::cout << "Score " << ins.score << " | "
                      << ins.description
                      << "]\n";
        }
    }

    if (!anyOsInsights) {
        std::cout << "(No primary OS -> study time insights passed thresholds.)\n";
    }

    std::cout << "\n";

    // ============================================================
    // 5b) Print favoriteColor -> hobby insights
    // ============================================================
    std::cout << "=== Generated Insights: Favorite Color -> Hobby ===\n";
    bool anyColorInsights = false;

    for (const auto& ins : insights) {
        if (ins.key.find("favorite_color") != std::string::npos) {
            anyColorInsights = true;
            std::cout << "Score " << ins.score << " | "
                      << ins.description
                    << "]\n";
        }
    }

    if (!anyColorInsights) {
        std::cout << "(No favorite color -> hobby insights passed thresholds.)\n";
    }

    std::cout << "=== Generated Insights: Region -> Languages Spoken ===\n";
    bool anyRegionInsights = false;

    for (const auto& ins : insights) {
    // We used "region = ..." and "language = ..." in InsightGenerator keys
    if (ins.key.find("region") != std::string::npos &&
        ins.key.find("language") != std::string::npos) {
        anyRegionInsights = true;
        std::cout << "[score " << ins.score << "] "
                  << ins.description << "\n";
        }
    }

    if (!anyRegionInsights) {
    std::cout << "(No region -> language insights passed thresholds.)\n";
    }

    std::cout << "=== Generated Insights: Engineering Focus -> Course Load ===\n";
    bool anyFocusInsights = false;

    for (const auto& ins : insights) {
    if (ins.key.find("engineering_focus") != std::string::npos &&
        ins.key.find("course_load") != std::string::npos) {
        anyFocusInsights = true;
        std::cout << "[score " << ins.score << "] "
                  << ins.description << "\n";
        }
    }

    if (!anyFocusInsights) {
    std::cout << "(No engineering focus -> course load insights passed thresholds.)\n";
    }

    return 0;
}
*/



// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <string>

// #include "InsightStore.h"

// using namespace std;

// // Write text content to file
// void writeFile(const string& path, const string& contents) {
//     ofstream out(path);
//     out << contents;
//     out.close();
// }

// // Utility to delete a file if needed (optional)
// void removeFile(const string& path) {
//     remove(path.c_str());
// }

// int main() {

//     cout << "===== Running InsightStore Tests =====\n\n";

//     // The test files this main() will create
//     string usefulFile = "test_useful_output.csv";
//     string blockedFile = "test_blocked_output.txt";

//     // ---------------------------------------------------------
//     // 1. TEST: saveUseful()
//     // ---------------------------------------------------------
//     cout << "[1] Testing saveUseful...\n";

//     InsightStore store;

//     Insight a;
//     a.key = "K1";
//     a.description = "Mac users study at night";
//     a.score = 88;
//     a.support = 12;
//     a.population = 40;

//     Insight b;
//     b.key = "K2";
//     b.description = "Windows users like mornings";
//     b.score = 72;
//     b.support = 8;
//     b.population = 40;

//     vector<Insight> testInsights = { a, b };

//     store.saveUseful(testInsights, usefulFile);

//     ifstream infile(usefulFile);
//     if (!infile.is_open()) {
//         cout << "FAIL: Could not create " << usefulFile << "\n";
//         return 1;
//     }

//     int lineCount = 0;
//     string line;
//     while (getline(infile, line)) {
//         lineCount++;
//     }
//     infile.close();

//     if (lineCount == 2) {
//         cout << "PASS: saveUseful created 2 lines.\n\n";
//     } else {
//         cout << "FAIL: expected 2 lines, found " << lineCount << "\n";
//         return 1;
//     }

//     // ---------------------------------------------------------
//     // 2. TEST: loadUseful()
//     // ---------------------------------------------------------
//     cout << "[2] Testing loadUseful...\n";

//     InsightStore store2;
//     store2.loadUseful(usefulFile);

//     const vector<Insight>& loaded = store2.getUseful();

//     if (loaded.size() != 2) {
//         cout << "FAIL: loadUseful did not load 2 insights\n";
//         return 1;
//     }

//     if (loaded[0].key != "K1") {
//         cout << "FAIL: first insight key mismatch\n";
//         return 1;
//     }

//     cout << "PASS: loadUseful loaded insights correctly\n\n";

//     // ---------------------------------------------------------
//     // 3. TEST: saveBlocked()
//     // ---------------------------------------------------------
//     cout << "[3] Testing saveBlocked...\n";

//     InsightStore store3;
//     store3.addBlockedKey("BAD_A");
//     store3.addBlockedKey("BAD_B");
//     store3.saveBlocked(blockedFile);

//     ifstream infile2(blockedFile);
//     if (!infile2.is_open()) {
//         cout << "FAIL: blocked file not created\n";
//         return 1;
//     }

//     vector<string> blockedLines;
//     while (getline(infile2, line)) {
//         blockedLines.push_back(line);
//     }
//     infile2.close();

//     if (blockedLines.size() == 2) {
//         cout << "PASS: saveBlocked saved 2 entries\n\n";
//     } else {
//         cout << "FAIL: saveBlocked expected 2 entries\n";
//         return 1;
//     }

//     // ---------------------------------------------------------
//     // 4. TEST: loadBlocked() + isBlocked()
//     // ---------------------------------------------------------
//     cout << "[4] Testing loadBlocked + isBlocked...\n";

//     InsightStore store4;
//     store4.loadBlocked(blockedFile);

//     if (!store4.isBlocked("BAD_A") || !store4.isBlocked("BAD_B")) {
//         cout << "FAIL: Blocked keys did not load correctly\n";
//         return 1;
//     }

//     if (store4.isBlocked("GOOD_KEY")) {
//         cout << "FAIL: incorrectly blocks a good key\n";
//         return 1;
//     }

//     cout << "PASS: loadBlocked + isBlocked work correctly\n\n";

//     // ---------------------------------------------------------
//     // 5. TEST: filterBlocked()
//     // ---------------------------------------------------------
//     cout << "[5] Testing filterBlocked...\n";

//     Insight bad;  bad.key  = "BAD_A";
//     Insight good; good.key = "GOOD_X";

//     vector<Insight> candidates = { bad, good };
//     vector<Insight> filtered = store4.filterBlocked(candidates);

//     if (filtered.size() != 1 || filtered[0].key != "GOOD_X") {
//         cout << "FAIL: filterBlocked did not filter out bad insights\n";
//         return 1;
//     }

//     cout << "PASS: filterBlocked removes blocked insights\n\n";

//     // ---------------------------------------------------------
//     // Final output
//     // ---------------------------------------------------------
//     cout << "===== ALL TESTS PASSED SUCCESSFULLY =====\n\n";

//     cout << "Files created:\n";
//     cout << " - " << usefulFile << "\n";
//     cout << " - " << blockedFile << "\n";

//     return 0;
// }


#include "Cli.h"

int main() {
    Cli cli;
    cli.run();
    return 0;
}
