import std;
import timer;

std::mt19937 rng(std::random_device{}()); //randomness 

std::vector<int> make_ints(std::size_t n, int low, int high){
    std::uniform_int_distribution<int> d(low, high);
    std::vector<int> v(n);
    for(auto& x : v) x = d(rng);
    return v;
}

std::string make_word(){
    std::uniform_int_distribution<int> d('a', 'z');
    std::string str;
    for(int i = 0; i < 20;++i){
        str += char(d(rng));
    }
    return str;
}

int main(){
    const std::size_t VECTOR_SIZE = 100000000;
    
    /*---- find() ----*/
    //7 in the middle (find())
    {
        auto v = make_ints(VECTOR_SIZE, 8, 1000);
        v[VECTOR_SIZE/2] = 7;
        auto it = run_timer("find 7 in vector<int>, hit at middle", 3, VECTOR_SIZE,
            [&]{ return std::find(v.begin(), v.end(), 7); });
        std::cout << "  index " << (it - v.begin()) << std::endl;
    }
    //no 7 (find())
    {
        auto v = make_ints(VECTOR_SIZE, 8, 1000);
        auto it = run_timer("find 7 in vector<int>, miss", 3, (VECTOR_SIZE/2),
            [&]{ return std::find(v.begin(), v.end(), 7); });
        std::cout << "  index " << (it - v.begin()) << std::endl;
    }

    /*---- find_if() ----*/
    // x < 7 in the middle (find_if)
    {
        auto v = make_ints(VECTOR_SIZE, 8, 1000);
        v[VECTOR_SIZE/2] = 3;
        auto it = run_timer("find_if x<7 in vector<int>, hit at middle", 3, VECTOR_SIZE,
            [&]{ return std::find_if(v.begin(), v.end(),
                                    [](int x){ return x < 7; }); });
        std::cout << "  index " << (it - v.begin()) << "\n";
    }
    //no x<7 (find_if())
    {
        auto v = make_ints(VECTOR_SIZE, 7, 1000);
        v[VECTOR_SIZE/2] = 8;
        auto it = run_timer("find_if x<7 in vector<int>, miss", 3, (VECTOR_SIZE/2),
            [&]{ return std::find_if(v.begin(), v.end(),
                                    [](int x){ return x < 7; }); });
        std::cout << "  index " << (it - v.begin()) << "\n";
    }
   
    /*---- find() string in vector<string> ----*/
    {
        std::vector<std::string> words;
        for(std::size_t i =  0 ; i < 1000000;++i){
            words.push_back(make_word());
        }
        std::string target(20,'X');
        auto it = run_timer("find string in vector<string>, miss", 3, (words.size()/2),
            [&]{ return std::find(words.begin(), words.end(), target);});
        std::cout << " index " << (it-words.begin()) << std::endl;
    }

    /*---- find() middle string in vector<string> ----*/
    {
        std::vector<std::string> words;
        for(std::size_t i =  0 ; i < 1000000;++i){
            words.push_back(make_word());
        }
        std::string target(20,'X');
        words[words.size()/2] = target;
        auto it = run_timer("find string in vector<string>, hit in middle", 3, words.size(),
            [&]{ return std::find(words.begin(), words.end(), target);});
        std::cout << " index " << (it-words.begin()) << std::endl;
    }

    return 0;
}