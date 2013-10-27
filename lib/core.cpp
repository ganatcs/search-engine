#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

class PostingList {
public:
  PostingList(const std::vector<int>& v);
  void init() {
    i_ = v_.begin();
  }
  int next() {
    if (i_ == v_.end()) return -1;
    int ret = *i_;
    ++i_;
    return ret;
  }
  int next(int id) {
    while (i_ != v_.end()) {
      if (*i_ < id) {
	++i_;
      } else {
	int ret = *i_;
	++i_;
	return ret;
      }
    }
    return -1;
  }
private:
  std::vector<int> v_;
  std::vector<int>::const_iterator i_;
};

PostingList::PostingList(const std::vector<int>& v): v_(v), i_(v_.begin()) {}

void taat_intersect(PostingList& in,
		    const std::vector<std::pair<int, int>>& accumlator,
		    std::vector<std::pair<int, int>>& next_accumlator) {
  in.init();
  auto i = in.next();
  auto j = accumlator.begin();
  while (i != -1 && j != accumlator.end()) {
    if (i < j->first) {
      next_accumlator.push_back(std::make_pair(i, 1));
      i = in.next();
    } else if(i == j->first) {
      next_accumlator.push_back(std::make_pair(j->first, j->second + 1));
      i = in.next();
      ++j;
    } else {
      next_accumlator.push_back(*j);
      ++j;
    }
  }
  while (i != -1) {
    next_accumlator.push_back(std::make_pair(i, 1));
    i = in.next();
  }
  while (j != accumlator.end()) {
    next_accumlator.push_back(*j);
    ++j;
  }
}

bool taat(std::vector<PostingList> &posting_lists,
	  const int top_k,
	  std::vector<std::pair<int, int>>& result) {
  result.clear();

  for (auto posting_list = posting_lists.begin();
       posting_list != posting_lists.end();
       ++posting_list) {
    posting_list->init();
    std::vector<std::pair<int, int>> next_accumlator;

    taat_intersect(*posting_list, result, next_accumlator);
    next_accumlator.swap(result);
  }

  std::partial_sort(result.begin(),
		     result.begin() + top_k,
		     result.end(),
		     [](const std::pair<int, int>& a, const std::pair<int, int>& b) -> bool{
		       if (a.second != b.second) return b.second - a.second < 0;
		       return b.first - a.first > 0;
		     });
  return true;
}

int main() {
  std::vector<PostingList> ps;
  ps.push_back(PostingList({1, 2, 3, 4, 5}));
  ps.push_back(PostingList({1, 3, 5}));

  std::vector<std::pair<int, int>> result;
  taat(ps, 5, result);

  for (auto i = result.begin(); i != result.end(); ++i) {
    std::cout << i->first << " " << i->second << std::endl;
  }
  return 0;
}
