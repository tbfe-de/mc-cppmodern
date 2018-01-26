#include <algorithm>
#include <chrono>
#include <cstring>
#include <functional>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <random>
#include <vector>
#include <set>

#if defined(TWO_HUGE_CHUNKS)
constexpr auto TOTAL_CHUNKS     = 4;
constexpr auto CHUNK_SIZE = 250'000;
#elif defined(FEW_LARGE_CHUNKS)
constexpr auto TOTAL_CHUNKS   = 100;
constexpr auto CHUNK_SIZE  = 10'000;
#elif defined(MANY_SMALL_CHUNKS)
constexpr auto TOTAL_CHUNKS = 5'000;
constexpr auto CHUNK_SIZE     = 200;
#else
#error "none of TWO_HUGE_CHUNKS or FEW_LARGE_CHUNKS or MANY_SMALL_CHUNKS defined"
#endif

namespace sc = std::chrono;

int main() {
	std::vector<sc::nanoseconds> timings;
	std::multiset<unsigned int> data;
	std::mt19937 gen;
	auto data_inserter = std::inserter(data, data.begin());
	for (auto i = 0; i < TOTAL_CHUNKS; ++i) {
		const auto started = sc::high_resolution_clock::now();
		std::generate_n(data_inserter, CHUNK_SIZE, std::ref(gen));
		const auto ended = sc::high_resolution_clock::now();
		timings.emplace_back(ended - started);
	}
	const auto total_duration = std::accumulate(timings.begin(), timings.end(),
			                            decltype(timings)::value_type{});
	using usec_duration = sc::duration<float, std::ratio<1, 1000>>;
	std::cout << "total data : "
		  << data.size() << " items\n";
	std::cout << "first chunk: "
		  << usec_duration{timings.front()}.count()
		  << " msec\n";
	std::cout << "last chunk : "
		  << usec_duration{timings.back()}.count()
		  << " msec\n";
	std::cout << "total time : "
		  << usec_duration{total_duration}.count()
		  << " msec\n";
}
