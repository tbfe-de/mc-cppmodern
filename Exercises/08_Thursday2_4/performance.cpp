#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstring>
#include <functional>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <random>
#include <vector>
#include <set>

constexpr auto TOTAL_CHUNKS = 5'000;
constexpr auto CHUNK_SIZE     = 200;

#if defined(USE_TREE_SET)
#include <set>
#elif defined(USE_HASH_SET)
#include <unordered_set>
#elif defined(USE_VECTOR)
#include <vector>
#else
#error "one of USE_TREE_SET or USE_HASH_SET or USE_VECTOR must be defined"
#endif
#ifndef DUPES_RATE
#error "DUPES_RATE needs to define (= approximate rate of duplicates)"
#endif

namespace sc = std::chrono;

int main() {
	std::mt19937 gen;
	std::vector<std::mt19937::result_type> in;
	constexpr auto total_in = std::size_t{TOTAL_CHUNKS*CHUNK_SIZE};
	constexpr auto high_val = total_in/DUPES_RATE;
	std::uniform_int_distribution<> dist(1, high_val);
	in.reserve(std::size_t{total_in});
	std::generate_n(std::back_inserter(in), total_in,
                       	[&gen, &dist]{ return dist(gen); });
	
	std::vector<sc::nanoseconds> timings;
	#if defined(USE_TREE_SET)
	std::set<unsigned int> data;
	#elif defined(USE_HASH_SET)
	std::unordered_set<unsigned int> data;
	#elif defined(USE_VECTOR)
	std::vector<unsigned int> data;
	#endif
	auto data_extractor = in.begin();
	#if !defined(USE_VECTOR)
	auto data_inserter = std::inserter(data, data.end());
	#else
	auto data_inserter = std::back_inserter(data);
	#endif
	for (auto i = 0; i < TOTAL_CHUNKS; ++i) {
		assert((in.end() - data_extractor) >= CHUNK_SIZE);
		const auto started = sc::high_resolution_clock::now();
		#if defined(USE_VECTOR)
		const auto old_size = data.size();
		#endif
		// ----------------------------------------------------
		std::copy_n(data_extractor, CHUNK_SIZE, data_inserter);
		data_extractor += CHUNK_SIZE;
		// ----------------------------------------------------
		#if defined(USE_VECTOR)
		const auto data_mid = data.begin() + old_size;
		std::sort(data_mid, data.end());
		std::inplace_merge(data.begin(), data_mid, data.end());
		const auto data_end = std::unique(data.begin(), data.end());
		data.erase(data_end, data.end());
		#endif
		const auto ended = sc::high_resolution_clock::now();
		timings.emplace_back(ended - started);
	}
	#if defined(USE_VECTOR)
	assert(std::is_sorted(std::begin(data), std::end(data)));
	#endif
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
