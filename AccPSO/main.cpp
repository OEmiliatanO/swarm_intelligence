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

double PSO(size_t N, size_t D, size_t k, double xl, double xu, double _alpha, double _beta, double gamma, double _L, [[maybe_unused]] int fn = 1)
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

    CH::vector<double> X[N];
    CH::vector<double> pbest[N];
    double fpbest[N];
    CH::vector<double> gbest;
    double fgbest;

    /*  init  */
    std::random_device rnd_device;
    std::mt19937 eng { rnd_device() };
    std::uniform_real_distribution<double> Xdist {xl, xu};
    std::uniform_real_distribution<double> dist01 {0, 1};

    fgbest = std::numeric_limits<double>::max();
    for (size_t i = 0; i < N; ++i)
    {
        X[i].resize(D);
        std::generate(X[i].begin(), X[i].end(), [&] { return Xdist(eng); });

#ifdef SAVE
        fs << X[i] << std::endl;
#endif

        pbest[i] = X[i];
        fpbest[i] = test_func(X[i]);
        if (fgbest > fpbest[i])
        {
            gbest = X[i];
            fgbest = fpbest[i];
        }
    }
    /*  init  */

    double alpha = _alpha, beta = _beta, L = _L;
    /*  iteration */
    for (size_t iter = 0; iter < k; ++iter)
    {
        for (size_t i = 0; i < N; ++i)
        {
            CH::vector<double> u;
            std::generate(u.begin(), u.end(), [&] { return dist01(eng); });
            X[i] = (1 - beta) * X[i] + beta * gbest + alpha * L * u;
            std::transform(X[i].begin(), X[i].end(), X[i].begin(), [&] (const auto& x) { return std::max(xl, std::min(xu, x)); });
            auto fxi = test_func(X[i]);
            if (fxi < fpbest[i])
            {
                fpbest[i] = fxi;
                pbest[i] = X[i];
            }
            if (fxi < fgbest)
            {
                fgbest = fxi;
                gbest = X[i];
            }
#ifdef SAVE
            fs << X[i] << std::endl;
#endif
        }
        alpha *= gamma;
    }
    /*  iteration */
#ifdef SAVE
    fs.close();
#endif
    std::cout << fgbest << std::endl;
    return fgbest;
}

// --N particle_num
// --D dimension
// --k iteration_num
// --alpha 0.5
// --beta 0.5
// --L 1
// --test_function fn
// (k+1)*N <= D*10^4
// ./main.elf --N 10 --D 10 --k 1 --alpha 0.5 --beta 0.5 --gamma 0.5 --L 1 --test_function 1
int main([[maybe_unused]]int argc, char **argv)
{
    using namespace std::chrono_literals;

    int N, D, k, fn;
    double alpha, beta, gamma, L;

    sscanf(argv[2], "%d", &N);
    sscanf(argv[4], "%d", &D);
    sscanf(argv[6], "%d", &k);
    sscanf(argv[8], "%lf", &alpha);
    sscanf(argv[10], "%lf", &beta);
    sscanf(argv[12], "%lf", &gamma);
    sscanf(argv[14], "%lf", &L);
    sscanf(argv[16], "%d", &fn);

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

    N = std::min(N, D * 10000 / (k+1));
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
        auto tmp_res = PSO(N, D, k, xl, xu, alpha, beta, gamma, L, fn);
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
