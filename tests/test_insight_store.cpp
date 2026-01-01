#include <gtest/gtest.h>
#include "InsightStore.h"

#include <fstream>
#include <vector>
#include <string>

TEST(InsightStoreTest, SaveAndLoadUsefulRoundTrip) {
    std::string filename = "test_useful_insights.csv";

    // Build some test insights
    Insight a;
    a.key = "K1";
    a.description = "Mac users study at night";
    a.score = 88;
    a.support = 12;
    a.population = 40;

    Insight b;
    b.key = "K2";
    b.description = "Windows users like mornings";
    b.score = 72;
    b.support = 8;
    b.population = 40;

    // Save
    InsightStore store;
    std::vector<Insight> toSave = { a, b };
    store.saveUseful(toSave, filename);

    // Load into a fresh store
    InsightStore loaded;
    loaded.loadUseful(filename);

    const auto& loadedVec = loaded.getUseful();
    ASSERT_EQ(loadedVec.size(), 2u);
    EXPECT_EQ(loadedVec[0].key, "K1");
    EXPECT_EQ(loadedVec[0].description, "Mac users study at night");
    EXPECT_EQ(loadedVec[1].key, "K2");
    EXPECT_EQ(loadedVec[1].description, "Windows users like mornings");
}

TEST(InsightStoreTest, SaveLoadBlockedAndFilter) {
    std::string blockedFile = "test_blocked_keys.txt";

    // Save a couple blocked keys
    InsightStore store;
    store.addBlockedKey("BAD1");
    store.addBlockedKey("BAD2");
    store.saveBlocked(blockedFile);

    // Load into a fresh store
    InsightStore loaded;
    loaded.loadBlocked(blockedFile);

    EXPECT_TRUE(loaded.isBlocked("BAD1"));
    EXPECT_TRUE(loaded.isBlocked("BAD2"));
    EXPECT_FALSE(loaded.isBlocked("GOODKEY"));

    // filterBlocked should remove BAD1 but keep GOOD
    Insight bad;
    bad.key = "BAD1";

    Insight good;
    good.key = "GOOD";

    std::vector<Insight> input = { bad, good };
    auto output = loaded.filterBlocked(input);

    ASSERT_EQ(output.size(), 1u);
    EXPECT_EQ(output[0].key, "GOOD");
}
