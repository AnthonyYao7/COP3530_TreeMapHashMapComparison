#include <iostream>
#include <map>
#include <unordered_map>
#include <random>
#include <chrono>
#include <vector>


class Timer // RAII timing object for easy timing
{
    std::chrono::high_resolution_clock::time_point start;
    std::string name;
public:
    explicit Timer(const std::string& name)
    {
        start = std::chrono::high_resolution_clock::now();
        this->name = name;
    }
    ~Timer()
    {
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::cout << "Time elapsed for " << name << ": " << duration.count() << std::endl;
    }
};


template <typename T>
concept Insertable = requires(T t, std::pair<int, int> key_value)
{
    t.insert(key_value);
};

template <typename T>
concept Searchable = requires(T t, int key)
{
    t.find(key);
};

template <typename T>
concept Deletable = requires(T t, int key)
{
    t.erase(key);
};


template <typename T>
concept MapADT = Insertable<T> && Searchable<T> && Deletable<T>;


template <MapADT T>
void test(T& map_structure, const std::vector<std::pair<int, int>>& data, const std::string& name)
{
    // will test insert, search, and delete
    {
        Timer timer(name + " insert " + std::to_string(data.size()));

        for (const auto & i : data)
            map_structure.insert(i);
    }

    {
        Timer timer(name + " search " + std::to_string(data.size()));

        for (const auto & i : data)
            map_structure.find(i.first);
    }

    {
        Timer timer(name + " delete " + std::to_string(data.size()));

        for (const auto & i : data)
            map_structure.erase(i.first);
    }
}



int main() {
    // setup
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 1<<15);

    std::vector<std::pair<int, int>> nums10k;

    for (size_t i = 0; i < 10000; ++i)
    {
        nums10k.emplace_back(dist(rng), dist(rng));
    }

    std::vector<std::pair<int, int>> nums100k;

    for (size_t i = 0; i < 100000; ++i)
    {
        nums100k.emplace_back(dist(rng), dist(rng));
    }

    {
        std::map<int, int> tree_map;

        test(tree_map, nums10k, "tree map");

        std::unordered_map<int, int> hash_map;

        test(hash_map, nums10k, "hash map");
    }

    {
        std::map<int, int> tree_map;

        test(tree_map, nums100k, "tree map");

        std::unordered_map<int, int> hash_map;

        test(hash_map, nums100k, "hash map");
    }


    return 0;
}
