#include "gtest/gtest.h"
#include "near_set.h"

TEST(NearSet, Simple) {
    NearSet nearSet;
    for (int i = 0; i < 10; ++i) {
        nearSet.Add(i);
    }
    std::vector<int64_t> kek = nearSet.FindNear(2, 2);
    std::vector<int64_t> heh = std::vector<int64_t>{0, 1, 2, 3, 4};
    ASSERT_EQ(kek, heh);

    for (int i = 200000; i >= 0; --i) {
        nearSet.Add(i);
    }

    kek = nearSet.FindNear(50000, 2);
    heh = std::vector<int64_t>{49998, 49999, 50000, 50001, 50002};
    ASSERT_EQ(kek, heh);

    for (int i = 0; i <= 210000; ++i) {
        if (i %2 == 0) {
            nearSet.Remove(i);
        }
    }

    for (int i = 0;i < 1000; ++i) {
        kek = nearSet.FindNear(60000 + 4 * i, 6);
        heh = std::vector<int64_t>{59995 + 4 * i, 59997 + 4 * i, 59999 + 4 * i, 60001 + 4 * i, 60003 + 4 * i, 60005 + 4 * i};
    }
    ASSERT_EQ(kek, heh);

    kek = nearSet.FindNear(6, 0);
    heh = std::vector<int64_t >{};
    ASSERT_EQ(kek, heh);

}
