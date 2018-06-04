//
// Created by karolina on 11.05.18.
//

#include <algorithm>
#include <iostream>
#include "Naive.h"
#include <fstream>
namespace algorithm {

    bool Naive::resolved() {
        for (auto &bucket: buckets)
            if (!bucket.invalid.empty())
                return false;
        return true;
    }

    Naive::Naive(const generator::Test &t) : k(t.getK()), result(-1) {
        for (auto &&bucket: t.getBuckets())
            buckets.push_back(generator::Bucket(bucket));
    }

/*    std::ostream &operator<<(std::ostream &out, const Naive &n) {
        for (int i = 0; i < n.buckets.size(); i++) {
            std::cout << "\n Bucket id: " << i << "\t Capacity: " << n.buckets[i].capacity;
            std::cout << "\t blocks number: " << n.buckets[i].blocks.size() << "\n";
            std::cout << "block's colours: \n";
            for (auto &block: n.buckets[i].blocks)
                std::cout << block << ", ";
            std::cout << "accepted colours: \n";
            for (auto &block: n.buckets[i].accepted)
                std::cout << block << ", ";
            std::cout << "invalid blocks: \n";
            for (auto &block: n.buckets[i].invalid)
                std::cout << block << ", ";
        }
        return out;
    }*/

    int move_left(int from, int dest, Naive &alg) {
        int moves = 0;
        while (from != dest) {
            if (alg.buckets[--from].isFull())
                return 0;
            if (from < 0)
                from = (int) alg.buckets.size() - 1;
            moves++;
        }
        return moves;
    }

    int move_right(int from, int dest, Naive &alg) {
        int moves = 0;
        while (from != dest) {
            if (alg.buckets[++from].isFull())
                return 0;
            from %= alg.buckets.size();
            moves++;
        }
        return moves;
    }

    int move(int from, int dest, int color_index, Naive &alg) {
        int moves = 0;
        if (abs(from - dest) <= alg.buckets.size() / 2) {
            if (dest < from) {
                moves = move_left(from, dest, alg);
                if (!moves)
                    moves = move_right(from, dest, alg);
            } else {
                moves = move_right(from, dest, alg);
                if (!moves)
                    moves = move_left(from, dest, alg);
            }
        }
        else {
            if (dest < from) {
                moves = move_right(from, dest, alg);
                if (!moves)
                    moves = move_left(from, dest, alg);
            } else {
                moves = move_left(from, dest, alg);
                if (!moves)
                    moves = move_right(from, dest, alg);
            }
        }
        if(moves){
            auto invalid_it = alg.buckets[from].invalid.find(color_index);
            alg.buckets[from].invalid.erase(invalid_it);
            auto blocks_it = std::find(alg.buckets[from].blocks.begin(),alg.buckets[from].blocks.end(), color_index);
            if(blocks_it == alg.buckets[from].blocks.end())
                std::cerr << "something went wrong";
            alg.buckets[from].blocks.erase(blocks_it);

            alg.buckets[dest].accepted.insert(color_index);
            alg.buckets[dest].blocks.push_back(color_index);
        }
        return moves;
    }

    int Naive::run() {
        if (resolved()) {
            result = 0;
            return 0;
        }
        int total_moves = 0;
        int iteration = 0;
        while (true) {
           bool move_made = false;
            for (int i = 0; i < buckets.size(); i++) {
                int moves_per_block = move_invalid_blocks(i, *this);
                if (moves_per_block)
                    move_made = true;
                total_moves += moves_per_block;
            }
            if ( iteration >= iteration_limit || !move_made ) {
                total_moves = -1;
                break;
            }
            if (resolved()) {
                result = total_moves;
                return result;
            }
            iteration++;
        }
        result = total_moves;
        return result;
    }



    int move_invalid_blocks(int from, Naive &alg) {
        int moves_per_bucket = 0;
        int moves_per_block = 0;
        auto invalid_colors = alg.buckets[from].invalid;
        for (auto& invalid: invalid_colors) {
            for (int i = 0; i < alg.buckets.size(); i++) {
                auto accept = alg.buckets[i].accepted.find(invalid);
                if (accept == alg.buckets[i].accepted.end() && !alg.buckets[i].isFull()) {
                    moves_per_block = move(from, i, invalid, alg);
                    alg.track.push_back({from,i,invalid, moves_per_block});
                    if (moves_per_block) {
                        moves_per_bucket += moves_per_block;
                        break;
                    }
                }
            }
        }
        return moves_per_bucket;
    }

    void Naive::saveToFile(std::string file_name) {
        std::ofstream output;
        output.open(file_name, std::ios::out);
        output<< "algorithm: NAIVE"<<std::endl;
        output<< "parameters: "<<std::endl;
        output<< "n: "<<buckets.size()<<" k: " << k << std::endl;


        if (result == -1)
            output<< "solution not found"<<std::endl;
        else {
            output<< "solution found in "<< result << " iterations" <<std::endl;
            for (auto vec : track)
                output << "from: " << vec[0] << "  to: " << vec[1] << "  block: " << vec[2] << " moves made: " << vec[3]<<std::endl;

        }
        output<<std::endl;
        for (int i = 0; i < buckets.size(); i++){
            output<< "bucket " <<i+1<<" capacity: " << buckets[i].capacity<< std::endl;
            for (auto &block: buckets[i].blocks)
                output<<block<< ", ";
            output<<std::endl;
        }
        output.close();
    }

    int Naive::getResult() const {
        return result;
    }
}
