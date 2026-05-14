#include "DelayedTape.hpp"

DelayedTape::DelayedTape(std::unique_ptr<ITape> tape, const TapeDelays& delays)
: tape_(std::move(tape))
, delays_(delays)
{ }

void DelayedTape::Sleep(std::chrono::milliseconds time)  {
    std::this_thread::sleep_for(time);
}

int32_t DelayedTape::Read() {
    Sleep(delays_.read);

    return tape_->Read();
}

void DelayedTape::Write(int32_t value) {
    Sleep(delays_.write);

    tape_->Write(value);
}

void DelayedTape::MoveNext() {
    Sleep(delays_.move);

    tape_->MoveNext();
}

void DelayedTape::MoveBack() {
    Sleep(delays_.move);

    tape_->MoveBack();
}

void DelayedTape::Rewind() {
    Sleep(delays_.rewind);

    tape_->Rewind();
}

bool DelayedTape::IsEnd() const {
    return tape_->IsEnd();
}