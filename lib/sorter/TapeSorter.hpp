#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <functional>

#include "tape/factory/ITapeFactory.hpp"

template<typename Comparator = std::less<int32_t>>
class TapeSorter {
public:
    TapeSorter(std::unique_ptr<ITapeFactory> factory, std::size_t memory_limit);

    std::unique_ptr<ITape> Sort();
private:
    std::vector<std::unique_ptr<ITape>> SortingPhase(); 
    std::unique_ptr<ITape> MergingPhase(std::vector<std::unique_ptr<ITape>>&& tapes);
    std::unique_ptr<ITape> CopyToOutput(ITape& res);

    std::vector<int32_t> ReadChunk(ITape& input);
    std::unique_ptr<ITape> CreateTape(const std::vector<int32_t>& chunk);
    std::unique_ptr<ITape> MergeTwo(ITape& lhs, ITape& rhs);


    static constexpr std::size_t kElementSize = sizeof(int32_t);

    std::unique_ptr<ITapeFactory> factory_;
    std::size_t chunk_size_;
    Comparator comp_;
};


template<typename Comparator>
TapeSorter<Comparator>::TapeSorter(std::unique_ptr<ITapeFactory> factory, std::size_t memory_limit)
: factory_(std::move(factory))
, chunk_size_(memory_limit / kElementSize)
, comp_()
{
    if (chunk_size_ < 2) {
        throw TapeException("Error: memory limit too small.");
    }
}


template<typename Comparator>
std::unique_ptr<ITape> TapeSorter<Comparator>::Sort() {
    std::vector<std::unique_ptr<ITape>> tapes = SortingPhase();
    std::unique_ptr<ITape> res_tape = MergingPhase(std::move(tapes));
    return CopyToOutput(*res_tape);
}

template<typename Comparator>
std::vector<std::unique_ptr<ITape>> TapeSorter<Comparator>::SortingPhase() {
    std::vector<std::unique_ptr<ITape>> tapes;
    std::unique_ptr<ITape> input = factory_->CreateInput();

    while (!input->IsEnd()) {
        std::vector<int32_t> chunk = ReadChunk(*input);
        std::sort(chunk.begin(), chunk.end(), comp_);
        tapes.push_back(CreateTape(chunk));
    }

    return tapes;
}

template<typename Comparator>
std::unique_ptr<ITape> TapeSorter<Comparator>::MergingPhase(std::vector<std::unique_ptr<ITape>>&& tapes) {
    if (tapes.empty()) {
        return factory_->CreateTemp();
    }

    while (tapes.size() > 1) {
        std::vector<std::unique_ptr<ITape>> next_tapes;

        for (std::size_t i = 0; i < tapes.size(); i += 2) {
            if (i + 1 == tapes.size()) {
                next_tapes.push_back(std::move(tapes[i]));
            } else {
                next_tapes.push_back(MergeTwo(*tapes[i], *tapes[i + 1]));
            }
        }

        tapes = std::move(next_tapes);
    }

    return std::move(tapes.front());
}

template<typename Comparator>
std::unique_ptr<ITape> TapeSorter<Comparator>::CopyToOutput(ITape& res) {
    std::unique_ptr<ITape> output = factory_->CreateOutput();

    while (!res.IsEnd()) {
        output->Write(res.Read());
        output->MoveNext();
        res.MoveNext();
    }

    output->Rewind();
    return output;
}

template<typename Comparator>
std::vector<int32_t> TapeSorter<Comparator>::ReadChunk(ITape& input) {
    std::vector<int32_t> chunk;
    chunk.reserve(chunk_size_);

    while (!input.IsEnd() && chunk.size() < chunk_size_) {
        chunk.push_back(input.Read());
        input.MoveNext();
    }

    return chunk;
}

template<typename Comparator>
std::unique_ptr<ITape> TapeSorter<Comparator>::CreateTape(const std::vector<int32_t>& chunk) {
    std::unique_ptr<ITape> res = factory_->CreateTemp();

    for (auto val : chunk) {
        res->Write(val);
        res->MoveNext();
    }

    res->Rewind();
    return res;
}

template<typename Comparator>
std::unique_ptr<ITape> TapeSorter<Comparator>::MergeTwo(ITape& lhs, ITape& rhs) {
    std::unique_ptr<ITape> res = factory_->CreateTemp();

    while (!lhs.IsEnd() && !rhs.IsEnd()) {
        int32_t left = lhs.Read();
        int32_t right = rhs.Read();
        if (comp_(left, right)) {
            res->Write(left);
            lhs.MoveNext();
        } else {
            res->Write(right);
            rhs.MoveNext();
        }

        res->MoveNext();
    }

    while (!lhs.IsEnd()) {
        res->Write(lhs.Read());
        res->MoveNext();
        lhs.MoveNext();
    }

    while (!rhs.IsEnd()) {
        res->Write(rhs.Read());
        res->MoveNext();
        rhs.MoveNext();
    }

    res->Rewind();
    return res;
}