#include "FileTape.hpp"

FileTape::FileTape(const std::filesystem::path& path) { 
    std::ofstream(path, std::ios::binary | std::ios::app);

    file_.open(path, std::ios::in | std::ios::out | std::ios::binary);
    if (!file_.is_open()) {
        throw TapeException("Error: cannot open file: " + path.string() + ".");
    }
    
    file_.seekg(0, std::ios::end);
    size_ = static_cast<std::size_t>(file_.tellg()) / kElementSize;
}

int32_t FileTape::Read() {
    if (IsEnd()) {
        throw TapeException("Error: read called at the end.");
    }

    MoveToPos();
    
    int32_t value;
    if (!file_.read(reinterpret_cast<char*>(&value), kElementSize)) {
        throw TapeException("Error: read failed.");
    }

    return value;
}

void FileTape::Write(int32_t value) {
    MoveToPos();

    if (!file_.write(reinterpret_cast<const char*>(&value), kElementSize)) {
        throw TapeException("Error: write failed.");
    }
    file_.flush();

    size_ = std::max(size_, pos_ + 1);
}


void FileTape::MoveNext() {
    if (IsEnd()) {
        throw TapeException("Error: MoveNext called at the end.");
    }
    
    ++pos_;
}

void FileTape::MoveBack() {
    if (pos_ == 0) {
        throw TapeException("Error: MoveBack called at the beginning.");
    }

    --pos_;
}

void FileTape::Rewind() {
    pos_ = 0;
}

bool FileTape::IsEnd() const {
    return pos_ >= size_;
}

void FileTape::MoveToPos() {
    auto offset = static_cast<std::streamoff>(pos_ * kElementSize);
    file_.seekg(offset, std::ios::beg);
    file_.seekp(offset, std::ios::beg);
}