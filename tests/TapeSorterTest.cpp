#include "common/TapeSorterBase.hpp"

class TapeSorterTest : public TapeSorterBase, public ::testing::Test { };

TEST_F(TapeSorterTest, Sort_SortsThreeElements_ReturnSortedTape) {
    SetupSorter({3, 1, 2}, 2 * sizeof(int32_t));
    auto output = Sort();
    ExpectTapeEq(*output, {1, 2, 3});
}

TEST_F(TapeSorterTest, Sort_AlreadySortedInput_ReturnsSortedTape) {
    SetupSorter({1, 2, 3, 4, 5}, 2 * sizeof(int32_t));
    auto output = Sort();
    ExpectTapeEq(*output, {1, 2, 3, 4, 5});
}

TEST_F(TapeSorterTest, Sort_SingleElement_ReturnsSingleElement) {
    SetupSorter({67}, 2 * sizeof(int32_t));
    auto output = Sort();
    ExpectTapeEq(*output, {67});
}

TEST_F(TapeSorterTest, Sort_EmptyArray_ReturnsSingleElement) {
    SetupSorter({}, 100 * sizeof(int32_t));
    auto output = Sort();
    ExpectTapeEq(*output, {});
}

TEST_F(TapeSorterTest, Sort_DuplicateElements_ReturnsSortedTape) {
    SetupSorter({3, 1, 3, 2, 1}, 3 * sizeof(int32_t));
    auto output = Sort();
    ExpectTapeEq(*output, {1, 1, 2, 3, 3});
}

TEST_F(TapeSorterTest, Sort_NegativeNumbers_ReturnsSortedTape) {
    SetupSorter({-3, 1, -1, 2, -2}, 2 * sizeof(int32_t));
    auto output = Sort();
    ExpectTapeEq(*output, {-3, -2, -1, 1, 2});
}

TEST_F(TapeSorterTest, Sort_AllSameElements_ReturnsSortedTape) {
    SetupSorter({1, 1, 1, 1}, 100 * sizeof(int32_t));
    auto output = Sort();
    ExpectTapeEq(*output, {1, 1, 1, 1});
}

TEST_F(TapeSorterTest, Sort_100numbers_ReturnsSortedTape) {
    std::vector<int32_t> input;
    for (int32_t i = 99; i >= 0; --i) {
        input.push_back(i);
    }
    std::vector<int32_t> expected = input;
    std::sort(expected.begin(), expected.end());

    SetupSorter(std::move(input), 2 * sizeof(int32_t));
    auto output = Sort();
    ExpectTapeEq(*output, expected);
}

TEST_F(TapeSorterTest, Constructor_MemoryLimitTooSmall_ThrowsTapeException) {
    EXPECT_THROW(SetupSorter({1, 2, 3}, sizeof(int32_t)), TapeException);
}