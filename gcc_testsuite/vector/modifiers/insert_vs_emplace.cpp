// { dg-do run { target c++11 } }

// The class X and test code is by by Howard Hinnant and used under a
// Creative Commons Attribution 4.0 International License.
// http://creativecommons.org/licenses/by/4.0/
// https://github.com/HowardHinnant/papers/blob/master/insert_vs_emplace.html
//
// The original code was reformatted and modified to use the VERIFY macro
// instead of writing to standard output.

#include "uxs/vector.h"

#include "gcc_testsuite/util/testsuite_hooks.h"

namespace {

class X {
    int i_;
    int* p_;

 public:
    struct special {
        unsigned c;
        unsigned dt;
        unsigned cc;
        unsigned ca;
        unsigned mc;
        unsigned ma;
    };
    static special sp;

    X(int i, int* p) : i_(i), p_(p) {
        //         std::cout << "X(int i, int* p)\n";
        sp.c++;
    }

    ~X() {
        //         std::cout << "~X()\n";
        sp.dt++;
    }

    X(const X& x) : i_(x.i_), p_(x.p_) {
        //         std::cout << "X(const X& x)\n";
        sp.cc++;
    }

    X& operator=(const X& x) {
        i_ = x.i_;
        p_ = x.p_;
        //         std::cout << "X& operator=(const X& x)\n";
        sp.ca++;
        return *this;
    }

    X(X&& x) noexcept : i_(x.i_), p_(x.p_) {
        //         std::cout << "X(X&& x)\n";
        sp.mc++;
    }

    X& operator=(X&& x) noexcept {
        i_ = x.i_;
        p_ = x.p_;
        //         std::cout << "X& operator=(X&& x)\n";
        sp.ma++;
        return *this;
    }
};

std::ostream& operator<<(std::ostream& os, X::special const& sp) {
    os << sp.c << '\n';
    os << sp.dt << '\n';
    os << sp.cc << '\n';
    os << sp.ca << '\n';
    os << sp.mc << '\n';
    os << sp.ma << '\n';
    return os;
}

X::special X::sp{};

bool operator==(const X::special& lhs, const X::special& rhs) {
    return lhs.c == rhs.c && lhs.dt == rhs.dt && lhs.cc == rhs.cc && lhs.ca == rhs.ca && lhs.mc == rhs.mc &&
           lhs.ma == rhs.ma;
}

// Verify that insert and emplace are equally efficient.
// Also verify exact number of operations (which are specific to this
// implementation) in order to catch any regressions.

// insert vs emplace lvalue no reallocation
int test01() {
    const X::special expected{0, 1, 1, 0, 1, 3};
    X::special ins, emp;
    {
        uxs::vector<X> v;
        v.reserve(4);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        X x{0, 0};
        // std::cout << "--insert lvalue no reallocation--\n";
        X::sp = {};
        v.insert(v.begin(), x);
        // std::cout << X::sp;
        // std::cout << "----\n";
        ins = X::sp;
    }
    {
        uxs::vector<X> v;
        v.reserve(4);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        X x{0, 0};
        // std::cout << "--emplace lvalue no reallocation--\n";
        X::sp = {};
        v.emplace(v.begin(), x);
        // std::cout << X::sp;
        // std::cout << "----\n";
        emp = X::sp;
    }
    VERIFY(ins == emp);
    VERIFY(ins == expected);
    return 0;
}

// insert vs emplace xvalue no reallocation
int test02() {
    const X::special expected{0, 0, 0, 0, 1, 3};
    X::special ins, emp;
    {
        uxs::vector<X> v;
        v.reserve(4);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        X x{0, 0};
        // std::cout << "--insert xvalue no reallocation--\n";
        X::sp = {};
        v.insert(v.begin(), std::move(x));
        // std::cout << X::sp;
        // std::cout << "----\n";
        ins = X::sp;
    }
    {
        uxs::vector<X> v;
        v.reserve(4);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        X x{0, 0};
        // std::cout << "--emplace xvalue no reallocation--\n";
        X::sp = {};
        v.emplace(v.begin(), std::move(x));
        // std::cout << X::sp;
        // std::cout << "----\n";
        emp = X::sp;
    }
    VERIFY(ins == emp);
    VERIFY(ins == expected);
    return 0;
}

// insert vs emplace rvalue no reallocation
int test03() {
    const X::special expected{1, 1, 0, 0, 1, 3};
    X::special ins, emp;
    {
        uxs::vector<X> v;
        v.reserve(4);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        // std::cout << "--insert rvalue no reallocation--\n";
        X::sp = {};
        v.insert(v.begin(), X{0, 0});
        // std::cout << X::sp;
        // std::cout << "----\n";
        ins = X::sp;
    }
    {
        uxs::vector<X> v;
        v.reserve(4);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        // std::cout << "--emplace rvalue no reallocation--\n";
        X::sp = {};
        v.emplace(v.begin(), X{0, 0});
        // std::cout << X::sp;
        // std::cout << "----\n";
        emp = X::sp;
    }
    VERIFY(ins == emp);
    VERIFY(ins == expected);
    return 0;
}

// insert vs emplace lvalue reallocation
int test04() {
    const X::special expected{0, 3, 1, 0, 3, 0};
    X::special ins, emp;
    {
        uxs::vector<X> v;
        v.reserve(3);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        X x{0, 0};
        // std::cout << "--insert lvalue reallocation--\n";
        X::sp = {};
        v.insert(v.begin(), x);
        // std::cout << X::sp;
        // std::cout << "----\n";
        ins = X::sp;
    }
    {
        uxs::vector<X> v;
        v.reserve(3);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        X x{0, 0};
        // std::cout << "--emplace lvalue reallocation--\n";
        X::sp = {};
        v.emplace(v.begin(), x);
        // std::cout << X::sp;
        // std::cout << "----\n";
        emp = X::sp;
    }
    VERIFY(ins == emp);
    VERIFY(ins == expected);
    return 0;
}

// insert vs emplace xvalue reallocation
int test05() {
    const X::special expected{0, 3, 0, 0, 4, 0};
    X::special ins, emp;
    {
        uxs::vector<X> v;
        v.reserve(3);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        X x{0, 0};
        // std::cout << "--insert xvalue reallocation--\n";
        X::sp = {};
        v.insert(v.begin(), std::move(x));
        // std::cout << X::sp;
        // std::cout << "----\n";
        ins = X::sp;
    }
    {
        uxs::vector<X> v;
        v.reserve(3);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        X x{0, 0};
        // std::cout << "--emplace xvalue reallocation--\n";
        X::sp = {};
        v.emplace(v.begin(), std::move(x));
        // std::cout << X::sp;
        // std::cout << "----\n";
        emp = X::sp;
    }
    VERIFY(ins == emp);
    VERIFY(ins == expected);
    return 0;
}

// insert vs emplace rvalue reallocation
int test06() {
    const X::special expected{1, 4, 0, 0, 4, 0};
    X::special ins, emp;
    {
        uxs::vector<X> v;
        v.reserve(3);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        // std::cout << "--insert rvalue reallocation--\n";
        X::sp = {};
        v.insert(v.begin(), X{0, 0});
        // std::cout << X::sp;
        // std::cout << "----\n";
        ins = X::sp;
    }
    {
        uxs::vector<X> v;
        v.reserve(3);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        // std::cout << "--emplace rvalue reallocation--\n";
        X::sp = {};
        v.emplace(v.begin(), X{0, 0});
        // std::cout << X::sp;
        // std::cout << "----\n";
        emp = X::sp;
    }
    VERIFY(ins == emp);
    VERIFY(ins == expected);
    return 0;
}

// push_back vs emplace_back lvalue no reallocation
int test07() {
    const X::special expected{0, 0, 1, 0, 0, 0};
    X::special ins, emp;
    {
        uxs::vector<X> v;
        v.reserve(4);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        X x{0, 0};
        // std::cout << "--push_back lvalue no reallocation--\n";
        X::sp = {};
        v.push_back(x);
        // std::cout << X::sp;
        // std::cout << "----\n";
        ins = X::sp;
    }
    {
        uxs::vector<X> v;
        v.reserve(4);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        X x{0, 0};
        // std::cout << "--emplace_back lvalue no reallocation--\n";
        X::sp = {};
        v.emplace_back(x);
        // std::cout << X::sp;
        // std::cout << "----\n";
        emp = X::sp;
    }
    VERIFY(ins == emp);
    VERIFY(ins == expected);
    return 0;
}

// push_back vs emplace_back xvalue no reallocation
int test08() {
    const X::special expected{0, 0, 0, 0, 1, 0};
    X::special ins, emp;
    {
        uxs::vector<X> v;
        v.reserve(4);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        X x{0, 0};
        // std::cout << "--push_back xvalue no reallocation--\n";
        X::sp = {};
        v.push_back(std::move(x));
        // std::cout << X::sp;
        // std::cout << "----\n";
        ins = X::sp;
    }
    {
        uxs::vector<X> v;
        v.reserve(4);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        X x{0, 0};
        // std::cout << "--emplace_back xvalue no reallocation--\n";
        X::sp = {};
        v.emplace_back(std::move(x));
        // std::cout << X::sp;
        // std::cout << "----\n";
        emp = X::sp;
    }
    VERIFY(ins == emp);
    VERIFY(ins == expected);
    return 0;
}

// push_back vs emplace_back rvalue no reallocation
int test09() {
    const X::special expected{1, 1, 0, 0, 1, 0};
    X::special ins, emp;
    {
        uxs::vector<X> v;
        v.reserve(4);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        // std::cout << "--push_back rvalue no reallocation--\n";
        X::sp = {};
        v.push_back(X{0, 0});
        // std::cout << X::sp;
        // std::cout << "----\n";
        ins = X::sp;
    }
    {
        uxs::vector<X> v;
        v.reserve(4);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        // std::cout << "--emplace_back rvalue no reallocation--\n";
        X::sp = {};
        v.emplace_back(X{0, 0});
        // std::cout << X::sp;
        // std::cout << "----\n";
        emp = X::sp;
    }
    VERIFY(ins == emp);
    VERIFY(ins == expected);
    return 0;
}

// push_back vs emplace_back lvalue reallocation
int test10() {
    const X::special expected{0, 3, 1, 0, 3, 0};
    X::special ins, emp;
    {
        uxs::vector<X> v;
        v.reserve(3);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        X x{0, 0};
        // std::cout << "--push_back lvalue reallocation--\n";
        X::sp = {};
        v.push_back(x);
        // std::cout << X::sp;
        // std::cout << "----\n";
        ins = X::sp;
    }
    {
        uxs::vector<X> v;
        v.reserve(3);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        X x{0, 0};
        // std::cout << "--emplace_back lvalue reallocation--\n";
        X::sp = {};
        v.emplace_back(x);
        // std::cout << X::sp;
        // std::cout << "----\n";
        emp = X::sp;
    }
    VERIFY(ins == emp);
    VERIFY(ins == expected);
    return 0;
}

// push_back vs emplace_back xvalue reallocation
int test11() {
    const X::special expected{0, 3, 0, 0, 4, 0};
    X::special ins, emp;
    {
        uxs::vector<X> v;
        v.reserve(3);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        X x{0, 0};
        // std::cout << "--push_back xvalue reallocation--\n";
        X::sp = {};
        v.push_back(std::move(x));
        // std::cout << X::sp;
        // std::cout << "----\n";
        ins = X::sp;
    }
    {
        uxs::vector<X> v;
        v.reserve(3);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        X x{0, 0};
        // std::cout << "--emplace_back xvalue reallocation--\n";
        X::sp = {};
        v.emplace_back(std::move(x));
        // std::cout << X::sp;
        // std::cout << "----\n";
        emp = X::sp;
    }
    VERIFY(ins == emp);
    VERIFY(ins == expected);
    return 0;
}

// push_back vs emplace_back rvalue reallocation
int test12() {
    const X::special expected{1, 4, 0, 0, 4, 0};
    X::special ins, emp;
    {
        uxs::vector<X> v;
        v.reserve(3);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        // std::cout << "--push_back rvalue reallocation--\n";
        X::sp = {};
        v.push_back(X{0, 0});
        // std::cout << X::sp;
        // std::cout << "----\n";
        ins = X::sp;
    }
    {
        uxs::vector<X> v;
        v.reserve(3);
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        v.push_back(X(0, 0));
        // std::cout << "--emplace_back rvalue reallocation--\n";
        X::sp = {};
        v.emplace_back(X{0, 0});
        // std::cout << X::sp;
        // std::cout << "----\n";
        emp = X::sp;
    }
    VERIFY(ins == emp);
    VERIFY(ins == expected);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
ADD_TEST_CASE("", "vector", test02);
ADD_TEST_CASE("", "vector", test03);
ADD_TEST_CASE("", "vector", test04);
ADD_TEST_CASE("", "vector", test05);
ADD_TEST_CASE("", "vector", test06);
ADD_TEST_CASE("", "vector", test07);
ADD_TEST_CASE("", "vector", test08);
ADD_TEST_CASE("", "vector", test09);
ADD_TEST_CASE("", "vector", test10);
ADD_TEST_CASE("", "vector", test11);
ADD_TEST_CASE("", "vector", test12);
