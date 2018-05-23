//
// Created by karolina on 13.05.18.
//

#ifndef AAL_IDASTAR_H
#define AAL_IDASTAR_H

#include "BSF.h"

namespace algorithm {

    class IDAStar : private BSF {

    public:
        class State : public BSF::State {

        private:
            int h;
            int depth;

        public:
            int getH() const;

            int getDepth() const;

            State(const generator::Test &t);

            State(const State &other);

            State(const BSF::State &other);

            int calculate_h();

            IDAStar::State move(int from, int dest, int color) const;

            void operator=(const State &other);
        };

    public:
        IDAStar(const generator::Test &t);

        int run();

        int search(State &current, int g, int bound);

    private:
        IDAStar::State node;

        std::vector<IDAStar::State> generate_children(State &s);

    };

}

#endif //AAL_IDASTAR_H
