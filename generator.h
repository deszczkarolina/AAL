//
// Created by karolina on 11.05.18.
//

#ifndef AAL_GENERATOR_H
#define AAL_GENERATOR_H


#include <vector>
#include <string>
#include <set>


namespace generator {

    class Bucket {
    public:
        Bucket();
        Bucket(const std::vector<int> &blocks, const int capacity, int k);
        Bucket(const Bucket &other);
        Bucket(Bucket &&other);
        Bucket(const int capacity, const int free_space, const std::vector<int> blocks, int k);
        bool isFull();

        std::vector<int> blocks;
        std::set<int> accepted;
        std::multiset<int> invalid;
        int capacity;
    };

    class Test {

    private:
        int k;

    public:
        std::vector<Bucket> buckets;

        Test() = default;
        Test(std::string filename);

        void setK(int k);
        int getK() const;
        void addBucket(const Bucket &bucket);
        std::vector<Bucket> getBuckets() const;
        void saveToFile(std::string filename);

        friend std::ostream &operator<<(std::ostream &out, const Test &t);
        friend std::istream &operator>>(std::istream &in, Test &t);
    };

    Test generate_test(int n, int k, int min_p, int max_p, int min_free_space, int max_free_space);
    std::vector<int> generate_colors(int k) ;
    Test &shuffle_test(Test &t);
}


#endif //AAL_GENERATOR_H
