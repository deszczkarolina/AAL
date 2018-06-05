//
// Created by karolina on 12.05.18.
//
#include <algorithm>
#include <queue>
#include <iostream>

#include "BSF.h"
#include <fstream>
namespace algorithm {

    BSF::State::State(const algorithm::BSF::State &other) {
        for (auto &&bucket : other.buckets)
            buckets.push_back(bucket);
    }

    BSF::State::State(const generator::Test &t) {
        for (auto &bucket : t.getBuckets())
            buckets.push_back(algorithm::BSF::Bucket(bucket));
    }

    bool BSF::State::operator==(const BSF::State &other) const {
        if (buckets.size() != other.buckets.size())
            return false;
        for (int i = 0; i < buckets.size(); i++)
            if (buckets[i] != other.buckets[i])
                return false;
        return true;
    }

    bool BSF::State::operator!=(const BSF::State &other) const {
        return !(*this == other);
    }

    bool BSF::State::resolved() const {
        for (auto &bucket : buckets)
            if (!bucket.invalid.empty())
                return false;
        return true;
    }

    BSF::State BSF::State::move(int from, int dest, int color) const {

        State new_State(*this);
        auto it = new_State.buckets[from].invalid.find(color);
        new_State.buckets[from].invalid.erase(it);
        auto it2 = std::find(new_State.buckets[from].blocks.begin(), new_State.buckets[from].blocks.end(), color);
        new_State.buckets[from].blocks.erase(it2);
        auto it3 = std::find(new_State.buckets[dest].blocks.begin(), new_State.buckets[dest].blocks.end(), color);
        if (it3 == new_State.buckets[dest].blocks.end())
            new_State.buckets[dest].accepted.insert(color);
        else
            new_State.buckets[dest].invalid.insert(color);
        new_State.buckets[dest].blocks.push_back(color);

        return new_State;
    }

    std::size_t BSF::State::hash() {
        std::size_t seed = buckets.size();
        for (auto &i : buckets) {
            seed ^= i.hash() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }

    BSF::BSF(const generator::Test &t) : initial(t), k(t.getK()), result(-1) {}


    int BSF::run() {
        std::queue<std::pair<int,BSF::State>> generated_states;
        generated_states.push(std::make_pair(0, initial));
        int iteration = 0;

        while (!generated_states.empty()) {
            State current = generated_states.front().second;
            int iteration_id = generated_states.front().first;
            generated_states.pop();

            if (current.resolved()) {
                result = iteration_id;
                return iteration_id;
            }

            std::vector<State> child_states = generate_states(current);
            for (auto &child : child_states)
                if (!visited.count(child.hash())) {
                    auto state = std::make_pair(iteration_id + 1, child);
                    generated_states.push(state);
                    visited.insert(state.second.hash());
                }

            iteration++;

            if (iteration > iteration_limit)
                return -1;

        }
        if (generated_states.empty())
            return -1;
    }

    void BSF::saveToFile(std::string file_name) {
        std::ofstream output;
        output.open(file_name, std::ios::out);
        output<< "algorithm: BSF"<<std::endl;
        output<< "parameters: "<<std::endl;

        output<< "n: "<<initial.buckets.size()<<" k: " << k << std::endl;

        for (auto vec : track)
            output << "from: " << vec[0] << "  to: " << vec[1] << "  block: " << vec[2] << std::endl;

        output.close();
    }

    std::vector<BSF::State> BSF::generate_states(BSF::State &s) {
        std::vector<State> states;
        for (int i = 0; i < s.buckets.size(); i++) {
            int from = i;
            auto invalid_colors = s.buckets[i].invalid;
            for (auto &invalid: invalid_colors) {
                int left_dest, right_dest;
                if (--from < 0)
                    left_dest = (int) s.buckets.size() - 1;
                else left_dest = from;
                from++;
                right_dest = ++from % (int) s.buckets.size();

                if (!s.buckets[left_dest].isFull()) {
                    states.push_back(s.move(i, left_dest, invalid));
                    track.push_back({i, left_dest, invalid});
                }
                if (!s.buckets[right_dest].isFull()) {
                    states.push_back(s.move(i, right_dest, invalid));
                    track.push_back({i, right_dest, invalid});
                }
            }
        }
        return states;
    }

    int BSF::getResult() const {
        return result;
    }


    std::size_t BSF::Bucket::hash() {
        std::size_t seed = blocks.size();
        for (auto &i : blocks)
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }

    BSF::Bucket::Bucket(const generator::Bucket &other) {
        capacity = other.capacity;
        for (auto block: other.blocks)
            blocks.push_back(block);
        for (auto accepted_block: other.accepted)
            accepted.insert(accepted_block);
        for (auto invalid_block: other.invalid)
            invalid.insert(invalid_block);

    }

    bool BSF::Bucket::operator==(const BSF::Bucket &other) const {
        return accepted == other.accepted && invalid == other.invalid && capacity == other.capacity;
    }

    bool BSF::Bucket::operator!=(const BSF::Bucket &other) const {
        return !(*this == other);
    }

    BSF::Bucket::Bucket(BSF::Bucket &other) {
        capacity = other.capacity;
        for (auto block: other.blocks)
            blocks.push_back(block);
        for (auto accepted_block: other.accepted)
            accepted.insert(accepted_block);
        for (auto invalid_block: other.invalid)
            invalid.insert(invalid_block);
    }

    BSF::Bucket::Bucket(BSF::Bucket &&other) {
        capacity = other.capacity;
        accepted = std::move(other.accepted);
        invalid = std::move(other.invalid);
        blocks = std::move(other.blocks);
    }
}

