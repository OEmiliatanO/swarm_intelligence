#include <iostream>
#include <format>
#include <fstream>
#include <cmath>
#include <chrono>
#include <random>
#include <deque>
#include <queue>

#include <vector.h>
#include <test_function.h>

double (*test_func)(const CH::vector<double>& X);

std::string table[] = {"0", "Ackley", "Rastrigin", "HappyCat", "Rosenbrock", "Zakharov", "Michalewicz"};

struct area_t
{
    std::vector<double> st;
    std::vector<double> ed;
    double f;
};

struct indv_t
{
    CH::vector<double> p, step;
    double f;
};

bool operator<(const area_t& lhs, const area_t& rhs)
{
    return lhs.f > rhs.f;
}
bool operator<(const indv_t& lhs, const indv_t& rhs)
{
    return lhs.f < rhs.f;
}

std::vector<indv_t> split(const CH::vector<double>& st, const CH::vector<double>& ed, size_t n, size_t m)
{
    std::random_device rnd_device;
    std::mt19937 eng { rnd_device() };
    std::uniform_int_distribution<size_t> dist {0, n-1};

    std::vector<indv_t> vec;
    vec.resize(n);
    auto d = st.size();
    if (d != ed.size())
    {
        std::cerr << "st.size() != ed.size()" << std::endl;
        exit(0);
    }

    for (size_t i = 0; i < n; ++i)
    {
        vec[i].p.resize(d);
        vec[i].step.resize(d);
    }

    for (size_t i = 0; i < d; ++i)
    {
        double step = (ed[i] - st[i]) / m;
        for (size_t j = 0; j < n; ++j)
        {
            auto k = dist(eng);
            vec[j].step[i] = (ed[i] - st[i]) * 1.5 / m;
            vec[j].p[i] = ((st[i] + k * step) + (st[i] + (k+1) * step)) / 2;
        }
    }

    return vec;
}

area_t neighbor(const indv_t& center)
{
    area_t area;
    area.st.resize(center.p.size());
    area.ed.resize(center.p.size());
    for (size_t i = 0; i < center.p.size(); ++i)
    {
        area.st[i] = center.p[i] - center.step[i];
        area.ed[i] = center.p[i] + center.step[i];
    }
    area.f = center.f;
    return area;
}

double SA(size_t N, size_t D, size_t max_eval, [[maybe_unused]]double m, [[maybe_unused]]double c, double xl, double xu, [[maybe_unused]] int fn = 1)
{
#ifdef SAVE
    std::fstream fs;
    fs.open(std::format("plot/{}/{}_path.txt", table[fn], table[fn]), std::ios::out);
    //fs << std::format("{} {} {}", N, D, max_eval) << std::endl;
#endif

    std::priority_queue<area_t> qu;
    CH::vector<double> gbest;
    double fgbest;

    /*  init  */
    {
        area_t tmp_area{};
        
        for (size_t i = 0; i < D; ++i)
        {
            tmp_area.st.emplace_back(xl);
            tmp_area.ed.emplace_back(xu);
        }
        qu.emplace(tmp_area);
    }
    std::random_device rnd_device;
    std::mt19937 eng { rnd_device() };
    std::uniform_real_distribution<double> Xdist {xl, xu};
    std::uniform_real_distribution<double> dist01 {0, 1};

    fgbest = std::numeric_limits<double>::max();
    /*  init  */

    /*  iteration */
    size_t it = 0, cnt = 0;
    while (qu.size() and cnt < max_eval)
    {
        auto new_p = split(qu.top().st, qu.top().ed, N, m); qu.pop();

        for (size_t i = 0; i < new_p.size(); ++i)
        {
            new_p[i].f = test_func(new_p[i].p);
            ++cnt;
            if (cnt >= max_eval)
                new_p[i].f = std::numeric_limits<double>::max();
        }
        
        std::sort(new_p.begin(), new_p.end());
        
        if (fgbest > new_p[0].f)
        {
            gbest = new_p[0].p;
            fgbest = new_p[0].f;
#ifdef SAVE
            fs << new_p.size() << std::endl;
            for (size_t i = 0; i < new_p.size(); ++i)
                fs << new_p[i].p << '\n' << new_p[i].step << std::endl;
            fs << fgbest << std::endl;
#endif
        }

        for (size_t i = 0; i < std::min(new_p.size(), N); ++i)
            qu.emplace(neighbor(new_p[i]));

        //std::cerr << std::format("iteration #{}: global best fitness = {}", it, fgbest) << std::endl;

        ++it;
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
// ./main.elf --N 10 --D 10 --k 1 --m 1 --c 0.5 --test_function Ackley
int main([[maybe_unused]]int argc, char **argv)
{
    using namespace std::chrono_literals;

    int N, D, k, fn, m;
    double c;

    sscanf(argv[2], "%d", &N);
    sscanf(argv[4], "%d", &D);
    sscanf(argv[6], "%d", &k);
    sscanf(argv[8], "%d", &m);
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

    k = D * 10000;
    std::cerr << std::format("N = {}, D = {}, k = {}", N, D, k) << std::endl;

    //constexpr auto EVAL_N = 30;
    auto eval_num = EVAL_N;
    double mean = 0, std = 0;
    std::chrono::duration<double> time_mean = 0ms;
    while (eval_num--)
    {
        double xl = 0, xu = 30;
        set_search_bound(&xu, &xl, fn);
        auto st = std::chrono::steady_clock::now();
        auto tmp_res = SA(N, D, k, m, c, xl, xu, fn);
        auto ed = std::chrono::steady_clock::now();
        time_mean += ed - st;

        mean += tmp_res;
        std += tmp_res * tmp_res;
    }

    mean /= EVAL_N;
    time_mean /= EVAL_N;
    std /= EVAL_N;
    std -= mean * mean;
    if (std::abs(std) < 1e-6) std = 0;
    std = sqrt(std);
    
    //std::cout << std::format("mean: {:.4f}±{:.4f}", mean, std) << std::endl;

    std::cerr << std::format("mean: {}±{}, avg time: {}", mean, std, time_mean) << std::endl;
    return 0;
}
