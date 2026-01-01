#ifndef DECODERSCPP_CLI_H
#define DECODERSCPP_CLI_H

#include "PersonRepository.h"
#include "PersonCsvReader.h"
#include "PersonJsonReader.h"
#include "PersonBuilder.h"
#include "InsightGenerator.h"
#include "InsightStore.h"

#include <string>
#include <vector>

using namespace std;

// CLI provides all user-facing text commands.
class Cli {
public:
    Cli();

    // Starts the command loop
    void run();

private:
    PersonRepository repo;
    InsightGenerator generator;
    InsightStore store;
    string currentDatasetPath;  // data persistence

    vector<Insight> lastGenerated;   // cached insights from "generate"

    // Commands
    void cmdLoad(const string& path);
    void cmdLoadJson();  // default URL
    void cmdLoadJsonCustom(const string& url); //user  URL
    void cmdSaveDataset();  // save to current file
    void cmdSaveAs(const string& filename);  // save to new file
    void cmdListPeople() const;
    void cmdAddPerson();
    void cmdEditPerson(size_t index);
    void cmdRemovePerson(size_t index);

    void cmdGenerateAuto(); // original generate function
    void cmdGenerateCustom(const std::string& a, const std::string& b);
    void cmdDiscoverBest();  // 6x6 heat map (36 cells, 15 pairs)
    void cmdDiscoverAll();   // 9x9 heat map (81 cells, 36 pairs)

    void cmdListInsights() const;
    void cmdSaveUseful(const vector<size_t>& indexes, const string& filename);
    void cmdDiscard(const vector<size_t>& indexes, const string& filename);

    void cmdListSaved() const;

    // helper
    void printHelp() const;
};

#endif // DECODERSCPP_CLI_H
