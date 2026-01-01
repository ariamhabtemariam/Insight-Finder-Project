
#ifndef DECODERSCPP_INSIGHTSTORE_H
#define DECODERSCPP_INSIGHTSTORE_H
#include "Insight.h"
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

/**
 * InsightStore
 *
 * Saves useful insights to disk and keeps a blocklist of suppressed insight keys.
 * Blocked insights are never generated again for the same dataset.
 */
class InsightStore {
public:
    InsightStore() = default;

    void loadUseful(const string& filename);
    void loadBlocked(const string& filename);

    void saveUseful(const vector<Insight>& insights,
                    const string& filename);

    void saveBlocked(const string& filename);

    void addBlockedKey(const string& key);
    bool isBlocked(const string& key) const;

    vector<Insight> filterBlocked(const vector<Insight>& insights) const;

    const vector<Insight>& getUseful() const;

private:
    vector<Insight> usefulInsights;
    unordered_set<string> blockedKeys;
};

#endif
