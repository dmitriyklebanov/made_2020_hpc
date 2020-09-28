#pragma once

#include <iostream>
#include <vector>
#include <utility>

template<typename TFunc>
class Tester {
 private:
    int _total_ = 0;
    int _success_ = 0;
    std::vector<size_t> _failed_ids_;

    TFunc _test_function_;
 public:
    explicit Tester(TFunc func);

    template<typename... Args>
    void execute(Args&&... args);

    void printStatistics() const;
    int getTotal() const;
    int getSuccess() const;
};

template<typename TFunc>
Tester<TFunc>::Tester(TFunc func)
    : _test_function_(func)
{
}

template<typename TFunc>
template<typename... Args>
void Tester<TFunc>::execute(Args&&... args) {
    _total_++;
    bool res = _test_function_(std::forward<Args>(args)...);
    _success_ += res;
    if (!res) {
        _failed_ids_.push_back(_total_);
    }
}

template<typename TFunc>
void Tester<TFunc>::printStatistics() const {
    std::cout << "total tests: " << _total_ << "\n";
    std::cout << "success: " << _success_ << "\n";
    if (_total_ != _success_) {
        std::cout << "ids of failed tests: ";
        for (const int id : _failed_ids_) {
            std::cout << id << " ";
        }
    }
}

template<typename TFunc>
int Tester<TFunc>::getTotal() const {
    return _total_;
}

template<typename TFunc>
int Tester<TFunc>::getSuccess() const {
    return _success_;
}
