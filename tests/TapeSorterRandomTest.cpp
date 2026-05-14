#include <random>

#include "common/TapeSorterBase.hpp"

struct RandomSortParams {
    uint32_t seed;
    std::size_t size;
    std::size_t memory_elements;
};

class TapeSorterRandomTest : public TapeSorterBase, public ::testing::TestWithParam<RandomSortParams> { };

TEST_P(TapeSorterRandomTest, Sort_RandomNumbers_ReturnsSortedTape) {
    auto [seed, size, memory_elements] = GetParam();

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int32_t> dist(-100000, 100000);

    std::vector<int32_t> input(size);
    std::generate(input.begin(), input.end(), [&]{ return dist(rng); });

    std::vector<int32_t> expected = input;
    std::sort(expected.begin(), expected.end());

    SetupSorter(std::move(input), memory_elements * sizeof(int32_t));

    auto output = Sort();

    ExpectTapeEq(*output, expected);
}

INSTANTIATE_TEST_SUITE_P(
    RandomSorts,
    TapeSorterRandomTest,
    ::testing::Values(
        RandomSortParams{42, 100, 10},
        RandomSortParams{37, 100000, 37},
        RandomSortParams{13, 50000, 100},
        RandomSortParams{67, 312312, 37},
        RandomSortParams{1234, 77777, 5},
        RandomSortParams{2384759, 8547, 4},
        RandomSortParams{97, 23451, 11},
        RandomSortParams{52, 4562, 13},
        RandomSortParams{37, 999999, 3},
        RandomSortParams{37, 999999, 7},
        RandomSortParams{37, 999999, 10000},
        RandomSortParams{37, 999999, 100000}
    )
);