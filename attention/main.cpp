#include <iostream>
#include <ranges>
#include <deque>
#include <format>
#include <fstream>
#include <cmath>
#include <chrono>
#include <random>

#include <vector.h>
#include <test_function.h>

double (*test_func)(const CH::vector<double>& X);

std::string table[] = {"0", "Ackley", "Rastrigin", "HappyCat", "Rosenbrock", "Zakharov", "Michalewicz"};

struct solution_t
{
    CH::vector<double> X;
    double f = 0;
    bool exiled = false;
    solution_t() = default;
};

double AS(size_t N, size_t D, size_t k, double r, double rt, [[maybe_unused]] double Exile_rate, double xl, double xu, [[maybe_unused]] int fn = 1)
{
    /*
     N D k
     x11 x12 x13 ... x1D  --\
     v11 v12 v13 ... v1D    |
     ...                    | <--- 0
     xN1 xN2 xN3 ... xND    |
     vN1 vN2 vN3 ... vND  --/
     x11 x12 x13 ... x1D  --\
     v11 v12 v13 ... v1D    |
     ...                    | <--- 1
     xN1 xN2 xN3 ... xND    |
     vN1 vN2 vN3 ... vND  --/

     ...
     
     x11 x12 x13 ... x1D  --\
     v11 v12 v13 ... v1D    |
     ...                    | <--- k
     xN1 xN2 xN3 ... xND    |
     vN1 vN2 vN3 ... vND  --/
     */
#ifdef SAVE
    std::fstream fs;
    fs.open(std::format("plot/{}/{}_path.txt", table[fn], table[fn]), std::ios::out);
    fs << std::format("{} {} {}", N, D, k) << std::endl;
#endif

    std::deque<solution_t> solutions;
    CH::vector<double> gbest;
    double fgbest;

    /*  init  */
    std::random_device rnd_device;
    std::mt19937 eng { rnd_device() };
    std::uniform_real_distribution<double> Xdist {xl, xu};
    std::uniform_real_distribution<double> dist01 {0, 1};

    fgbest = std::numeric_limits<double>::max();
    solutions.resize(N);
    double fsum = 0.0, fmin = std::numeric_limits<double>::max();//, fmax = std::numeric_limits<double>::lowest();

    for (size_t i = 0; i < N; ++i)
    {
        solutions[i].X.resize(D);
        auto& X = solutions[i].X;
        auto& f = solutions[i].f;
        std::generate(X.begin(), X.end(), [&] { return Xdist(eng); });

#ifdef SAVE
        fs << X << std::endl;
#endif

        f = test_func(X);
        fmin = std::min(fmin, f);
        //fmax = std::max(fmax, f);
        if (fgbest > f)
        {
            gbest = X;
            fgbest = f;
        }
    }
    for (size_t i = 0; i < N; ++i)
    {
        auto& f = solutions[i].f;
        fsum += 1.0 / (f - fmin + 1);
    }
    /*  init  */

    /*  iteration */
    for (size_t iter = 0; iter < k - 1; ++iter)
    {
        auto choose = static_cast<size_t>((N - 1) * dist01(eng));

        [[maybe_unused]]const auto oX = solutions[choose].X;
        [[maybe_unused]]const auto nf = solutions[choose].f;
        [[maybe_unused]]double f_region = std::numeric_limits<double>::max();
        [[maybe_unused]]size_t i_region = 0;
        for (size_t i = 0; i < N; ++i)
        {
            solutions.emplace_back();
            auto& X = solutions.back().X;
            auto& f = solutions.back().f;
            X.resize(D);
            for (size_t d = 0; d < D; ++d)
            {
                //auto pr = sqrt(dist01(eng)) * r; // circumference
                //auto pa = dist01(eng) * 2 * M_PI;
                X[d] = std::max(std::min(oX[d] + dist01(eng) * r, xu), xl);
            }
            f = test_func(X);
            if (f < fgbest)
            {
                gbest = X;
                fgbest = f;
            }
            if (f < f_region)
            {
                i_region = solutions.size() - 1;
                f_region = f;
            }
        }

        /*
        auto dist_threshold = CH::euclidean_dist(oX, solutions[i_region].X);
        solutions.erase(
                std::remove_if(solutions.begin(), 
                    solutions.end(), 
                    [&](const auto& p) { return CH::euclidean_dist(p.X, oX) < dist_threshold; } ), 
                solutions.end());
        */

        //std::sort(solutions.begin(), solutions.end(), [&] (const auto& lhs, const auto& rhs) { return lhs.f < rhs.f; });

        /*while (solutions.size() > N)
            solutions.pop_back();*/
        /*
        fmin = std::numeric_limits<double>::max();
        for (size_t i = 0; i < solutions.size(); ++i)
            fmin = std::min(fmin, solutions[i].f);
        fsum = 0;
        for (size_t i = 0; i < solutions.size(); ++i)
            fsum += 1.0 / (solutions[i].f - fmin + 1);
        */
        r *= rt;
    }
    /*  iteration */
#ifdef SAVE
    fs.close();
#endif
    std::cout << fgbest << std::endl;
    return fgbest;
}

// --N fork_num
// --D dimension
// --k iteration_num
// --test_function fn
// (k+1)*N <= D*10^4
// ./main.elf --N 10 --D 10 --r 1 --rt 0.999 -ER 0.2 --k 1 --test_function Ackley
int main([[maybe_unused]]int argc, char **argv)
{
    using namespace std::chrono_literals;

    int N, D, k, fn;
    double r, rt, ER;

    sscanf(argv[2], "%d", &N);
    sscanf(argv[4], "%d", &D);
    sscanf(argv[6], "%d", &k);
    sscanf(argv[8], "%lf", &r);
    sscanf(argv[10], "%lf", &rt);
    sscanf(argv[12], "%lf", &ER);
    sscanf(argv[14], "%d", &fn);

    switch(fn)
    {
        case 1:
            test_func = static_cast<double (*)(const CH::vector<double>&)>(&Ackley);
            break;

        case 2:
            test_func = static_cast<double (*)(const CH::vector<double>&)>(&Rastrigin);
            break;

        case 3:
            test_func = static_cast<double (*)(const CH::vector<double>&)>(HappyCat);
            break;

        case 4:
            test_func = static_cast<double (*)(const CH::vector<double>&)>(Rosenbrock);
            break;

        case 5:
            test_func = static_cast<double (*)(const CH::vector<double>&)>(Zakharov);
            break;

        case 6:
            test_func = static_cast<double (*)(const CH::vector<double>&)>(Michalewicz);
            break;
    }

    if (N == -1)
        N = D * 10000 / k;
    std::cerr << std::format("N = {}, D = {}, k = {}", N, D, k) << std::endl;

    //constexpr auto EVAL_N = 30;
    auto eval_num = EVAL_N;
    double mean = 0;
    double std = 0;
    std::chrono::duration<double> time_mean = 0ms;
    while (eval_num--)
    {
        double xl = 0, xu = 30;
        set_search_bound(&xu, &xl, fn);
        auto st = std::chrono::steady_clock::now();
        auto tmp_res = AS(N, D, k, r, rt, ER, xl, xu, fn);
        auto ed = std::chrono::steady_clock::now();
        time_mean += ed - st;

        mean += tmp_res;
        std += tmp_res * tmp_res;
    }

    mean /= EVAL_N;
    time_mean /= EVAL_N;
    std /= EVAL_N;
    std -= mean * mean;
    std = sqrt(std);
    
    //std::cout << std::format("mean: {:.4f}±{:.4f}", mean, std) << std::endl;

    std::cerr << std::format("mean: {}±{}, avg time: {}", mean, std, time_mean) << std::endl;
    return 0;
}
