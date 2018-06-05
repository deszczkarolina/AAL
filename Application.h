//
// Created by karolina on 23.05.18.
//

#ifndef AAL_APPLICATION_H
#define AAL_APPLICATION_H

#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <chrono>
#include "generator.h"
#include "Naive.h"
#include "BSF.h"
#include "IDAStar.h"

class Application {

    using Result = std::pair<long long int,std::chrono::duration<double>>;

    enum Flags{
        m1, m2, m3, n, k, min_p, max_p, min_free_space,
        max_free_space, input_file, all, tests_num, n_step, k_step, repeats, BSF, Naive, IDA
    };

    class CommandLineReader{
    private:
        std::stringstream in;
        int parameters_number;

    public:
        CommandLineReader(int argc, char **argv);
        void print_usage();
        template <typename T>
        std::istream& operator>>(T& out) {
            T test;
            in >> test;
            out = test;
            return in;
        }
    };


private:
     CommandLineReader reader;
    std::map<std::string, Flags> flags;
public:
    Application(int argc, char **argv);
    int start();
    void print_results(std::vector<Result> result, int unresolved,int step);

    void run_test_from_file();
    void run_test_from_command_line_parameters();
    void generate_test_cases();
    Result get_average(std::vector<Result> res);
    };
long double calc_q(int n, int n_med, double time, double time_med);

#endif //AAL_APPLICATION_H
