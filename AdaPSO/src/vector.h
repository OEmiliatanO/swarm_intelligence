#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <vector>
#include <iostream>
#include <initializer_list>
#include <cmath>
#include <functional>
#include <limits>
#include <utility>

namespace CH
{
    template<typename T>
    class point_t;

    // add
    template<typename T> point_t<T> operator+(const point_t<T>& a, const point_t<T>& b);
    // dot
    template<typename T> T operator*(const point_t<T>& a, const point_t<T>& b);
    // scale
    template<typename T> point_t<T> Hadamard_prod(const point_t<T>& a, const point_t<T>& b);
    template<typename T> point_t<T> operator*(const point_t<T>& a, T scale);
    template<typename T> point_t<T> operator*(T scale, const point_t<T>& a);
    template<typename T> point_t<T> operator/(const point_t<T>& a, T scale);
    // minus
    template<typename T> point_t<T> operator-(const point_t<T>& a, const point_t<T>& b);
    // point_t cout
    template<typename T> std::ostream& operator<<(std::ostream& os, const point_t<T>& a);

    // distance
    template<typename T> double euclidean_dist(const point_t<T>& a, const point_t<T>& b);
    template<typename T> double minkowski_dist(const point_t<T>& a, const point_t<T>& b, int p);

    template<typename T>
    class point_t
    {
    public:
        std::vector<T> x;

        point_t() = default;
        
        point_t(const point_t<T>& other): x{other.x} { }
        point_t(point_t<T>&& other): x{std::move(other.x)} { }
        
        point_t(const std::vector<T>& vx): x{vx} { }
        point_t(std::vector<T>&& vx): x{std::move(vx)} { }
        
        point_t(std::initializer_list<T>& list): x{list} { }
        point_t(std::initializer_list<T>&& list): x{list} { }

        point_t(std::size_t n, std::function<T(void)> rd) { x.resize(n); for (auto& it : x) it = rd(); }

        // size
        std::size_t size() const noexcept { return this->x.size(); }
        bool empty() const noexcept { return this->x.empty(); }
        void resize(std::size_t n) { this->x.resize(n); }

        // clear
        void clear() { this->x.clear(); }
        void fill0() { std::fill(this->x.begin(), this->x.end(), 0); }

        // cout
        friend std::ostream& operator<< <>(std::ostream& os, const point_t<T>& a);

        // extend && emplace_back
        void extend(const point_t<T>& other) { this->x.insert(this->x.end(), other.x.begin(), other.x.end()); }
        void extend(point_t<T>&& other) { this->x.insert(this->x.end(), other.x.begin(), other.x.end()); }
        void extend(const std::vector<T>& vx) { this->x.insert(this->x.end(), vx.begin(), vx.end()); }
        void extend(std::vector<T>&& vx) { this->x.insert(this->x.end(), vx.begin(), vx.end()); }
        void emplace_back(const T& x) { this->x.emplace_back(x); }
        void emplace_back(T&& x) { this->x.emplace_back(std::forward<decltype(x)>(x)); }

        /**** index-access ****/
        const T& operator[](std::size_t i) const { return this->x.at(i); }
              T& operator[](std::size_t i)       { return this->x.at(i); }
        const T& at(std::size_t i) const { return this->x.at(i); }
              T& at(std::size_t i)       { return this->x.at(i); }
        /**** index-access ****/
        
        /**** iterator-related ****/
        auto begin() noexcept -> decltype(x.begin()) { return x.begin(); }
        auto end()   noexcept -> decltype(x.end())   { return x.end(); }
        auto begin() const noexcept -> decltype(x.begin()) { return x.begin(); }
        auto end()   const noexcept -> decltype(x.end())   { return x.end(); }
        /**** iterator-related ****/
        
        /**** assignment ****/
        point_t<T>& operator=(const point_t<T>& other);
        point_t<T>& operator=(point_t<T>&& other);
        /**** assignment ****/

        /**** operator ****/
        point_t<T>& operator+=(const point_t<T>& b);
        friend point_t<T>  operator+ <> (const point_t<T>& a, const point_t<T>& b);
        // dot
        friend T operator* <> (const point_t<T>& a, const point_t<T>& b);
        // scale
        friend point_t<T> operator* <> (const point_t<T>& a, T scale);
        friend point_t<T> operator* <> (T scale, const point_t<T>& a);
        friend point_t<T> operator/ <> (const point_t<T>& a, T scale);
        point_t<T>& operator*=(T scale);
        point_t<T>& operator/=(T scale);
        // minus
        friend point_t<T> operator- <> (const point_t<T>& a, const point_t<T>& b);
        /**** operator ****/
    };

    template<typename T>
    using vector = point_t<T>;

    /***** point_t ******/
    /**** assignment ****/
    template<typename T>
    point_t<T>& point_t<T>::operator=(const point_t<T>& other)
    {
        this->x = other.x;
        return *this;
    }

    template<typename T>
    point_t<T>& point_t<T>::operator=(point_t<T>&& other)
    {
        this->x = std::move(other.x);
        return *this;
    }
    /**** assignment ****/

    /**** operator ****/
    template<typename T>
    point_t<T>& point_t<T>::operator+=(const point_t<T>& other)
    {
        if (this->size() != other.size())
        {
            std::cerr << "ERROR: a+=b: a.size(" << this->size() << ") != b.size(" << other.size() << "), exit." << std::endl;
            exit(1);
        }
        for (size_t i = 0; i < this->size(); ++i) { (*this)[i] += other[i]; }
        return *this;
    }
    template<typename T>
    point_t<T> operator+(const point_t<T>& a, const point_t<T>& b)
    {
        point_t<T> ret;
        
        const auto& [ma, mi] = std::pair{(a.size() > b.size() ? a : b), (a.size() <= b.size() ? a : b)};
        for (size_t i = 0; i < mi.size(); ++i) 
            ret.emplace_back(ma[i] + mi[i]);
        for (size_t i = mi.size(); i < ma.size(); ++i) 
            ret.emplace_back(ma[i]);

        return ret;
    }
    // dot
    template<typename T>
    T operator*(const point_t<T>& a, const point_t<T>& b)
    {
        T ret = 0;
        for (size_t i = 0; i < std::min(a.size(), b.size()); ++i) ret += a[i] * b[i];
        return ret;
    }
    // scale
    template<typename T> 
    point_t<T> Hadamard_prod(const point_t<T>& a, const point_t<T>& b)
    {
        point_t<T> ret;
        for (size_t i = 0; i < std::min(a.size(), b.size()); ++i) ret.emplace_back(a[i] * b[i]);
        return ret;
    }
    template<typename T>
    point_t<T> operator*(const point_t<T>& a, T scale)
    {
        point_t<T> ret{a};
        for (auto& x : ret) x *= scale;
        return ret;
    }
    template<typename T>
    point_t<T> operator*(T scale, const point_t<T>& a)
    {
        return a * scale;
    }
    template<typename T>
    point_t<T> operator/(const point_t<T>& a, T scale)
    {
        point_t<T> ret{a};
        for (auto& x : ret) x /= scale;
        return ret;
    }
    template<typename T> 
    point_t<T>& point_t<T>::operator/=(T scale)
    {
        for (auto& y : this->x) y /= scale;
        return *this;
    }

    template<typename T>
    point_t<T>& point_t<T>::operator*=(T scale)
    {
        for (auto& y : this->x) y *= scale;
        return *this;
    }

    // minus
    template<typename T>
    point_t<T> operator-(const point_t<T>& a, const point_t<T>& b)
    {
        return a+(T)(-1)*b;
    }
    /**** operator ****/

    // point_t cout
    template<typename T>
    std::ostream& operator<<(std::ostream& os, const point_t<T>& a)
    {
        for (auto& x : a.x) os << x << ' ';
        return os;
    }

    // distance
    template<typename T>
    double minkowski_dist(const point_t<T>& a, const point_t<T>& b, int p)
    {
        if (a.size() != b.size()) { std::cerr << "minkowski_dist(a,b): a.size() != b.size(), exit..." << std::endl; exit(1); }
        double ret = 0;
        for (std::size_t i = 0; i < a.size(); ++i) ret += pow(a[i]-b[i], p);
        return pow(ret, 1.0/p);
    }
    template<typename T>
    double euclidean_dist(const point_t<T>& a, const point_t<T>& b)
    {
        // naive
        //const point_t c = a-b;
        //return sqrt(c*c);

        // minkowski
        return minkowski_dist(a, b, 2);
    }
}

#endif
