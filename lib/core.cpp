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


bool taat(std::vector<PostingList> &posting_lists,
	  std::vector<std::pair<int, int>>& result) {
  result.clear();
  std::map<int, int> accumulator;
  for (auto posting_list = posting_lists.begin();
       posting_list != posting_lists.end();
       ++posting_list) {
    posting_list->init();
    while (true) {
      int tmp_docid = posting_list->next();
      if (tmp_docid == -1) break;
      accumulator[tmp_docid]++;
    }
  }
  std::vector<std::pair<int, int>> tmp_v;

  for (auto i = accumulator.begin();
       i != accumulator.end();
       ++i) {
    tmp_v.push_back(std::make_pair(i->first, i->second));
  }
  std::sort(tmp_v.begin(),
	    tmp_v.end(),
	    [](const std::pair<int, int>& a, const std::pair<int, int>& b) -> bool{
	      if (a.second != b.second) return b.second - a.second < 0;
	      return b.first - a.first > 0;
	    });

  for (auto i = tmp_v.begin();
       i != tmp_v.end();
       ++i) {
    result.push_back(std::make_pair(i->first, i->second));
  }
  return true;
}

int main() {
  std::vector<PostingList> ps;
  ps.push_back(PostingList({1, 2, 3, 4, 5}));
  ps.push_back(PostingList({1, 3, 5}));

  std::vector<std::pair<int, int>> result;
  taat(ps, result);

  for (auto i = result.begin(); i != result.end(); ++i) {
    std::cout << i->first << " " << i->second << std::endl;
  }
  return 0;
}
