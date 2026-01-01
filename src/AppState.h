#ifndef APPSTATE_H
#define APPSTATE_H

#include <string>
#include "PersonRepository.h"

/**
 * AppState
 *
 * Holds the current PersonRepository and remembers which CSV file
 * is the "current dataset". Also handles reading/writing last_dataset.txt
 * so the last used CSV is auto-loaded on next run.
 */
class AppState {
public:
    AppState();  // constructor will try to auto-load last dataset

    // Access to repository
    PersonRepository& repository() { return m_repo; }
    const PersonRepository& repository() const { return m_repo; }

    // Load a dataset from a CSV path (and remember it)
    void loadDataset(const std::string& csvPath);

    // Save to the "current" CSV path
    bool saveDataset() const;

    // Save to a new path, and update "current" path + last_dataset.txt
    bool saveDatasetAs(const std::string& csvPath);

private:
    PersonRepository m_repo;
    std::string m_currentCsvPath;

    void loadLastDatasetPath();
    void saveLastDatasetPath() const;

    static std::string lastDatasetFileName();
};

#endif // APPSTATE_H