#pragma once

#include <algorithm>
using namespace std;

template <typename It>
class IteratorRange {
public:
  IteratorRange(It first, It last) : first(first), last(last) {
  }

  It begin() const {
    return first;
  }

  It end() const {
    return last;
  }

  size_t size() const {
    return distance(first, last);
  }

private:
  It first, last;
};

template <typename Container>
auto Head(Container& c, int top) {
  //auto size_ = min<size_t>(max(top, 0), c.size());
    return IteratorRange(begin(c), begin(c) + min<size_t>(max(top, 0), c.size()));
}
