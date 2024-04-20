#include <iostream>
#include <format>
#include <fstream>
#include <cmath>
#include <chrono>
#include <array>
#include <random>

#include <vector.h>
#include <test_function.h>

double (*test_func)(const CH::vector<double>& X);

std::string table[] = {"0", "Ackley", "Rastrigin", "HappyCat", "Rosenbrock", "Zakharov", "Michalewicz"};

std::array<double, 4> evolutionary_factor_inference(double f)
{
    std::array<double, 4> singleton;
    singleton.fill(0);

    if (f >= 0 && f <= 0.4) {
        singleton[0] = 0;
    } else if (f > 0.4 && f <= 0.6) {
        singleton[0] = 5*f - 2;
    } else if (f > 0.6 && f <= 0.7) {
        singleton[0] = 1;
    } else if (f > 0.7 && f <= 0.8) {
        singleton[0] = -10*f + 8;
    } else if (f > 0.8 && f <= 1) {
        singleton[0] = 0;
    }

    // Second singleton
    if (f >= 0 && f <= 0.2) {
        singleton[1] = 0;
    } else if (f > 0.2 && f <= 0.3) {
        singleton[1] = 10*f - 2;
    } else if (f > 0.3 && f <= 0.4) {
        singleton[1] = 1;
    } else if (f > 0.4 && f <= 0.6) {
        singleton[1] = -5*f + 3;
    } else if (f > 0.6 && f <= 1) {
        singleton[1] = 0;
    }

    // Third singleton
    if (f >= 0 && f <= 0.1) {
        singleton[2] = 1;
    } else if (f > 0.1 && f <= 0.3) {
        singleton[2] = -5*f + 1.5;
    } else if (f > 0.3 && f <= 1) {
        singleton[2] = 0;
    }

    // Fourth singleton
    if (f >= 0 && f <= 0.7) {
        singleton[3] = 0;
    } else if (f > 0.7 && f <= 0.9) {
        singleton[3] = 5*f - 3.5;
    } else if (f > 0.9 && f <= 1) {
        singleton[3] = 1;
    }

    return singleton;
}

double adaPSO(const size_t N, const size_t D, const size_t k, const double xl, const double xu, [[maybe_unused]] int fn = 1)
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

    std::random_device rnd_device;
    std::mt19937 eng { rnd_device() };
    std::uniform_real_distribution<double> Xdist {xl, xu};
    std::uniform_real_distribution<double> dist01 {0, 1};
    std::uniform_real_distribution<double> disdelta{0.05, 0.1};

    constexpr size_t rulebase[4][4] = \
    {
        {1, 1, 4, 1},
        {2, 2, 2, 1},
        {2, 3, 3, 3},
        {4, 3, 4, 4}
    };
    const double delta = disdelta(eng);
    const double acc_coe_update[5][2] = \
    {
        {        0,          0}, 
        {    delta,     -delta},
        {0.5*delta, -0.5*delta},
        {0.5*delta,  0.5*delta},
        {   -delta,     delta}
    };
    const double vmax = (xu - xl) * 0.2;
    std::uniform_real_distribution<double> Vdist {-vmax, vmax};
    constexpr double omega_max = 0.9, omega_min = 0.4, cmax = 2.5, cmin = 1.5, csum_max = 4;
    constexpr double sigma_max = 1, sigma_min = 0.1;
    CH::vector<double> X[N], V[N];
    CH::vector<double> pbest[N];
    double fpbest[N];
    CH::vector<double> gbest;
    double fgbest = std::numeric_limits<double>::max();
    size_t gbest_idx = 0;
    size_t now_state = 1; // starts at exploration stage
    double omega = omega_max;
    double sigma = sigma_max;
    double c1 = 1.5 + dist01(eng), c2 = 1.5 + dist01(eng);
    c1 = std::max(std::min(cmax, c1), cmin);
    c2 = std::max(std::min(cmax, c2), cmin);
    if (c1 + c2 >= csum_max)
    {
        auto s = c1 + c2;
        c1 = 4.0 * c1 / s;
        c2 = 4.0 * c2 / s;
    }

    /*  init  */
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
            gbest_idx = i;
            fgbest = fpbest[i];
        }
    }
    /*  init  */

    /*  iteration */
    for (size_t iter = 0; iter < k; ++iter)
    {
        /* ESE */
        /* calculate mean distance */
        double d[N]{};
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                d[i] += CH::euclidean_dist(X[i], X[j]);
        std::transform(d, d + N, d, [&] (const auto& x) { return x / (N-1); } );

        /* evolutionary factor */
        auto d_max = *std::max_element(d, d + N), d_min = *std::min_element(d, d + N);
        auto f = (d[gbest_idx] - d_min) / (d_max - d_min);
        auto singleton = evolutionary_factor_inference(f);
        auto s = std::max_element(singleton.begin(), singleton.end()) - singleton.begin();
        now_state = rulebase[s][now_state-1];

        /* inertia weight */
        omega = 1.0 / (1 + 1.5 * exp(-2.6*f));
        omega = std::max(std::min(omega_max, omega), omega_min);
        
        /* accerlation coefficients */
        c1 += acc_coe_update[now_state][0];
        c2 += acc_coe_update[now_state][1];
        c1 = std::max(std::min(cmax, c1), cmin);
        c2 = std::max(std::min(cmax, c2), cmin);
        if (c1 + c2 >= csum_max)
        {
            auto s = c1 + c2;
            c1 = 4.0 * c1 / s;
            c2 = 4.0 * c2 / s;
        }
        /* ESE */

        /* ELS */
        if (now_state == 3)
        {
            std::normal_distribution normal_dist{0.0, sigma};
            auto d = static_cast<size_t>(dist01(eng) * D);
            CH::vector<double> P = gbest;
            P[d] = P[d] + (xu - xl) * normal_dist(eng);
            sigma = sigma_max - (sigma_max - sigma_min) * iter / k;
            P[d] = std::max(std::min(xu, P[d]), xl);
            auto v = test_func(P);
            if (v < fgbest)
            {
                gbest = X[gbest_idx] = P;
                fpbest[gbest_idx] = fgbest = v;
            }
            else if (auto worst = std::max_element(fpbest, fpbest + N) - fpbest; v < fpbest[worst])
            {
                X[worst] = P;
                fpbest[worst] = v;
            }
        }
        /* ELS */

        /* standard PSO*/
        CH::vector<double> r1, r2;
        r1.resize(D); r2.resize(D);
        std::generate(r1.begin(), r1.end(), [&] { return dist01(eng); });
        std::generate(r2.begin(), r2.end(), [&] { return dist01(eng); });
        for (size_t i = 0; i < N; ++i)
        {
            V[i] = omega * V[i] + c1 * CH::Hadamard_prod(r1, (pbest[i] - X[i])) + c2 * CH::Hadamard_prod(r2, (gbest - X[i]));
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
                gbest_idx = i;
            }
#ifdef SAVE
            fs << X[i] << '\n' << V[i] << std::endl;
#endif
        }
        /* standard PSO*/
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
// --test_function fn
// (k+1)*N <= D*10^4
// ./main.elf --N 10 --D 10 --k 1 --test_function 1
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
        auto tmp_res = adaPSO(N, D, k, xl, xu, fn);
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
