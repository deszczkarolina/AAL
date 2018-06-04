//
// Created by karolina on 23.05.18.
//

#include <cmath>
#include "Application.h"

Application::Application(int argc, char **argv) : reader(argc, argv) {
    flags["m1"] = Application::Flags::m1;
    flags["m2"] = Application::Flags::m2;
    flags["m3"] = Application::Flags::m3;
    flags["-n"] = Application::Flags::n;
    flags["-k"] = Application::Flags::k;
    flags["-min_p"] = Application::Flags::min_p;
    flags["-max_p"] = Application::Flags::max_p;
    flags["-min_free_space"] = Application::Flags::min_free_space;
    flags["-max_free_space"] = Application::Flags::max_free_space;
    flags["-f"] = Application::Flags::input_file;
    flags["-a"] = Application::Flags::all;
    flags["-s_n"] = Application::Flags::n_step;
    flags["-s_k"] = Application::Flags::k_step;
    flags["-t"] = Application::Flags::tests_num;
    flags["-r"] = Application::Flags::repeats;
    flags["-B"] = Application::Flags::BSF;
    flags["-N"] = Application::Flags::Naive;
    flags["-I"] = Application::Flags::IDA;

}

int Application::start() {
    std::string mode;
    reader >> mode;
    reader.print_usage();
    switch (flags[mode]) {
        case Flags::m1 :
            run_test_from_file();
            break;
        case Flags::m2 :
            run_test_from_command_line_parameters();
            break;
        case Flags::m3 :
            generate_test_cases();
            break;
        default:
            reader.print_usage();
            break;
    }
}

void Application::run_test_from_file() {

    std::string filename, output_filename;
    bool print_all = false;
    std::string flag;

    reader >> flag;
    reader >> filename;

    if (flags[flag] != Flags::input_file) {
        std::cout << "incorrect parameter, -t expected";
        return;
    }
    reader >> flag;
    if (flags[flag] == Flags::all) {
        print_all = true;
        reader >> output_filename;
    }

    generator::Test test(filename);
    algorithm::Naive N(test);
    algorithm::BSF B(test);
    algorithm::IDAStar A(test);

    auto start_Naive = std::chrono::steady_clock::now();
    N.run();
    auto stop_Naive = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_Naive = stop_Naive - start_Naive;
    if (N.getResult() == -1)
        std::cout << "Naive algorithm hasn't found the solution" << std::endl;
    else
        std::cout << "Naive algorithm resolved in: " << N.getResult() << " iterations" << std::endl;
    std::cout << "Naive algorithm run in: " << elapsed_Naive.count() << std::endl;

    auto start_BSF = std::chrono::steady_clock::now();
    B.run();
    auto stop_BSF = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_BSF = stop_BSF - start_BSF;
    if (B.getResult() == -1)
        std::cout << "BSF algorithm hasn't found the solution" << std::endl;
    else
        std::cout << "BSF algorithm resolved in: " << B.getResult() << " iterations" << std::endl;
    std::cout << "BSF algorithm run in: " << elapsed_BSF.count() << std::endl;

    auto start_IDAstar = std::chrono::steady_clock::now();
    A.run();
    auto stop_IDAstar = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_IDAstar = stop_IDAstar - start_IDAstar;
    if (A.getResult() == -1)
        std::cout << "IDAstar algorithm hasn't found the solution" << std::endl;
    else
        std::cout << "IDAstar algorithm resolved in: " << A.getResult() << " iterations" << std::endl;
    std::cout << "IDAstar algorithm run in: " << elapsed_IDAstar.count() << std::endl;
    if (print_all) {
        N.saveToFile("Naive_" + output_filename);
        B.saveToFile("BSF_" + output_filename);
        A.saveToFile("IDAStar_" + output_filename);
    }
}

void Application::run_test_from_command_line_parameters() {

    bool print_all = false;
    std::string flag, output_filename;
    int n, k, max_cap = 10, min_cap = 3, max_free = 4, min_free = 1;


    reader >> flag;
    if (flags[flag] != Flags::n) {
        std::cout << "incorrect parameter, -n expected";
        return;
    }
    reader >> n;

    reader >> flag;
    if (flags[flag] != Flags::k) {
        std::cout << "incorrect parameter, -k expected";
        return;
    }
    reader >> k;

    if (flags[flag] == Flags::max_p)
        reader >> max_cap;

    if (flags[flag] == Flags::min_p)
        reader >> min_cap;

    if (flags[flag] == Flags::max_free_space)
        reader >> max_free;

    if (flags[flag] == Flags::min_free_space)
        reader >> min_free;

    if (flags[flag] == Flags::all) {
        print_all = true;
        reader >> output_filename;
    }

    generator::Test test = generator::generate_test(n, k, min_cap, max_cap, min_free, max_free);


    algorithm::Naive N(test);

    algorithm::BSF B(test);
    algorithm::IDAStar A(test);

    auto start_Naive = std::chrono::steady_clock::now();
    N.run();
    auto stop_Naive = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_Naive = stop_Naive - start_Naive;
    if (N.getResult() == -1)
        std::cout << "Naive algorithm hasn't found the solution" << std::endl;
    else
        std::cout << "Naive algorithm resolved in: " << N.getResult() << " iterations" << std::endl;
    std::cout << "Naive algorithm run in: " << elapsed_Naive.count() << std::endl;

    auto start_BSF = std::chrono::steady_clock::now();
    B.run();
    auto stop_BSF = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_BSF = stop_BSF - start_BSF;
    if (B.getResult() == -1)
        std::cout << "BSF algorithm hasn't found the solution" << std::endl;
    else
        std::cout << "BSF algorithm resolved in: " << B.getResult() << " iterations" << std::endl;
    std::cout << "BSF algorithm run in: " << elapsed_BSF.count() << std::endl;

    auto start_IDAstar = std::chrono::steady_clock::now();
    A.run();
    auto stop_IDAstar = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_IDAstar = stop_IDAstar - start_IDAstar;
    if (A.getResult() == -1)
        std::cout << "IDAstar algorithm hasn't found the solution" << std::endl;
    else
        std::cout << "IDAstar algorithm resolved in: " << A.getResult() << " iterations" << std::endl;
    std::cout << "IDAstar algorithm run in: " << elapsed_IDAstar.count() << std::endl;
    if (print_all) {
        N.saveToFile("Naive_" + output_filename);
        B.saveToFile("BSF_" + output_filename);
        A.saveToFile("IDAStar_" + output_filename);
    }
}

void Application::generate_test_cases() {
    std::string flag;
    int t, n, k, step_n = 0, step_k = 10, unresolved, step_numb = 21;
    std::vector<Result> results;
    bool k_changes = true;
    std::string alg;

    reader >> alg;
    if (flags[alg] != Flags::BSF && flags[alg] != Flags::Naive && flags[alg] != Flags::IDA) {
        std::cout << "incorrect parameter, B/N/I expected";
        return;
    }

    reader >> flag;
    if (flags[flag] != Flags::repeats) {
        std::cout << "incorrect parameter, -r expected";
        return;
    }
    reader >> step_numb;

    reader >> flag;
    if (flags[flag] != Flags::tests_num) {
        std::cout << "incorrect parameter, -t expected";
        return;
    }
    reader >> t;

    reader >> flag;
    if (flags[flag] != Flags::n) {
        std::cout << "incorrect parameter, -n expected";
        return;
    }
    reader >> n;

    reader >> flag;
    if (flags[flag] != Flags::k) {
        std::cout << "incorrect parameter, -k expected";
        return;
    }
    reader >> k;

    reader >> flag;
    if (flags[flag] == Flags::n_step) {
        reader >> step_n;
        step_k = 0;
        k_changes = false;
    }
    if (step_n != 0) {
        reader >> flag;
       if (flags[flag] == Flags::k_step) {
            std::cout << "only one value can change \n";
        }
    } else {
        reader >> flag;
        if (flags[flag] == Flags::k_step)
            reader >> step_k;
    }


    switch (flags[alg]) {
        case Naive:
            for (int i = 0; i < step_numb; i++) {
                unresolved = 0;
                for (int j = 0; j < t; j++) {
                    generator::Test test = generator::generate_test(n + i * step_n, k + i * step_k, 3, 10, 1, 4);
                    algorithm::Naive N(test);

                    auto start_Naive = std::chrono::steady_clock::now();
                    N.run();
                    auto stop_Naive = std::chrono::steady_clock::now();
                    std::chrono::duration<double> elapsed_Naive = stop_Naive - start_Naive;
                    if (N.getResult() == -1)
                        unresolved++;
                    else
                        results.push_back(std::make_pair(N.getResult(), elapsed_Naive));
                }
                if (k_changes)
                    print_results(results, unresolved, k+i*step_k);
                else
                    print_results(results, unresolved, n+i*step_n);
            }
            break;
        case BSF:
            for (int i = 0; i < step_numb; i++) {
                unresolved = 0;
                for (int j = 0; j < t; j++) {
                    generator::Test test = generator::generate_test(n + i * step_n, k + i * step_k, 3, 10, 1, 4);
                    algorithm::BSF B(test);

                    auto start_BSF = std::chrono::steady_clock::now();
                    B.run();
                    auto stop_BSF = std::chrono::steady_clock::now();
                    std::chrono::duration<double> elapsed_BSF = stop_BSF - start_BSF;
                    if (B.getResult() == -1)
                        unresolved++;
                    else
                        results.push_back(std::make_pair(B.getResult(), elapsed_BSF));
                }
                if (k_changes)
                    print_results(results, unresolved, k+i*step_k);
                else
                    print_results(results, unresolved, n+i*step_n);
            }
            break;
        case IDA:
            for (int i = 0; i < step_numb; i++) {
                unresolved = 0;
                for (int j = 0; j < t; j++) {
                    generator::Test test = generator::generate_test(n + i * step_n, k + i * step_k, 3, 10, 1, 4);
                    algorithm::IDAStar A(test);

                    auto start_IDAstar = std::chrono::steady_clock::now();
                    A.run();
                    auto stop_IDAstar = std::chrono::steady_clock::now();
                    std::chrono::duration<double> elapsed_IDAstar = stop_IDAstar - start_IDAstar;
                    if (A.getResult() == -1)
                        unresolved++;
                    else
                        results.push_back(std::make_pair(A.getResult(), elapsed_IDAstar));
                }
                if (k_changes)
                    print_results(results, unresolved, k+i*step_k);
                else
                    print_results(results, unresolved, n+i*step_n);
            }
            break;
        default: break;
    }
}

void Application::print_results(std::vector<Result> result, int unres, int step) {
    Result avg = get_average(result);
    std::cout << step << "\t \t " << avg.second.count() << "\t \t \t" << avg.first << "\t \t \t \t \t"
              << unres  << "\n";
}


long double calc_q(int n, int n_med, double time, double time_med) {
    long double result;
    result = (time * 1/n_med) / (time_med * 1/n);
    return result;
}

Application::Result Application::get_average(std::vector<Application::Result> res) {
    Result average;
    for (auto &&testCase : res) {
        average.first += testCase.first;
        average.second += testCase.second;
    }
    average.first /= res.size();
    average.second /= res.size();
    return average;
}


Application::CommandLineReader::CommandLineReader(int argc, char **argv) : parameters_number(argc - 1) {
    for (int i = 1; i < argc; ++i) {
        in << argv[i] << " ";
    }
}

void Application::CommandLineReader::print_usage() {
    std::cout << R"(
Usage:
    -m[1|2|3] - specifies mode
Modes:
    m1: read test from file, write output to standard output
        Example:
            m1 -f input.txt -a output.txt
        Flags:
            -f      input file
            --a     prints detail information about execution of each algorithm to
                    files Naive_otput, BSF_output, IDA_output

    m2: generate random test and solve it.
         Flags:
            -n              - number of buckets (default=15)
            -k              - number of colors (default=10)
           --min_p          - minimal capacity a bucket can have (default=3)
           --max_p          - maximum capacity a bucket can have (default=10)
           --min_free_space - minimum free space a bucket can have (default=1)
           --max_free-_pace - maximum free space a bucket can have (default=4)
           --shuffle-moves  - how many moves wil be made to shuffle
                              the color bricks in test (default=100)
         Example:
            m2 -n 100 -k 20 --min-p 5 --shuffle-moves 1000
    -m3: generate n random tests and solve them.
         Then write statistics for each of the algorithms.
         Flags:
            -t              - number of test cases
            -n              -  n
            -k              -  k
    )";
    std::cout << std::endl;
}


