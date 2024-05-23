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

struct indv_t
{
    CH::vector<double> x;
    double f;

    indv_t() = default;
    indv_t(const CH::vector<double>& _x, const double& _f): x(_x), f(_f);
};

double DE(size_t N, size_t D, size_t k, double p, double c, double xl, double xu, [[maybe_unused]] int fn = 1)
{
#ifdef SAVE
    std::fstream fs;
    fs.open(std::format("plot/{}/{}_path.txt", table[fn], table[fn]), std::ios::out);
    fs << std::format("{} {} {}", N, D, k) << std::endl;
#endif

    std::vector<indv_t> P, nexP;
    P.resize(N), nexP.resize(N);
    std::vector<indv_t> A;
    std::vector<double> SCR, SF;

    CH::vector<double> gbest;
    double fgbest;

    /*  init  */
    std::random_device rnd_device;
    std::mt19937 eng { rnd_device() };
    std::uniform_real_distribution<double> Xdist {xl, xu};
    std::uniform_real_distribution<double> dist01 {0, 1};

    double mu_cr = 0.5, mu_f = 0.5;
    double mean_scr = 0.0, mean_sf = 0.0;

    fgbest = std::numeric_limits<double>::max();
    for (size_t i = 0; i < N; ++i)
    {
        P[i].x.resize(D);
        std::generate(P[i].x.begin(), P[i].x.end(), [&] { return Xdist(eng); });

#ifdef SAVE
        fs << P[i].x << std::endl;
#endif

        P[i].f = test_func(P[i].x);
        if (fgbest > P[i].f)
        {
            gbest = P[i].x;
            fgbest = P[i].f;
        }
    }
    /*  init  */

    /*  iteration */
    for (size_t iter = 0; iter < k; ++iter)
    {
        std::vector<indv_t> v;
        v.resize(N);

        std::normal_distribution<double> dist_n {mu_cr, 0.1};
        std::cauchy_distribution<double> dist_c {mu_f,  0.1};

        std::sort(P.begin(), P.end(), [] (const auto& lhs, const auto& rhs) { return lhs.f < rhs.f; });

        for (size_t i = 0; i < N; ++i)
        {
            double CR = dist_n(eng), F = dist_c(eng);
            auto r1 = static_cast<size_t>(dist01(eng) * (N - 1)), 
                 r2 = static_cast<size_t>(dist01(eng) * (N + A.size() - 1));

            while (r1 == i) r1 = static_cast<size_t>(dist01(eng) * (N - 1));
            while (r2 == i or r2 == r1) r2 = static_cast<size_t>(dist01(eng) * (N + A.size() - 1));

            const auto& X_pbest = P[static_cast<size_t>(dist01(eng) * (std::floor(N * p) - 1))].x;
            const auto& X_ = (r2 >= N ? A[r2-N].x : P[r2].x);
            
            v[i].x = P[i].x + F * (X_pbest - P[i].x) + F * (P[r1].x - X_);

            const auto jrand = static_cast<size_t>(dist01(eng) * (D - 1));
            for (size_t j = 0; j < D; ++j)
                v[i].x[j] = j == jrand or dist01(eng) < CR ? v[i].x[j] : P[i].x[j];

            v[i].f = test_func(v[i].x);

            if (P[i].f > v[i].f)
            {
                A.emplace_back(P[i].x, P[i].f);
                SCR.emplace_back(CR);
                SF.emplace_back(F);
                nexP[i].x = v[i].x, nexP[i].f = v[i].f;
            }
            else
                nexP[i].x = P[i].x, nexP[i].f = P[i].f;
        }

        P.swap(nexP);

        while (A.size() > N)
            A.erase(A.begin() + dist01(eng) * (A.size() - 1));

        auto meanA = [](const std::vector<double>& S) {
            double res = 0;
            for (const auto& a : S) res += a;
            return res / S.size();
        };

        auto meanL = [](const std::vector<double>& S) {
            double p = 0, q = 0;
            for (const auto& a : S) p += a * a, q += a;
            return p / q;
        };

        mu_cr = (1 - c) * mu_cr + c * meanA(SCR);
        mu_f  = (1 - c) * mu_f  + c * meanL(SF);

        for (size_t i = 0; i < N; ++i)
        {
#ifdef SAVE
            fs << P[i].x << std::endl;
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
// ./main.elf --N 10 --D 10 --k 1 --p 0.05 --c 0.1 --test_function Ackley
int main([[maybe_unused]]int argc, char **argv)
{
    using namespace std::chrono_literals;

    int N, D, k, fn;
    double p, c;

    sscanf(argv[2], "%d", &N);
    sscanf(argv[4], "%d", &D);
    sscanf(argv[6], "%d", &k);
    sscanf(argv[8], "%lf", &p);
    sscanf(argv[10], "%lf", &c);
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
        auto tmp_res = DE(N, D, k, p, c, xl, xu, fn);
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
