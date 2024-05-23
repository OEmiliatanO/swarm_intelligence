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

double DE(size_t N, size_t D, size_t k, double xl, double xu, [[maybe_unused]] int fn = 1)
{
#ifdef SAVE
    std::fstream fs;
    fs.open(std::format("plot/{}/{}_path.txt", table[fn], table[fn]), std::ios::out);
    fs << std::format("{} {} {}", N, D, k) << std::endl;
#endif

    std::vector<CH::vector<double>> X;
    std::vector<CH::vector<double>> newX;
    X.resize(N);
    newX.resize(N);
    std::vector<double> fp;
    fp.resize(N);
    CH::vector<double> gbest;
    double fgbest;

    /*  init  */
    std::random_device rnd_device;
    std::mt19937 eng { rnd_device() };
    std::uniform_real_distribution<double> Xdist {xl, xu};
    std::uniform_int_distribution<int> dist13{1, 3};
    std::uniform_real_distribution<double> dist01 {0, 1};

    fgbest = std::numeric_limits<double>::max();
    for (size_t i = 0; i < N; ++i)
    {
        X[i].resize(D);
        std::generate(X[i].begin(), X[i].end(), [&] { return Xdist(eng); });

#ifdef SAVE
        fs << X[i] << std::endl;
#endif

        fp[i] = test_func(X[i]);
        if (fgbest > fp[i])
        {
            gbest = X[i];
            fgbest = fp[i];
        }
    }
    /*  init  */

    /*  iteration */
    for (size_t iter = 0; iter < k; ++iter)
    {
        CH::vector<double> v[N];
        for (size_t i = 0; i < N; ++i)
        {
            auto r1 = static_cast<size_t>(dist01(eng) * (N - 1)), 
                 r2 = static_cast<size_t>(dist01(eng) * (N - 1)), 
                 r3 = static_cast<size_t>(dist01(eng) * (N - 1)), 
                 r4 = static_cast<size_t>(dist01(eng) * (N - 1)), 
                 r5 = static_cast<size_t>(dist01(eng) * (N - 1));

            double m = 0.0, c = 0.0;
            auto q = dist13(eng);
            if (q == 1)      m = 1, c = 0.1;
            else if (q == 2) m = 1, c = 0.9;
            else             m = 0.8, c = 0.2;

            CH::vector<double> u1;
            u1.resize(D);
            auto randj = static_cast<size_t>(dist01(eng) * (N - 1));
            for (size_t j = 0; j < D; ++j)
                u1[j] = dist01(eng) < c or j == randj ? X[r1][j] + m * (X[r2][j] - X[r3][j]) : X[i][j];

            q = dist13(eng);
            if (q == 1)      m = 1, c = 0.1;
            else if (q == 2) m = 1, c = 0.9;
            else             m = 0.8, c = 0.2;

            CH::vector<double> u2;
            u2.resize(D);
            randj = static_cast<size_t>(dist01(eng) * (N - 1));
            for (size_t j = 0; j < D; ++j)
                u2[j] = dist01(eng) < c or j == randj ? X[r1][j] + m * (X[r2][j] - X[r3][j]) + m * (X[r4][j] - X[r5][j]) : X[i][j];

            q = dist13(eng);
            if (q == 1)      m = 1, c = 0.1;
            else if (q == 2) m = 1, c = 0.9;
            else             m = 0.8, c = 0.2;

            CH::vector<double> u3 = X[i] + dist01(eng) * (X[r1] - X[i])  + m * (X[r2] - X[r3]);

            double f1 = test_func(u1);
            double f2 = test_func(u2);
            double f3 = test_func(u3);
            
            double minf = std::min({f1, f2, f3});
            if (minf == f1)      { newX[i] = fp[i] > f1 ? u1 : X[i]; fp[i] = std::min(f1, fp[i]); }
            else if (minf == f2) { newX[i] = fp[i] > f2 ? u2 : X[i]; fp[i] = std::min(f2, fp[i]); }
            else if (minf == f3) { newX[i] = fp[i] > f3 ? u3 : X[i]; fp[i] = std::min(f3, fp[i]); }
            
            if (fgbest > fp[i])
            {
                gbest = newX[i];
                fgbest = fp[i];
            }
        }

        newX.swap(X);

#ifdef SAVE
        for (size_t i = 0; i < N; ++i)
            fs << X[i] << std::endl;
#endif

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
// --mutation_amp x
// --crossover xx
// --test_function fn
// (k+1)*N <= D*10^4
// ./main.elf --N 10 --D 10 --k 1 --test_function Ackley
int main([[maybe_unused]]int argc, char **argv)
{
    using namespace std::chrono_literals;

    int N, D, k, fn;

    sscanf(argv[2], "%d", &N);
    sscanf(argv[4], "%d", &D);
    sscanf(argv[6], "%d", &k);
    sscanf(argv[8], "%d", &fn);

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

    N = std::min(N, D * 10000 / (3 * k+1));
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
        auto tmp_res = DE(N, D, k, xl, xu, fn);
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
