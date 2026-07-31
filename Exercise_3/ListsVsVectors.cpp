import std;
import timer;

std::vector<int> make_distinct(std::size_t n, unsigned seed){
    std::vector<int> v(n);
    std::iota(v.begin(), v.end(),0);
    std::mt19937 rng(seed);
    std::shuffle(v.begin(), v.end(), rng);
    return v;
}
std::vector<std::size_t> make_positions(std::size_t n, unsigned seed){
    std::vector<std::size_t> pos;
    pos.reserve(n);
    std::mt19937 rng(seed+1);
    for(std::size_t rem = n; rem >0; --rem){
        std::uniform_int_distribution<std::size_t> d(0, rem - 1);
        pos.push_back(d(rng));
    }
    return pos;
}

template<class Container>
std::pair<double, double> experiment(const std::vector<int>& values, const std::vector<std::size_t>& positions){
    Container c;
    auto [size_after_insert, insert_ms] = timer([&]{
        for(int value : values){
            auto pos = c.begin();
            while (pos != c.end() && *pos < value){++pos;}
            c.insert(pos, value);
        }
        return c.size();
    });
    if(!std::is_sorted(c.begin(), c.end())){std::println("Seq not sorted after inserts");}
    auto[size_after_remove, remove_ms] = timer([&]{
        for (std::size_t p : positions){
            auto it = c.begin();
            for(std::size_t step = 0; step < p; ++step){
                ++it;
            }
            c.erase(it);
        }
        return c.size();
    });

    return{insert_ms, remove_ms};
}

template<>
std::pair<double, double> experiment<std::set<int>>(
    const std::vector<int>& values, 
    const std::vector<std::size_t>& positions
) {
    std::set<int> c;

    auto [size_after_insert, insert_ms] = timer([&]{
        for (int value : values) {
            c.insert(value);
        }
        return c.size();
    });

    if (!std::is_sorted(c.begin(), c.end())) {std::println("Seq not sorted after inserts");}

    auto [size_after_remove, remove_ms] = timer([&]{
        for (std::size_t p : positions) {
            auto it = c.begin();
            for (std::size_t step = 0; step < p; ++step) {
                ++it;
            }
            c.erase(it);
        }
        return c.size();
    });

    return {insert_ms, remove_ms};
}

int main(){
    const std::vector<std::size_t> sizes = {50'000,100'000, 200'000};
    const std::vector<unsigned> seeds = {1,2,3};
    std::println("container, phase, n, seed, ms");

    for(auto n : sizes){
        for(auto seed : seeds){
            auto values = make_distinct(n, seed);
            auto positions = make_positions(n, seed);

            auto [v_insert, v_remove] = experiment<std::vector<int>>(values, positions);
            std::println("vector, insert, {}, {}, {:.1f}", n , seed, v_insert);
            std::println("vector, remove, {}, {}, {:.1f}", n , seed, v_remove);

            auto [l_insert, l_remove] = experiment<std::list<int>>(values, positions);
            std::println("list, insert, {}, {}, {:.1f}", n , seed, l_insert);
            std::println("list, remove, {}, {}, {:.1f}", n , seed, l_remove);

            auto [s_insert, s_remove] = experiment<std::set<int>>(values, positions);
            std::println("set, insert, {}, {}, {:.1f}", n, seed, s_insert);
            std::println("set, remove, {}, {}, {:.1f}", n, seed, s_remove);
        }
        std::println("done n = {}", n);
    }
}