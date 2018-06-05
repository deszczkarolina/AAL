//
// Created by karolina on 11.05.18.
//

#ifndef AAL_NAIVE_H
#define AAL_NAIVE_H

#include <set>
#include "generator.h"

namespace algorithm {

    class Naive {

    public:
        const int iteration_limit = 1000000;
        std::vector<generator::Bucket> buckets;
    private:
        int k;
        std::vector<std::vector<int>> track;
        int result;

    public:
        Naive(const generator::Test &t);
        int run();
        bool resolved();
        void saveToFile(std::string file_name);

        friend int move(int from, int dest, int color_index, Naive& alg);
        friend int move_right(int from, int dest, Naive& alg);
        friend int move_left(int from, int dest, Naive& alg);

        friend std::ostream &operator<<(std::ostream &out, const Naive &n);
        friend int move_invalid_blocks(int from, Naive &alg);
        int getResult() const;
    };

}
#endif //AAL_NAIVE_H
