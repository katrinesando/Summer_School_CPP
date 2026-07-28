export module timer;
import std;
export namespace timer{
    template<class F>
    auto timer(F f){
        auto start = std::chrono::steady_clock::now();
        auto res = f();
        auto stop = std::chrono::steady_clock::now();
        double ms = std::chrono::duration<double, std::milli>(stop-start).count();
        return std::pair{res, ms};
    }

    template<class F>

    auto run_timer(const std::string& name, int runs, std::size_t n_total, std::size_t n_scanned, F f) { 
        std::vector<double> times; 
        decltype(f()) last{}; 
        std::cout << "=== " << name << " ===\n "; 
        for (int i = 0; i < runs; ++i) { auto [res, ms] = timer(f); 
            std::cout << "run " << (i + 1) << ": " << std::fixed << std::setprecision(1) << ms << " ms "; 
            times.push_back(ms); last = res; 
        } 
        std::sort(times.begin(), times.end()); 
        double median = times[times.size() / 2]; 
        std::cout << "\n median: " << median << " ms spread: " << times.front() << " - " << times.back() << " ms\n" << " cost per element scanned: " << std::setprecision(2) << (median * 1e6 / n_scanned) << " ns\n\n"; 
        return last; 
    }
}

