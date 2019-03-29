#include <algorithm>
#include <cassert>
#include <future>
#include <iostream>
#include <iterator>
#include <numeric>

template<typename T1, typename Cmp>
void multisort(T1 from, T1 upto, Cmp cmp) {
	auto start{from};
	auto end{upto};
	const auto count{std::distance(start, end)};
	// for optimization consider:
	// if (count <= 1) return;
	auto mid{from + count/2};
	auto lower{std::async(std::launch::async,
                              [start, mid, cmp] { std::sort(start, mid, cmp); })};
	auto upper{std::async(std::launch::async,
                              [mid,   end, cmp] { std::sort(mid,   end, cmp); })};
	lower.get();
	upper.get();
	std::inplace_merge(start, mid, end, cmp);
}

int main() {
	auto sort_cmp{
		[](int lhs, int rhs) { return (rhs < lhs); }
	};
	std::vector<int> testdata(16);
	std::iota(testdata.begin(), testdata.end(), 100);
	assert(std::is_sorted(testdata.cbegin(), testdata.cend()));
	std::copy(testdata.cbegin(), testdata.cend(),
                  std::ostream_iterator<int>(std::cout, "; "));
        std::cout << std::endl;
	std::random_shuffle(testdata.begin(), testdata.end());
	std::copy(testdata.cbegin(), testdata.cend(),
                  std::ostream_iterator<int>(std::cout, "; "));
        std::cout << std::endl;
	multisort(testdata.begin(), testdata.end(), sort_cmp);
//	std::sort(testdata.begin(), testdata.end(), sort_cmp);
	assert(std::is_sorted(testdata.cbegin(), testdata.cend(), sort_cmp));
	std::copy(testdata.cbegin(), testdata.cend(),
                  std::ostream_iterator<int>(std::cout, "; "));
        std::cout << std::endl;
}
