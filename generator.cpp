//
// Created by karolina on 11.05.18.
//

#include <algorithm>
#include <iostream>
#include "generator.h"
#include <fstream>
#include <string>

namespace generator {

    Bucket::Bucket(const std::vector<int> &blocks, const int capacity, int k) : blocks(blocks), capacity(capacity) {
        std::vector<int> count_repeated(k, 0);

        for (auto block: this->blocks)
            count_repeated[block]++;
        for (int i = 0; i < count_repeated.size(); i++)
            if (count_repeated[i] > 1) {
                accepted.insert(i);
                for (int j = 1; j < count_repeated[i]; j++)
                    invalid.insert(i);
              }
            else if (count_repeated[i] == 1)
                accepted.insert(i);
    }

    Bucket::Bucket(const Bucket &other) : capacity(other.capacity) {
        for (auto block: other.blocks)
            blocks.push_back(block);
        for (auto accepted_block: other.accepted)
            accepted.insert(accepted_block);
        for (auto invalid_block: other.invalid)
            invalid.insert(invalid_block);
    }

   Bucket::Bucket(Bucket &&other){
     capacity = other.capacity;
     accepted = std::move(other.accepted);
     invalid = std::move(other.invalid);
     blocks = std::move(other.blocks);
    }

    Bucket::Bucket() : capacity(0){}

    bool Bucket::isFull() {
        return blocks.size() == capacity;
    }

    Bucket::Bucket(const int capacity, const int free_space, const std::vector<int> blocks, int k) {
        std::vector<int> count_repeated(k+1, 0);
        this->capacity = capacity;
        auto end = blocks.begin() + std::min((int)blocks.size(), capacity-free_space);
        for (auto it = blocks.begin(); it != end; it ++)
            this->blocks.push_back(*it);

        for (auto &block: this->blocks)
            count_repeated[block]++;
        for (int i = 0; i < count_repeated.size(); i++)
            if (count_repeated[i] > 1){
                    accepted.insert(i);
                    for (int j = 1; j < count_repeated[i]; j++)
                        invalid.insert(i);
                }
            else if (count_repeated[i] == 1)
                accepted.insert(i);
    }

    Test::Test(std::string filename) {
        std::fstream file;
        file.open(filename, std::fstream::in);
        if (file.is_open()) {
            int n = 0;
            file >> n;
            file >> k;
            for (int i = 0; i < n; i++) {
                int capacity, blocksCount;
                file >> capacity >> blocksCount;
                std::vector<int> blocks;
                for (int j = 0; j < blocksCount; j++) {
                    int block;
                    file >> block;
                    blocks.push_back(block);
                }
                buckets.emplace_back(Bucket(blocks, capacity, k));
            }
            file.close();
        }
    }

    void Test::saveToFile(std::string filename) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << buckets.size() << " " << k << std::endl;
            for (auto &&bucket : buckets) {
                file << bucket.capacity<< " " << bucket.blocks.size() << std::endl;
                for (auto &&block : bucket.blocks) {
                    file << block << std::endl;
                }
            }
            file.close();
        } else {
            throw std::ios_base::failure("Could not open file to write the test to");
        }
    }

   std::ostream &operator<<(std::ostream &out, const Test &t) {
        out << t.buckets.size() << " " << t.k << std::endl;
        for (auto &&bucket : t.buckets) {
            out <<bucket.capacity<< " " << bucket.blocks.size() << std::endl;
            for (auto &&block : bucket.blocks) {
                out << block << std::endl;
            }
        }
        return out;
    }

    std::istream &operator>>(std::istream &in, Test &t) {
        int n = 0;
        in >> n;
        in >> t.k;
        for (int i = 0; i < n; i++) {
            int capacity, blockCount;
            in >> capacity >> blockCount;
            std::vector<int> blocks;
            for (int j = 0; j < blockCount; j++) {
                int block;
                in >> block;
                blocks.push_back(block);
            }
            t.buckets.push_back(Bucket(blocks, capacity, t.k));
        }
        return in;
    }

    Test generate_test(int n, int k, int min_p, int max_p, int min_free_space, int max_free_space) {
        Test test = Test();
        test.setK(k);

        std::random_device generator;
        std::mt19937 gen(generator());
        std::uniform_int_distribution<int> capacity_distribution(min_p, max_p);
        std::vector<int> blocks;
        for (int i = 0; i < n; ++i) {
            blocks.clear();
            std::vector<int> blocks = generate_colors(k);
            int capacity = capacity_distribution(gen);

            std::uniform_int_distribution<int> free_space_distribution(min_free_space, std::min(max_free_space, capacity));
            int space = free_space_distribution(gen);

            test.addBucket(Bucket(capacity, space, blocks,k));
        }
        shuffle_test(test);
        return test;
    }

    Test &shuffle_test(Test &t) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, (int)t.buckets.size() - 1);

        for (int i = 0; i < 100; ++i) {
            int x = dis(gen);
            Bucket &a = t.buckets[x];
            Bucket &b = t.buckets[(x + 1) % t.buckets.size()];

            if (b.isFull() or a.blocks.size() == 0) {
                continue;
            }
            int tmp = a.blocks.back();

            auto it = a.invalid.find(tmp);
            if (it != a.invalid.end())
                a.invalid.erase(tmp);
            else
                a.accepted.erase(tmp);

            auto it2 = b.accepted.find(tmp);
            if (it2 == b.accepted.end())
                b.accepted.insert(tmp);
            else
                b.invalid.insert(tmp);
            a.blocks.pop_back();
            b.blocks.push_back(tmp);
        }
        return t;
    }

    std::vector<int>  generate_colors(int k) {
        std::random_device generator;
        std::mt19937 gen(generator());
        std::uniform_int_distribution<int> color_distribution(1,k);
        std::vector<int> results;
        for (int i = 0; i < k; i++)
            results.emplace_back(color_distribution(gen));
        return results;
    }

    int Test::getK() const {
        return k;
    }

    std::vector<Bucket>Test::getBuckets() const  {
        return buckets;
    }

    void Test::setK(int k) {
        Test::k = k;
    }


    void Test::addBucket(const Bucket& bucket) {
        buckets.emplace_back(bucket);
    }


}