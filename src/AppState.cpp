#include "AppState.h"
#include "PersonCsvReader.h"

#include <fstream>
#include <iostream>  

AppState::AppState() {
    //on startup, try to read last_dataset.txt to get last used CSV path
    loadLastDatasetPath();

    if (!m_currentCsvPath.empty()) {
        try {
            loadDataset(m_currentCsvPath);
        } catch (const std::exception& ex) {
            std::cerr << "Failed to auto-load dataset '" << m_currentCsvPath
                      << "': " << ex.what() << "\n";
            m_currentCsvPath.clear();
        }
    }
}

std::string AppState::lastDatasetFileName() {
    // simple local file next to the executable
    return "last_dataset.txt";
}

void AppState::loadLastDatasetPath() {
    std::ifstream in(lastDatasetFileName());
    if (!in.is_open()) {
        // no last_dataset.txt yet; first run is fine
        return;
    }

    std::string line;
    if (std::getline(in, line)) {
        m_currentCsvPath = line;
    }
}

void AppState::saveLastDatasetPath() const {
    if (m_currentCsvPath.empty()) return;

    std::ofstream out(lastDatasetFileName());
    if (!out.is_open()) {
        std::cerr << "Warning: could not write " << lastDatasetFileName() << "\n";
        return;
    }

    out << m_currentCsvPath << "\n";
}

void AppState::loadDataset(const std::string& csvPath) {
    PersonCsvReader reader(csvPath);
    std::vector<Person> people = reader.read();

    m_repo.setPersons(std::move(people));
    m_currentCsvPath = csvPath;
    saveLastDatasetPath();
}

bool AppState::saveDataset() const {
    if (m_currentCsvPath.empty()) {
        // No known path – call saveDatasetAs() instead
        return false;
    }
    return m_repo.saveToCsv(m_currentCsvPath);
}

bool AppState::saveDatasetAs(const std::string& csvPath) {
    if (!m_repo.saveToCsv(csvPath)) {
        return false;
    }

    m_currentCsvPath = csvPath;
    saveLastDatasetPath();
    return true;
}