#pragma once

#include <gtest/gtest.h>

#include "common/InMemoryTape.hpp"
#include "common/InMemoryTapeFactory.hpp"
#include "lib/tape/ITape.hpp"
#include "lib/sorter/TapeSorter.hpp"

class TapeSorterBase {
protected:
    void SetupSorter(std::vector<int32_t>&& input, std::size_t memory_limit) { 
        auto factory = std::make_unique<InMemoryTapeFactory>(std::move(input));
        sorter_ = std::make_unique<TapeSorter<>>(std::move(factory), memory_limit);
    }

    std::unique_ptr<ITape> Sort() {
        return sorter_->Sort();
    }

    static void ExpectTapeEq(ITape& tape, const std::vector<int32_t>& expected) {
        for (std::size_t i = 0; i < expected.size(); ++i) {
            ASSERT_FALSE(tape.IsEnd()) << "Tape ended early at index: " << i;
            EXPECT_EQ(tape.Read(), expected[i]) << "Mismatch at index: " << i;
            tape.MoveNext();
        }
        EXPECT_TRUE(tape.IsEnd()) << "Tape has extra elements";
    }

private:
    std::unique_ptr<TapeSorter<>> sorter_;
};