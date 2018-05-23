//
// Created by karolina on 13.05.18.
//

#include <algorithm>
#include <queue>
#include <iostream>
#include "IDAStar.h"

#define FOUND 0
namespace algorithm {

    IDAStar::IDAStar(const generator::Test &t):node(t), BSF(t) {}

    IDAStar::State::State(const IDAStar::State &other): BSF::State(other) {
        h = calculate_h();
        depth = other.depth;
    }
    IDAStar::State::State(const BSF::State &other): BSF::State(other){
        h = calculate_h();
        depth = 0;
    }
    IDAStar::State::State(const generator::Test &t): BSF::State(t){
        h = calculate_h();
        depth = 0;
    }


    int IDAStar::State::calculate_h() {
        h = 0;
        for(auto& bucket : buckets)
            h += bucket.invalid.size();

        return h;
    }

    int IDAStar::run() {
        int bound = node.getH();
        int iteration = 0;
        while(1){
           if (iteration > iteration_limit)
               return -1;
            visited.clear();
            int t = search(node,0,bound);
            if (t == FOUND)
                return node.getDepth();
            if (t == INT32_MAX)
                return -1;
            bound = t;
            iteration++;
        }
    }

    std::vector<IDAStar::State> IDAStar::generate_children(IDAStar::State &s) {
        std:: vector<IDAStar::State> states;
        for (int i = 0; i < s.buckets.size(); i++) {
            int from = i;
            for (auto &&invalid: s.buckets[i].invalid) {
                int left_dest, right_dest;
                if (--from < 0)
                    left_dest = (int) s.buckets.size() - 1;
                else left_dest = from;
                from++;
                right_dest = ++from % (int) s.buckets.size();
                if (!s.buckets[left_dest].isFull())
                    states.push_back(s.move(i, left_dest, invalid));

                if (!s.buckets[right_dest].isFull() )
                    states.push_back(s.move(i, right_dest, invalid));
            }
        }
        return states;
    }

    int IDAStar::search(IDAStar::State& current, int g, int bound) {

        int f = g + current.getH();
        int i = 0;
        if (f > bound)
            return f;
        if(current.resolved()) {
            node = current;
            return FOUND;
        }
        int min = INT32_MAX;

        std::vector<IDAStar::State> children = generate_children(current);
        for (auto &next: children){
     //       std::cout<<"iteration: "<< i<<std::endl;
            if (visited.count(next.hash())){
                break;
            }
            int t = search(next, g+1,bound);
            if (t == FOUND)
                return FOUND;
            if (t < min)
                min = t;
            i++;
        }
        return min;
    }

    IDAStar::State IDAStar::State::move(int from, int dest, int color) const {
        IDAStar::State new_State(*this);
        auto it = new_State.buckets[from].invalid.find(color);
        new_State.buckets[from].invalid.erase(it);
        auto it2 =  std::find(new_State.buckets[from].blocks.begin(),new_State.buckets[from].blocks.end(), color);
        new_State.buckets[from].blocks.erase(it2);

        new_State.buckets[dest].accepted.insert(color);
        new_State.buckets[dest].blocks.push_back(color);
        new_State.calculate_h();
        new_State.depth ++;
    //    std::cout<<"from: " <<from <<" dest: " << dest <<" color: " <<color<< std::endl;
        return new_State;
    }


    int IDAStar::State::getH() const {
        return h;
    }

    int IDAStar::State::getDepth() const {
        return depth;
    }

   void IDAStar::State::operator=(const IDAStar::State &other) {

        depth = other.depth;
        h = other.h;
        buckets.clear();
       for (auto &&bucket : other.buckets)
           buckets.push_back(bucket);
    }

}