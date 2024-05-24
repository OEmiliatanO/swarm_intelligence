#include <iostream>
#include <format>
#include <fstream>
#include <cmath>
#include <chrono>
#include <random>

#include <vector.h>
#include <test_function.h>

double (*test_func)(const CH::vector<double>& X);

std::string table[] = {"0", "Ackley", "Rastrigin", "HappyCat", "Rosenbrock", "Zakharov", "Michalewicz", "Schwefel", "BentCigar", "DropWave", "Step"};

double DE(size_t N, size_t D, size_t k, double xl, double xu, [[maybe_unused]] int fn = 1)
{
#ifdef SAVE
    std::fstream fs;
    fs.open(std::format("plot/{}/{}_path.txt", table[fn], table[fn]), std::ios::out);
    fs << std::format("{} {} {}", N, D, k) << std::endl;
#endif

    CH::vector<double> X[N];
    double fp[N];
    CH::vector<double> gbest;
    double fgbest, p1 = 0.5;

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

        fp[i] = test_func(X[i]);
        if (fgbest > fp[i])
        {
            gbest = X[i];
            fgbest = fp[i];
        }
    }
    /*  init  */

    /*  iteration */
    double mean_cr = 0.5;
    std::vector<double> c;
    std::vector<double> sucessful_c = {0.5};
    c.resize(N);
    int ns1, nf1, ns2, nf2;

    for (size_t iter = 0; iter < k; ++iter)
    {
        CH::vector<double> v[N];
        if (iter % 25 == 0)
        {
            mean_cr = 0;
            for (auto x : sucessful_c)
                mean_cr += x;
            mean_cr /= sucessful_c.size();
            sucessful_c.clear();
        }

        std::normal_distribution<double> dist_of_c{mean_cr, 0.1};
        
        if (iter % 5 == 0)
            std::generate(c.begin(), c.end(), [&] { return dist_of_c(eng); });

        for (size_t i = 0; i < N; ++i)
        {
            double m = (std::normal_distribution<double>{0.5, 0.3})(eng);
            m = std::min(std::max(std::numeric_limits<double>::epsilon(), m), 2.);

            size_t current_strategy = 0;
            auto r1 = static_cast<size_t>(dist01(eng) * (N - 1)), 
                 r2 = static_cast<size_t>(dist01(eng) * (N - 1)), 
                 r3 = static_cast<size_t>(dist01(eng) * (N - 1));

            if (dist01(eng) < p1)
            {
                v[i] = X[r1] + m * (X[r2] - X[r3]);
                current_strategy = 1;
            }
            else
            {
                v[i] = X[i] + m * (gbest - X[i]) + m * (X[r1] - X[r2]);
                current_strategy = 2;
            }

            auto randk = static_cast<size_t>(dist01(eng) * (D - 1));
            for (size_t j = 0; j < D; ++j)
            {
                v[i][j] = (dist01(eng) <= c[i] or j == randk ? v[i][j] : X[i][j]);
                v[i][j] = std::max(std::min(xu, v[i][j]), xl);
            }

            auto fvi = test_func(v[i]);

            if (fvi < fp[i])
            {
                X[i] = v[i];
                fp[i] = fvi;
                
                if (current_strategy == 1)
                    ++ns1;
                else
                    ++ns2;

                sucessful_c.emplace_back(c[i]);
            }
            if (fgbest > fp[i])
            {
                gbest = X[i];
                fgbest = fp[i];

                if (current_strategy == 1)
                    ++nf1;
                else
                    ++nf2;
            }
            
            p1 = ns1 * (ns2 + nf2) / (ns2 * (ns1 + nf1) + ns1 * (ns2 + nf2));
        }
        for (size_t i = 0; i < N; ++i)
        {
#ifdef SAVE
            fs << X[i] << std::endl;
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

        case 7:
            test_func = static_cast<double (*)(const CH::vector<double>&)>(Schwefel);
            break;

        case 8:
            test_func = static_cast<double (*)(const CH::vector<double>&)>(BentCigar);
            break;

        case 9:
            test_func = static_cast<double (*)(const CH::vector<double>&)>(DropWave);
            break;

        case 10:
            test_func = static_cast<double (*)(const CH::vector<double>&)>(Step);
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
