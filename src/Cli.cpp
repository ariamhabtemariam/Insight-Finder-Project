#include "Cli.h"
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <algorithm>
#include <map>

using namespace std;

Cli::Cli() {
    // try loading saved knowledge
    store.loadUseful("insights_saved.csv");
    store.loadBlocked("blocked_keys.txt");
}

void Cli::run() {
    cout << "Insight Finder CLI\n";
    cout << "Type 'help' for a list of commands.\n";

    string line;

    while (true) {
        cout << "> ";
        getline(cin, line);

        if (line.empty())
            continue;

        stringstream ss(line);
        string cmd;
        ss >> cmd;

        if (cmd == "help") {
            printHelp();
        }
        else if (cmd == "quit" || cmd == "exit") {
            cout << "Exiting...\n";
            break;
        }
        else if (cmd == "load") {
            string path;
            ss >> path;
            cmdLoad(path);
        }
        else if (cmd == "load-json") {
            cmdLoadJson();
        }
        else if (cmd == "load-json-custom") {
            string url;
            getline(ss, url);
            // Trim leading whitespace
            size_t start = url.find_first_not_of(" \t");
            if (start != string::npos) {
                url = url.substr(start);
            }
            if (url.empty()) {
                cout << "Usage: load-json-custom <url>\n";
                cout << "Example: load-json-custom http://example.com/data.json\n";
            } else {
                cmdLoadJsonCustom(url);
            }
        }
        else if (cmd == "save-dataset") {
            cmdSaveDataset();
        }
        else if (cmd == "save-as") {
            string filename;
            ss >> filename;
            if (filename.empty()) {
                cout << "Usage: save-as <filename.csv>\n";
            } else {
                cmdSaveAs(filename);
            }
        }
        else if (cmd == "list") {
            cmdListPeople();
        }
        else if (cmd == "add") {
            cmdAddPerson();
        }
        else if (cmd == "edit") {
            size_t idx;
            ss >> idx;
            cmdEditPerson(idx);
        }
        else if (cmd == "remove") {
            size_t idx;
            ss >> idx;
            cmdRemovePerson(idx);
        }



        else if (cmd == "generate") { //same as the original generate
            cmdGenerateAuto();
        }
        else if (cmd == "generate-auto") { //same as the original generate got error when i only have 1
            cmdGenerateAuto();
        }
        else if (cmd == "generate-custom") {  // user picks categories
            string topic_a, topic_b;
            ss >> topic_a >> topic_b;
            if (topic_a.empty() || topic_b.empty()) {
                cout << "Usage: generate-custom <topic1> <topic2>\n";
            } else {
                cmdGenerateCustom(topic_a, topic_b);
            }
        }
        else if (cmd == "discover-best") {  // creative feature - 6x6 matrix
            cmdDiscoverBest();
        }
        else if (cmd == "discover-all") {  // full 9x9 matrix
            cmdDiscoverAll();
        }

        else if (cmd == "list-insights") {
            cmdListInsights();
        }
        else if (cmd == "save") {
            vector<size_t> idxs;
            string filename = "insights_saved.csv";  // default
            string token;
            
            // parse and optional filename
            while (ss >> token) {
                // error handling file name
                if (token.find('.') != string::npos || token.find('/') != string::npos) {
                    filename = token;
                } else {
                    // try to parse as index
                    try {
                        size_t idx = stoul(token);
                        idxs.push_back(idx);
                    } catch (...) {
                        cout << "Invalid index or filename: " << token << "\n";
                    }
                }
            }
            cmdSaveUseful(idxs, filename);
        }
        else if (cmd == "discard") {
            vector<size_t> idxs;
            string filename = "blocked_keys.txt";  // default
            string token;
            
            // parse  and optional filename
            while (ss >> token) {
                // check if filename
                if (token.find('.') != string::npos || token.find('/') != string::npos) {
                    filename = token;
                } else {
                    // parse as index
                    try {
                        size_t idx = stoul(token);
                        idxs.push_back(idx);
                    } catch (...) {
                        cout << "Invalid index or filename: " << token << "\n";
                    }
                }
            }
            cmdDiscard(idxs, filename);
        }
        else if (cmd == "list-saved") {
            cmdListSaved();
        }
        else {
            cout << "Unknown command. Type 'help'.\n";
        }
    }
}

void Cli::cmdLoad(const string& path) {
    PersonCsvReader reader(path);
    vector<Person> persons = reader.read();

    repo.setPersons(persons);
    currentDatasetPath = path;  //remember path for save-dataset

    cout << "Loaded " << repo.size() << " people.\n";
}

void Cli::cmdLoadJson() {
    // default URL from project requirements
    const string defaultUrl = "http://gist.githubusercontent.com/esolovey-bu/cba6c1b4eedd0a621ce879e6e6299d28/raw/sample_people.json?v=2";
    
    cout << "Fetching JSON from default URL...\n";
    cout << "URL: " << defaultUrl << "\n";
    
    PersonJsonReader reader(defaultUrl);
    vector<Person> persons = reader.read();
    
    if (persons.empty()) {
        cout << "Failed to load or no people found in JSON.\n";
        return;
    }
    
    repo.setPersons(persons);
    currentDatasetPath = "";  // JSON loaded, no local file path
    cout << "Loaded " << repo.size() << " people from JSON.\n";
}

void Cli::cmdLoadJsonCustom(const string& url) {
    cout << "Fetching JSON from custom URL...\n";
    cout << "URL: " << url << "\n";
    
    PersonJsonReader reader(url);
    vector<Person> persons = reader.read();
    
    if (persons.empty()) {
        cout << "Failed to load or no people found in JSON.\n";
        cout << "Make sure the URL is correct and returns valid JSON.\n";
        return;
    }
    
    repo.setPersons(persons);
    currentDatasetPath = "";  // JSON loaded, no local file path
    cout << "Loaded " << repo.size() << " people from custom JSON URL.\n";
}

void Cli::cmdSaveDataset() {
    if (currentDatasetPath.empty()) {
        cout << "No dataset path set. Use 'save-as <filename.csv>' first.\n";
        return;
    }
    
    if (repo.saveToCsv(currentDatasetPath)) {
        cout << "Dataset saved to: " << currentDatasetPath << "\n";
    } else {
        cout << "Failed to save dataset.\n";
    }
}

void Cli::cmdSaveAs(const string& filename) {
    if (repo.size() == 0) {
        cout << "No data to save. Load a dataset first.\n";
        return;
    }
    
    if (repo.saveToCsv(filename)) {
        currentDatasetPath = filename;  // update current path
        cout << "Dataset saved to: " << filename << "\n";
    } else {
        cout << "Failed to save dataset.\n";
    }
}

void Cli::cmdListPeople() const {
    const auto& persons = repo.getAll();

    if (persons.empty()) {
        cout << "No people loaded.\n";
        return;
    }

    for (size_t i = 0; i < persons.size(); i++) {
        cout << i << ") " << persons[i].toString() << "\n";
    }
}















void Cli::cmdAddPerson() {
    cout << "=== Add New Person ===\n";
    string input;
    PersonBuilder builder;
    
    // ID
    cout << "ID (nickname or identifier): ";
    getline(cin, input);
    if (input.empty()) {
        cout << "Cancelled - ID required.\n";
        return;
    }
    builder.setId(input);
    
    // Graduation Year
    cout << "Graduation Year (e.g., 2025): ";
    getline(cin, input);
    if (!input.empty()) builder.setGraduationYear(stoi(input));
    
    // Region
    cout << "Region (us-northeast, us-west, china, etc.): ";
    getline(cin, input);
    if (!input.empty()) builder.setRegion(input);
    
    // Primary OS
    cout << "Primary OS (Windows, MacOS, Linux): ";
    getline(cin, input);
    if (!input.empty()) builder.setPrimaryOS(input);
    
    // Engineering Focus
    cout << "Engineering Focus (cybersecurity, electronics, etc.): ";
    getline(cin, input);
    if (!input.empty()) builder.setEngineeringFocus(input);
    
    // Study Time
    cout << "Study Time (Morning, Afternoon, Night): ";
    getline(cin, input);
    if (!input.empty()) builder.setStudyTime(input);
    
    // Course Load
    cout << "Course Load (number of courses): ";
    getline(cin, input);
    if (!input.empty()) builder.setCourseLoad(stoi(input));
    
    // Favorite Colors
    cout << "Favorite Colors (comma-separated, e.g., blue,green): ";
    getline(cin, input);
    if (!input.empty()) builder.setColorsFromString(input);
    
    // Hobbies
    cout << "Hobbies (comma-separated, e.g., gaming,reading): ";
    getline(cin, input);
    if (!input.empty()) builder.setHobbiesFromString(input);
    
    // Languages
    cout << "Languages (comma-separated, e.g., english,spanish): ";
    getline(cin, input);
    if (!input.empty()) builder.setLanguagesFromString(input);
    
    // Build and add person using PersonBuilder
    repo.addPerson(builder.build());
    
    cout << "Person added! Total: " << repo.size() << " people.\n";
    cout << "Use 'save-dataset' or 'save-as <file>' to save changes.\n";
}
















void Cli::cmdEditPerson(size_t index) {
    if (index >= repo.size()) {
        cout << "Invalid index.\n";
        return;
    }
    
    const Person& current = repo.get(index);
    cout << "=== Editing Person " << index << " ===\n";
    cout << "Press Enter to keep current value.\n\n";
    
    // Start with current values using PersonBuilder
    PersonBuilder builder;
    builder.copyFrom(current);
    
    string input;
    
    // ID
    cout << "ID [" << current.getId() << "]: ";
    getline(cin, input);
    if (!input.empty()) builder.setId(input);
    
    // Graduation Year
    cout << "Graduation Year [" << current.getGraduationYear() << "]: ";
    getline(cin, input);
    if (!input.empty()) builder.setGraduationYear(stoi(input));
    
    // Region
    cout << "Region [" << to_string(current.getRegion()) << "]: ";
    getline(cin, input);
    if (!input.empty()) builder.setRegion(input);
    
    // Primary OS
    cout << "Primary OS [" << to_string(current.getPrimaryOS()) << "]: ";
    getline(cin, input);
    if (!input.empty()) builder.setPrimaryOS(input);
    
    // Engineering Focus
    cout << "Engineering Focus [" << to_string(current.getEngineeringFocus()) << "]: ";
    getline(cin, input);
    if (!input.empty()) builder.setEngineeringFocus(input);
    
    // Study Time
    cout << "Study Time [" << to_string(current.getStudyTime()) << "]: ";
    getline(cin, input);
    if (!input.empty()) builder.setStudyTime(input);
    
    // Course Load
    cout << "Course Load [" << current.getCourseLoad() << "]: ";
    getline(cin, input);
    if (!input.empty()) builder.setCourseLoad(stoi(input));
    
    // Favorite Colors
    cout << "Current colors: ";
    for (const auto& c : current.getFavoriteColors()) cout << c << " ";
    cout << "\nNew colors (comma-separated, or Enter to keep): ";
    getline(cin, input);
    if (!input.empty()) builder.setColorsFromString(input);
    
    // Hobbies
    cout << "Current hobbies: ";
    for (const auto& h : current.getHobbies()) cout << h << " ";
    cout << "\nNew hobbies (comma-separated, or Enter to keep): ";
    getline(cin, input);
    if (!input.empty()) builder.setHobbiesFromString(input);
    
    // Languages
    cout << "Current languages: ";
    for (const auto& l : current.getLanguages()) cout << l << " ";
    cout << "\nNew languages (comma-separated, or Enter to keep): ";
    getline(cin, input);
    if (!input.empty()) builder.setLanguagesFromString(input);
    
    // Build updated person and replace using PersonBuilder
    repo.updatePerson(index, builder.build());
    
    cout << "Person updated!\n";
    cout << "Use 'save-dataset' or 'save-as <file>' to save changes.\n";
}

void Cli::cmdRemovePerson(size_t index) {
    if (index >= repo.size()) {
        cout << "Invalid index.\n";
        return;
    }
    repo.removePerson(index);
    cout << "Person removed.\n";
}









void Cli::cmdGenerateAuto() {
    cout << "Generating insights automatically...\n";

    // Step 1: get people
    vector<Person> all = repo.getAll();

    // Step 2: get blocked keys
    unordered_set<string> suppressedKeys;
    // store already holds them internally but we pass nothing for now
    // the generator receives them as a parameter
    // store.filterBlocked will handle suppression after generation

    // Step 3: generate everything
    auto raw = generator.generate(all, suppressedKeys);

    // Step 4: filter based on InsightStore blocklist
    lastGenerated = store.filterBlocked(raw);

    cout << "Generated " << lastGenerated.size() << " insights.\n";
}

void Cli::cmdGenerateCustom(const string& attr1, const string& attr2) {
    cout << "Generating insights matching '" << attr1
         << "' and '" << attr2 << "'...\n";

    vector<Person> all = repo.getAll();
    unordered_set<string> suppressedKeys;

    // generic generator for any combination
    auto raw = generator.generateGeneric(all, suppressedKeys, attr1, attr2);

    // uses the blocklist
    lastGenerated = store.filterBlocked(raw);

    if (lastGenerated.empty()) {
        cout << "No insights matched those attributes.\n";
    } else {
        cout << "Generated " << lastGenerated.size()
             << " matching insights.\n";
    }
}








void Cli::cmdListInsights() const {
    if (lastGenerated.empty()) {
        cout << "No insights generated yet.\n";
        return;
    }

    for (size_t i = 0; i < lastGenerated.size(); i++) {
        const Insight& x = lastGenerated[i];
        cout << i << ") "
             << "[Score " << x.score << "] "
             << x.description << "\n";
    }
}

void Cli::cmdSaveUseful(const vector<size_t>& indexes, const string& filename) {
    if (indexes.empty()) {
        cout << "Usage: save <index1> <index2> ... [filename.csv]\n";
        return;
    }

    vector<Insight> chosen;

    for (size_t idx : indexes) {
        if (idx < lastGenerated.size()) {
            chosen.push_back(lastGenerated[idx]);
            cout << "Saved insight: " << lastGenerated[idx].description << "\n";
        }
    }

    store.saveUseful(chosen, filename);
    cout << "Saved to: " << filename << "\n";
}

void Cli::cmdDiscard(const vector<size_t>& indexes, const string& filename) {
    if (indexes.empty()) {
        cout << "Usage: discard <index1> <index2> ... [filename.txt]\n";
        return;
    }

    for (size_t idx : indexes) {
        if (idx < lastGenerated.size()) {
            store.addBlockedKey(lastGenerated[idx].key);
            cout << "Discarded insight: " << lastGenerated[idx].description << "\n";
        }
    }

    store.saveBlocked(filename);
    cout << "Blocked keys saved to: " << filename << "\n";
}

void Cli::cmdListSaved() const {
    const auto& saved = store.getUseful();
    if (saved.empty()) {
        cout << "No saved insights.\n";
        return;
    }

    for (size_t i = 0; i < saved.size(); i++) {
        cout << i << ") " << saved[i].description
             << " (score=" << saved[i].score << ")\n";
    }
}

void Cli::cmdDiscoverBest() {
    cout << "===========================================\n";
    cout << "  DISCOVERING STRONGEST RELATIONSHIPS\n";
    cout << "===========================================\n\n";
    cout << "6x6 Heat Map (36 cells, 15 unique pairs)\n";
    cout << "Use 'discover-all' for 9x9 heat map (81 cells, 36 pairs).\n\n";

    //edge case
    vector<Person> all = repo.getAll();
    if (all.empty()) {
        cout << "No data loaded. Use 'load <csv>' first.\n";
        return;
    }

    unordered_set<string> suppressedKeys;

    // Define 6 core attributes (original)
    vector<string> attributes = {
        "os", "study", "color", "hobby", "region", "language"
    };

    struct CombinationResult {
        string attrX;
        string attrY;
        int count = 0;
        int totalScore = 0;
        double avgScore = 0.0;
    };

    vector<CombinationResult> results;

    // test all combinations
    for (size_t i = 0; i < attributes.size(); i++) {
        for (size_t j = i + 1; j < attributes.size(); j++) {
            auto insights = generator.generateGeneric(all, suppressedKeys, 
                                                     attributes[i], attributes[j]);
            
            if (!insights.empty()) {
                CombinationResult result;
                result.attrX = attributes[i];
                result.attrY = attributes[j];
                result.count = insights.size();
                
                for (const auto& ins : insights) {
                    result.totalScore += ins.score;
                }
                result.avgScore = static_cast<double>(result.totalScore) / result.count;
                
                results.push_back(result);
            }
        }
    }

    // sort by average score in descending order
    sort(results.begin(), results.end(), 
         [](const CombinationResult& a, const CombinationResult& b) {
             return a.avgScore > b.avgScore;
         });

    // shows top 10 strongest relationships
    cout << "TOP 10 STRONGEST RELATIONSHIPS:\n";
    cout << "============================================\n\n";

    int displayCount = min(10, static_cast<int>(results.size()));
    for (int i = 0; i < displayCount; i++) {
        const auto& r = results[i];
        
        // visual bar
        int barLength = static_cast<int>(r.avgScore / 5);
        string bar(barLength, '#');
        
        cout << i + 1 << ". " << r.attrX << " ←→ " << r.attrY << "\n";
        cout << "   Avg Score: " << static_cast<int>(r.avgScore) << "/100  ";
        cout << bar << "\n";
        cout << "   Insights Found: " << r.count << "\n\n";
    }

    // lookup map 
    map<pair<string, string>, double> scoreMap;
    for (const auto& r : results) {
        scoreMap[{r.attrX, r.attrY}] = r.avgScore;
        scoreMap[{r.attrY, r.attrX}] = r.avgScore;  // symmetric
    }

    // searched ansi color codes -
    const string RED = "\033[91m";  // over 65 strong
    const string ORANGE = "\033[38;5;208m";  // 50-65 moderate
    const string YELLOW = "\033[93m"; // under 50 weak
    const string RESET = "\033[0m";  // reset color

    // matrix 1 symbols 
    cout << "============================================\n";
    cout << "CORRELATION STRENGTH MATRIX (SYMBOLS):\n";
    cout << "============================================\n\n";
    cout << "Legend: " << YELLOW << "." << RESET << " <50 (weak)  " << ORANGE << "+" << RESET << " 50-65 (moderate)  " << RED << "*" << RESET << " >65 (strong)\n\n";

    // display attribute labels
    cout << "        ";
    for (const auto& attr : attributes) {
        cout << attr.substr(0, 4) << " ";
    }
    cout << "\n";

    // show symbol matrix
    for (size_t i = 0; i < attributes.size(); i++) {
        cout << attributes[i].substr(0, 7);
        for (size_t pad = attributes[i].length(); pad < 8; pad++) cout << " ";
        
        for (size_t j = 0; j < attributes.size(); j++) {
            if (i == j) {
                cout << " -   ";
            } else {
                auto it = scoreMap.find({attributes[i], attributes[j]});
                if (it != scoreMap.end()) {
                    double score = it->second;
                    char symbol;
                    string color;
                    if (score > 65) { symbol = '*'; color = RED; }
                    else if (score >= 50) { symbol = '+'; color = ORANGE; }
                    else { symbol = '.'; color = YELLOW; }
                    
                    cout << " " << color << symbol << RESET << "   ";
                } else {
                    cout << " -   ";
                }
            }
        }
        cout << "\n";
    }

    // matrix 2: numbers
    cout << "\n============================================\n";
    cout << "CORRELATION STRENGTH MATRIX (SCORES):\n";
    cout << "============================================\n\n";
    cout << "Color Scale: " << YELLOW << "<50" << RESET << " (weak)  " << ORANGE << "50-65" << RESET << " (moderate)  " << RED << ">65" << RESET << " (strong)\n\n";

    // display attribute labels
    cout << "        ";
    for (const auto& attr : attributes) {
        cout << attr.substr(0, 4) << "  ";
    }
    cout << "\n";

    // show score matrix 
    for (size_t i = 0; i < attributes.size(); i++) {
        cout << attributes[i].substr(0, 7);
        for (size_t pad = attributes[i].length(); pad < 8; pad++) cout << " ";
        
        for (size_t j = 0; j < attributes.size(); j++) {
            if (i == j) {
                cout << "  --  ";
            } else {
                auto it = scoreMap.find({attributes[i], attributes[j]});
                if (it != scoreMap.end()) {
                    int scoreInt = static_cast<int>(it->second);
                    string color;
                    if (scoreInt > 65) color = RED;
                    else if (scoreInt >= 50) color = ORANGE;
                    else color = YELLOW;
                    
                    if (scoreInt < 10) cout << "  ";
                    else if (scoreInt < 100) cout << " ";
                    cout << color << scoreInt << RESET << "  ";
                } else {
                    cout << "  --  ";
                }
            }
        }
        cout << "\n";
    }

    cout << "\n============================================\n";
    cout << "Use 'generate-custom <attr1> <attr2>' to explore!\n";
    cout << "============================================\n";
}

void Cli::cmdDiscoverAll() {
    cout << "===========================================\n";
    cout << "  FULL RELATIONSHIP ANALYSIS (9x9)\n";
    cout << "===========================================\n\n";
    cout << "9x9 Heat Map (81 cells, 36 unique pairs)\n\n";

    vector<Person> all = repo.getAll();
    if (all.empty()) {
        cout << "No data loaded. Use 'load <csv>' first.\n";
        return;
    }

    unordered_set<string> suppressedKeys;

    // 9 attributes
    vector<string> attributes = {
        "os", "study", "color", "hobby", "region", "language", "focus", "course", "graduation"
    };

    struct CombinationResult {
        string attrX;
        string attrY;
        int count = 0;
        int totalScore = 0;
        double avgScore = 0.0;
    };

    vector<CombinationResult> results;

    for (size_t i = 0; i < attributes.size(); i++) {
        for (size_t j = i + 1; j < attributes.size(); j++) {
            auto insights = generator.generateGeneric(all, suppressedKeys, 
                                                     attributes[i], attributes[j]);
            if (!insights.empty()) {
                CombinationResult result;
                result.attrX = attributes[i];
                result.attrY = attributes[j];
                result.count = insights.size();
                for (const auto& ins : insights) {
                    result.totalScore += ins.score;
                }
                result.avgScore = static_cast<double>(result.totalScore) / result.count;
                results.push_back(result);
            }
        }
    }

    sort(results.begin(), results.end(), 
         [](const CombinationResult& a, const CombinationResult& b) {
             return a.avgScore > b.avgScore;
         });

    cout << "TOP 10 STRONGEST RELATIONSHIPS:\n";
    cout << "============================================\n\n";

    int displayCount = min(10, static_cast<int>(results.size()));
    for (int i = 0; i < displayCount; i++) {
        const auto& r = results[i];
        int barLength = static_cast<int>(r.avgScore / 5);
        string bar(barLength, '#');
        cout << i + 1 << ". " << r.attrX << " <-> " << r.attrY << "\n";
        cout << "   Avg Score: " << static_cast<int>(r.avgScore) << "/100  " << bar << "\n";
        cout << "   Insights Found: " << r.count << "\n\n";
    }

    map<pair<string, string>, double> scoreMap;
    for (const auto& r : results) {
        scoreMap[{r.attrX, r.attrY}] = r.avgScore;
        scoreMap[{r.attrY, r.attrX}] = r.avgScore;
    }

    const string RED = "\033[91m";
    const string ORANGE = "\033[38;5;208m";
    const string YELLOW = "\033[93m";
    const string RESET = "\033[0m";

    cout << "============================================\n";
    cout << "FULL 9x9 CORRELATION MATRIX:\n";
    cout << "============================================\n\n";
    cout << "Color Scale: " << YELLOW << "<50" << RESET << " (weak)  " 
         << ORANGE << "50-65" << RESET << " (moderate)  " 
         << RED << ">65" << RESET << " (strong)\n\n";

    cout << "        ";
    for (const auto& attr : attributes) {
        cout << attr.substr(0, 4) << "  ";
    }
    cout << "\n";

    for (size_t i = 0; i < attributes.size(); i++) {
        cout << attributes[i].substr(0, 7);
        for (size_t pad = attributes[i].length(); pad < 8; pad++) cout << " ";
        
        for (size_t j = 0; j < attributes.size(); j++) {
            if (i == j) {
                cout << "  --  ";
            } else {
                auto it = scoreMap.find({attributes[i], attributes[j]});
                if (it != scoreMap.end()) {
                    int scoreInt = static_cast<int>(it->second);
                    string color;
                    if (scoreInt > 65) color = RED;
                    else if (scoreInt >= 50) color = ORANGE;
                    else color = YELLOW;
                    
                    if (scoreInt < 10) cout << "  ";
                    else if (scoreInt < 100) cout << " ";
                    cout << color << scoreInt << RESET << "  ";
                } else {
                    cout << "  --  ";
                }
            }
        }
        cout << "\n";
    }

    cout << "\n============================================\n";
    cout << "Use 'generate-custom <attr1> <attr2>' to explore!\n";
    cout << "============================================\n";
}

void Cli::printHelp() const {
    cout << "Commands:\n";
    cout << "\n  === Data Loading ===\n";
    cout << "  load <csv>              Load dataset from CSV file\n";
    cout << "  load-json               Load from default JSON URL\n";
    cout << "  load-json-custom <url>  Load from custom JSON URL\n";
    cout << "  list                    List loaded people\n";
    cout << "\n  === Data Persistence ===\n";
    cout << "  save-dataset            Save to current CSV file\n";
    cout << "  save-as <file.csv>      Save dataset to new file\n";
    cout << "\n  === Person Management ===\n";
    cout << "  add                     Add a person\n";
    cout << "  edit <index>            Edit a person\n";
    cout << "  remove <index>          Remove a person\n";
    cout << "\n  === Insight Generation ===\n";
    cout << "  generate                Generate all 4 default insights\n";
    cout << "  generate-auto           Same as 'generate'\n";
    cout << "  generate-custom a b     Generate insights for ANY attribute pair\n";
    cout << "                          Supports: os, study, color, hobby, region,\n";
    cout << "                                    language, focus, course, graduation\n";
    cout << "  discover-best           6x6 heat map (36 cells, 15 pairs)\n";
    cout << "  discover-all            9x9 heat map (81 cells, 36 pairs)\n";
    cout << "\n  === Insight Management ===\n";
    cout << "  list-insights           Show generated insights\n";
    cout << "  save <i1 i2...> [file]  Save insights (default: insights_saved.csv)\n";
    cout << "  discard <i1 i2...> [file] Block insights (default: blocked_keys.txt)\n";
    cout << "  list-saved              Show saved insights\n";
    cout << "\n  === General ===\n";
    cout << "  help                    Show this list\n";
    cout << "  quit                    Exit program\n";
}
