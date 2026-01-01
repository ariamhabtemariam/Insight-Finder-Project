#include "InsightStore.h"
#include <fstream>
#include <sstream>

using namespace std;

// Load saved useful insights from CSV
void InsightStore::loadUseful(const string& filename) {

    usefulInsights.clear();
    ifstream file(filename);

    if (!file.is_open())
        return;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Insight i;

        getline(ss, i.key, ',');
        getline(ss, i.description, ',');

        ss >> i.score; ss.ignore();
        ss >> i.support; ss.ignore();
        ss >> i.population; ss.ignore();

        usefulInsights.push_back(i);
    }
    file.close();
}

// Load blocked insight keys from text file
void InsightStore::loadBlocked(const string& filename) {

    blockedKeys.clear();
    ifstream file(filename);

    if (!file.is_open())
        return;

    string key;

    while (getline(file, key)) {
        blockedKeys.insert(key);
    }

    file.close();
}

// Append useful insights to CSV
void InsightStore::saveUseful(const vector<Insight>& insights,
                              const string& filename) {

    ofstream file(filename, ios::app);
    if (!file.is_open())
        return;

    for (const Insight& i : insights) {
        file << i.key << ","
             << i.description << ","
             << i.score << ","
             << i.support << ","
             << i.population << "\n";
    }

    file.close();
}

// Save blocklist to text file
void InsightStore::saveBlocked(const string& filename) {

    ofstream file(filename);
    if (!file.is_open())
        return;

    for (const string& key : blockedKeys) {
        file << key << "\n";
    }

    file.close();
}

void InsightStore::addBlockedKey(const string& key) {
    blockedKeys.insert(key);
}

bool InsightStore::isBlocked(const string& key) const {
    return blockedKeys.find(key) != blockedKeys.end();
}

// Remove blocked insights before showing them to the user
vector<Insight> InsightStore::filterBlocked(const vector<Insight>& insights) const {

    vector<Insight> result;

    for (const Insight& i : insights) {
        if (!isBlocked(i.key)) {
            result.push_back(i);
        }
    }

    return result;
}

const vector<Insight>& InsightStore::getUseful() const {
    return usefulInsights;
}

