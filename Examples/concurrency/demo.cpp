// http://coliru.stacked-crooked.com/a/8e21a5839c14175d

#include <iostream>

#define PX(expr)\
    ((void)(std::cout << __FUNCTION__  << ':' << __LINE__\
                      << "\t" #expr " --> " << (expr)\
                      << std::endl))

#include <array>
std::array<double, 100000> data;

#include <numeric>

void synchr_demo() {
        auto result = std::accumulate(data.begin(), data.end(), 0.0);
        PX(result);
}

#include <future>
void async_2_demo() {
        const auto split_point = data.begin() + data.size()/2;
        auto lower = std::async(std::launch::async,
                [split_point] {
                        return std::accumulate(data.begin(), split_point, 0.0);
        });
        auto upper = std::async(std::launch::async,
                [split_point] {
                        return std::accumulate(split_point, data.end(), 0.0);
        });
        auto result = lower.get() + upper.get();
        PX(result);
}

constexpr auto NTHREAD = 4;
void async_n_demo() {
        std::array<std::future<double>, NTHREAD> part;
        auto split_point = data.cbegin();
        auto worker = [](auto from, auto upto) {
                return std::accumulate(from, upto, 0.0);
        };
        for (auto &e : part) {
                constexpr long long chunk = (data.size()%4 != 0)
                                        ? data.size()/NTHREAD + 1
                                        : data.size()/NTHREAD;
                const auto next_split = ((data.cend() - split_point) < chunk)
                                        ? data.cend() : split_point + chunk;
                e = std::async(std::launch::async, worker, split_point, next_split);
                split_point = next_split;
        }
        auto result = 0.0;
        for (auto &e : part)
                result += e.get();
        PX(result);
}

#include <atomic>
void async_x_demo() {
        std::atomic_bool die{false};
        auto worker = [&die](auto from, auto upto) {
                return std::accumulate(from, upto, 0.0,
                        [&die](auto partial_sum, auto e) {
                        using namespace std::literals::string_literals;
                                if (die)
                                        throw "stopping prematurely"s;
                                if (e < 0.0) {
                                        die = true;
                                        throw "bad data: "s + std::to_string(e);
                                }
                                return partial_sum + e;;
                        });
        };
        std::array<std::future<double>, NTHREAD> part;
        auto split_point = data.cbegin();
//      data.at(3*data.size()/8) = -1.0;
        for (auto &e : part) {
                constexpr long long chunk = (data.size()%4 != 0)
                                        ? data.size()/NTHREAD + 1
                                        : data.size()/NTHREAD;
                const auto next_split = ((data.cend() - split_point) < chunk)
                                        ? data.cend() : split_point + chunk;
                e = std::async(std::launch::async, worker, split_point, next_split);
                split_point = next_split;
        }
        double result{};
        for (auto &e : part) {
                try {
                        result += e.get();
                }
                catch (const std::string& err) {
                        PX(err);
                }
        }
        if (!die) PX(result);
}

#include <mutex>
void mutex_demo() {
        std::mutex progress_update;
        std::mutex result_update;
        auto progress = data.cbegin();
        auto result = 0.0;
        auto worker = [&]() {
                while (progress < data.end()) {
                        progress_update.lock();
                        const auto split_point = progress;
                        constexpr long long chunk = 4096;
                        const auto next_split = ((data.cend() - split_point) < chunk)
                                                ? data.cend() : split_point + chunk;
                        progress = next_split;
                        progress_update.unlock();
                        const auto r = std::accumulate(split_point, next_split, 0.0);
                        std::unique_lock<std::mutex> lock(result_update);
                        result += r;
                }
        };
        std::array<std::future<void>, NTHREAD> part;
        for (auto &e : part)
                e = std::async(worker);
        for (auto &e : part)
                e.get();
        PX(result);
}

class RingBuffer {
public:
        enum { N = 200 };
        std::array<double, N+1> buf;
        std::size_t p = 0, g = 0;
        bool empty() const { return p == g; }
        bool full() const { return (p+1) % buf.size() == g; }
        std::condition_variable data_available;
        std::condition_variable space_available;
        std::mutex buffer_access;
        std::atomic_bool done{false};
public:
        void close() {
                done = true;
                data_available.notify_all();
        }
        bool put(const double &val) {
                if (done) return false;
                std::unique_lock<std::mutex> lock(buffer_access);
                space_available.wait(lock, [this]{ return !full(); });
                buf[p++] = val; p %= buf.size();
                data_available.notify_one();
                return true;
        }
        bool get(double &val) {
                std::unique_lock<std::mutex> lock(buffer_access);
                data_available.wait(lock, [this]{ return !empty() || done; });
                if (empty()) return false;
                val = buf[g++]; g %= buf.size();
                space_available.notify_one();
                return true;
        }
};

void queue_demo() {
        RingBuffer buf;
        auto worker = [&buf]() {
                auto part_result = 0.0;
                double r;
                while (buf.get(r))
                        part_result += r;
                return part_result;
        };
        std::array<std::future<double>, NTHREAD> part;
        for (auto &e : part)
                e = std::async(std::launch::async, worker);
                // IMPORTANT:  ^^^^^^^^^^^^^^^^^^ (might block otherwise)
        for (auto e : data)
                buf.put(e);
        buf.close();
        double result{};
        for (auto &e : part)
                result += e.get();
        PX(result);
}

void atomic_demo() {
        std::atomic_size_t progress{};
        std::atomic<double> result{};
        auto worker = [&]() {
                while (progress < data.size()) {
                        std::size_t beg = progress.load();
                        std::size_t end;
                        do {
                                constexpr std::size_t chunk = 4096;
                                end = ((data.size() - beg) < chunk)
                                        ? data.size() : beg + chunk;
                        } while (!progress.compare_exchange_weak(beg, end));
                        const auto split_point = data.begin() + beg;
                        const auto next_split = data.begin() + end;
                        const auto r = std::accumulate(split_point, next_split, 0.0);
                        auto x = result.load();
                        do;while (!result.compare_exchange_weak(x, x+r));
                }
        };
        std::array<std::future<void>, NTHREAD> part;
        for (auto &e : part)
                e = std::async(std::launch::async, worker);
        for (auto &e : part)
                e.get();
        PX(result);
}

void task_demo() { // TO-DO
        // implement workers as packaged task,
        // explicitly using promises and futures
}

void thread_demo() { // TO-DO
        // run workers as low-level threads
}

int main() {
        std::cout.setf(std::ios::boolalpha);
        std::iota(data.begin(), data.end(), 0);
        synchr_demo();
//      async_2_demo();
//      async_n_demo();
//      async_x_demo();
//      mutex_demo();
//      queue_demo();
//      atomic_demo();
//      task_demo();
//      thread_demo();
}
