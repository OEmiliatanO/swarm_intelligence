#include <iostream>
#include <format>
#include <fstream>
#include <cmath>
#include <chrono>
#include <random>

#include <vector.h>
#include <test_function.h>

double (*test_func)(const CH::vector<double>& X);

std::string table[] = {"0", "Ackley", "Rastrigin", "HappyCat", "Rosenbrock", "Zakharov", "Michalewicz"};

CH::vector<double> neighbor(CH::vector<double>& X, size_t D, std::mt19937& eng, std::uniform_real_distribution<double> distpm1, double xl, double xu, double step)
{
    CH::vector<double> disturbance;
    disturbance.resize(D);
    std::generate(disturbance.begin(), disturbance.end(), [&] { return sqrt(step) * distpm1(eng); } );
    CH::vector<double> res = X + disturbance;
    std::transform(res.begin(), res.end(), res.begin(), [&] (double x) { return std::max(xl, std::min(xu, x)); });
    return res;
}

double SA(size_t D, size_t k, double initT, double rt, double step, double xl, double xu, [[maybe_unused]] int fn = 1)
{
#ifdef SAVE
    std::fstream fs;
    fs.open(std::format("plot/{}/{}_path.txt", table[fn], table[fn]), std::ios::out);
    fs << std::format("{} {}", D, k) << std::endl;
#endif

    CH::vector<double> S;
    double E;
    CH::vector<double> gbest;
    double fgbest;
    double T = initT;

    /*  init  */
    std::random_device rnd_device;
    std::mt19937 eng { rnd_device() };
    std::uniform_real_distribution<double> Xdist {xl, xu};
    std::uniform_real_distribution<double> dist01 {0, 1};
    std::uniform_real_distribution<double> distpm1 {-1, 1};

    S.resize(D);
    std::generate(S.begin(), S.end(), [&] { return Xdist(eng); });
    gbest = S;
    fgbest = E = test_func(S);
    /*  init  */
#ifdef SAVE
    fs << S << '\n' << gbest << std::endl;
#endif

    /*  iteration */
    for (size_t i = 0; i < k - 1; ++i)
    {
        T *= rt;
        auto Snex = neighbor(S, D, eng, distpm1, xl, xu, step);
        step *= rt;
        auto Enex = test_func(Snex);
        auto dT = Enex - E;

        if (dT < 0 or dist01(eng) < exp(-dT/T))
        {
            S = Snex;
            E = Enex;
        }

        if (Enex < fgbest)
        {
            gbest = Snex;
            fgbest = Enex;
        }
#ifdef SAVE
        fs << S << '\n' << gbest << std::endl;
#endif
    }
    /*  iteration */

#ifdef SAVE
    fs.close();
#endif
    std::cout << fgbest << std::endl;
    return fgbest;
}

// --D dimension
// --k iteration_num
// --initT initial temperature
// --rt decrease rate
// --step length of step
// --test_function fn
// (k+1)*N <= D*10^4
// ./main.elf --D 10 --k 10 --initT 10 --rt 0.5 --step 1 --test_function Ackley
int main([[maybe_unused]]int argc, char **argv)
{
    using namespace std::chrono_literals;

    int D, k, fn;
    double initT, rt, step;

    sscanf(argv[2], "%d", &D);
    sscanf(argv[4], "%d", &k);
    sscanf(argv[6], "%lf", &initT);
    sscanf(argv[8], "%lf", &rt);
    sscanf(argv[10], "%lf", &step);
    sscanf(argv[12], "%d", &fn);

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

    k = std::max(D * 10000, k);
    std::cerr << std::format("D = {}, k = {}", D, k) << std::endl;

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
        auto tmp_res = SA(D, k, initT, rt, step, xl, xu, fn);
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
