#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
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
  int current() {
    if (i_ == v_.end()) return -1;
    return *i_;
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
	  const size_t top_k,
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

  size_t k = std::min(top_k, result.size());
  std::partial_sort(result.begin(),
		    result.begin() + k,
		    result.end(),
		    [](const std::pair<int, int>& a, const std::pair<int, int>& b) -> bool {
		      if (a.second != b.second) return b.second - a.second < 0;
		      return b.first - a.first > 0;
		    });
  return true;
}

bool daat(std::vector<PostingList> &posting_lists,
	  const size_t top_k,
	  std::vector<std::pair<int, int>>& result) {
  result.clear();

  for (auto posting_list = posting_lists.begin();
       posting_list != posting_lists.end();
       ++posting_list) {
    posting_list->init();
  }

  auto queue_comp = [](const std::pair<int, int>& a,
		       const std::pair<int, int>& b) -> bool {
    if (a.second != b.second) return b.second - a.second < 0;
    return b.first - a.first > 0;
  };

  std::priority_queue<std::pair<int, int>,
                      std::vector<std::pair<int, int>>,
		      decltype(queue_comp)> que(queue_comp);

  while (true) {
    size_t end_list = 0;
    int minv = std::numeric_limits<int>::max();
    for (auto posting_list = posting_lists.begin();
	 posting_list != posting_lists.end();
	 ++posting_list) {
      int t = posting_list->current();
      if (t == -1) {
	end_list++;
	continue;
      }
      minv = std::min(minv, t);
    }
    if (end_list == posting_lists.size()) {
      break;
    }

    int minv_count = 0;
    for (auto posting_list = posting_lists.begin();
	 posting_list != posting_lists.end();
	 ++posting_list) {
      int t = posting_list->current();
      if (t != minv) continue;
      minv_count++;
      posting_list->next();
    }

    if (que.empty() || que.size() < top_k) {
      que.push(std::make_pair(minv, minv_count));
      continue;
    }
    if (que.top().second > minv_count) {
      continue;
    }
    que.pop();
    que.push(std::make_pair(minv, minv_count));
  }

  while(!que.empty()) {
    std::pair<int, int> t = que.top();
    que.pop();
    result.push_back(t);
  }

  std::reverse(result.begin(), result.end());
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

  result.clear();
  daat(ps, 5, result);
  std::cout << result.size() << std::endl;
  for (auto i = result.begin(); i != result.end(); ++i) {
    std::cout << i->first << " " << i->second << std::endl;
  }
  return 0;
}
