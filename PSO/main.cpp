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

double PSO(size_t N, size_t D, size_t k, double vmax, double c1, double c2, double xl, double xu, double vl = 0.001, double vu = 1,[[maybe_unused]] int fn = 1)
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

    CH::vector<double> X[N], V[N];
    CH::vector<double> pbest[N];
    double fpbest[N];
    CH::vector<double> gbest;
    double fgbest;

    /*  init  */
    std::random_device rnd_device;
    std::mt19937 eng { rnd_device() };
    std::uniform_real_distribution<double> Xdist {xl, xu};
    std::uniform_real_distribution<double> Vdist {vl, vu};
    std::uniform_real_distribution<double> dist01 {0, 1};

    fgbest = std::numeric_limits<double>::max();
    for (size_t i = 0; i < N; ++i)
    {
        X[i].resize(D);
        V[i].resize(D);
        std::generate(X[i].begin(), X[i].end(), [&] { return Xdist(eng); });
        std::generate(V[i].begin(), V[i].end(), [&] { return Vdist(eng); });

#ifdef SAVE
        fs << X[i] << '\n' << V[i] << std::endl;
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

    /*  iteration */
    for (size_t iter = 0; iter < k; ++iter)
    {
        CH::vector<double> r1, r2;
        r1.resize(D); r2.resize(D);
        std::generate(r1.begin(), r1.end(), [&] { return dist01(eng); });
        std::generate(r2.begin(), r2.end(), [&] { return dist01(eng); });
        for (size_t i = 0; i < N; ++i)
        {
            V[i] = V[i] + c1 * CH::Hadamard_prod(r1, (pbest[i] - X[i])) + c2 * CH::Hadamard_prod(r2, (gbest - X[i]));
            std::transform(V[i].begin(), V[i].end(), V[i].begin(), [&] (const auto& v) { return std::min(vmax, v); });
            X[i] = X[i] + V[i];
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
            fs << X[i] << '\n' << V[i] << std::endl;
#endif
        }
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
// --vmax vmax
// --c1 c1
// --c2 c1
// --test_function fn
// (k+1)*N <= D*10^4
// ./main.elf --N 10 --D 10 --k 1 --vmax 1 --c1 2 --c2 0.2 --test_function Ackley
int main([[maybe_unused]]int argc, char **argv)
{
    using namespace std::chrono_literals;

    int N, D, k, fn;
    double vmax, c1, c2;

    sscanf(argv[2], "%d", &N);
    sscanf(argv[4], "%d", &D);
    sscanf(argv[6], "%d", &k);
    sscanf(argv[8], "%lf", &vmax);
    sscanf(argv[10], "%lf", &c1);
    sscanf(argv[12], "%lf", &c2);
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

    N = std::min(N, D * 10000 / (k+1));
    std::cerr << std::format("N = {}, D = {}, k = {}", N, D, k) << std::endl;

    //constexpr auto EVAL_N = 30;
    auto eval_num = EVAL_N;
    double mean = 0;
    double std = 0;
    std::chrono::duration<double> time_mean = 0ms;
    while (eval_num--)
    {
        double xl = 0, xu = 30, vl = 0.1, vu=1;
        set_search_bound(&xu, &xl, fn);
        auto st = std::chrono::steady_clock::now();
        auto tmp_res = PSO(N, D, k, vmax, c1, c2, xl, xu, vl, vu, fn);
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
