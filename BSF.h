//
// Created by karolina on 12.05.18.
//

#ifndef AAL_BSF_H
#define AAL_BSF_H

#include "generator.h"

namespace algorithm {

    class BSF {

    public:
        class Bucket:public generator::Bucket{
        public:
            std::size_t hash();
            Bucket(const generator::Bucket &other);
            Bucket(Bucket &other);
            Bucket(Bucket &&other);

            bool operator==(const Bucket& other) const;
            bool operator!=(const Bucket& other) const;
        };

        class State {
        public:
            std::vector<BSF::Bucket> buckets;

            State(const State& other);
            State(const generator::Test &t);

            bool operator==(const State &other) const;
            bool operator!=(const State &other) const;

            bool resolved() const;

            State move (int from, int dest, int color) const;
            std::size_t hash();
        };
        BSF(const generator::Test &t);
        const int iteration_limit = 100000;
        virtual int run();
        virtual void saveToFile(std::string file_name);

    protected:
        std::set<size_t> visited;
        int k;
        std::vector<std::vector<int>> track;
        int result;
    public:
        int getResult() const;

    private:
        State initial;
        virtual std::vector<State> generate_states(State& s);
    };
}

#endif //AAL_BSF_H
