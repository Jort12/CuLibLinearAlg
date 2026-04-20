#include "linalg/matrix.hpp"
#include "linalg/ops.hpp"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

using linalg::Matrix;

static int g_passed = 0;
static int g_failed = 0;

static void report(const std::string& name, bool ok, const std::string& detail = "") {
    if (ok) {
        ++g_passed;
        std::cout << "  [PASS] " << name << "\n";
    } else {
        ++g_failed;
        std::cout << "  [FAIL] " << name;
        if (!detail.empty()) std::cout << "  --  " << detail;
        std::cout << "\n";
    }
}

template <typename F>
static void run(const std::string& name, F fn) {
    try {
        fn(name);
    } catch (const std::exception& e) {
        report(name, false, std::string("threw: ") + e.what());
    } catch (...) {
        report(name, false, "threw unknown exception");
    }
}

template <typename T>
static bool approx_equal(T a, T b, T tol = T(1e-5)) {
    return std::abs(a - b) <= tol;
}

template <typename T>
static bool matrices_equal(const Matrix<T>& a, const Matrix<T>& b, T tol = T(1e-5)) {
    if (a.rows() != b.rows() || a.cols() != b.cols()) return false;
    for (std::size_t i = 0; i < a.rows(); ++i) {
        for (std::size_t j = 0; j < a.cols(); ++j) {
            if (!approx_equal(a(i, j), b(i, j), tol)) return false;
        }
    }
    return true;
}

int main() {
    std::cout << "=== CuLibLinearAlg tests ===\n\n";

    // ---------------- Matrix construction and accessors ----------------
    std::cout << "-- Matrix<T> --\n";

    run("default constructor", [](const std::string& n) {
        Matrix<float> m;
        report(n, m.rows() == 0 && m.cols() == 0);
    });

    run("sized constructor (rows, cols)", [](const std::string& n) {
        Matrix<float> m(3, 4);
        report(n, m.rows() == 3 && m.cols() == 4);
    });

    run("sized-with-value constructor", [](const std::string& n) {
        Matrix<float> m(2, 2, 7.5f);
        bool ok = m(0, 0) == 7.5f && m(0, 1) == 7.5f
               && m(1, 0) == 7.5f && m(1, 1) == 7.5f;
        report(n, ok);
    });

    run("operator() read/write", [](const std::string& n) {
        Matrix<float> m(2, 2, 0.0f);
        m(0, 0) = 1.0f;
        m(0, 1) = 2.0f;
        m(1, 0) = 3.0f;
        m(1, 1) = 4.0f;
        bool ok = m(0, 0) == 1.0f && m(0, 1) == 2.0f
               && m(1, 0) == 3.0f && m(1, 1) == 4.0f;
        report(n, ok);
    });

    run("row-major layout via data()", [](const std::string& n) {
        Matrix<float> m(2, 3, 0.0f);
        m(0, 0) = 10.0f; m(0, 1) = 11.0f; m(0, 2) = 12.0f;
        m(1, 0) = 20.0f; m(1, 1) = 21.0f; m(1, 2) = 22.0f;
        const float* p = m.data();
        bool ok = p[0] == 10.0f && p[1] == 11.0f && p[2] == 12.0f
               && p[3] == 20.0f && p[4] == 21.0f && p[5] == 22.0f;
        report(n, ok);
    });

    run("fill()", [](const std::string& n) {
        Matrix<float> m(3, 3, 0.0f);
        m.fill(9.0f);
        bool ok = true;
        for (std::size_t i = 0; i < 3; ++i)
            for (std::size_t j = 0; j < 3; ++j)
                if (m(i, j) != 9.0f) ok = false;
        report(n, ok);
    });

    // ---------------- add ----------------
    std::cout << "\n-- add --\n";

    run("add: 2x2 + 2x2", [](const std::string& n) {
        Matrix<float> a(2, 2), b(2, 2);
        a(0,0)=1; a(0,1)=2; a(1,0)=3; a(1,1)=4;
        b(0,0)=10; b(0,1)=20; b(1,0)=30; b(1,1)=40;
        Matrix<float> c = linalg::add(a, b);
        bool ok = c(0,0)==11 && c(0,1)==22 && c(1,0)==33 && c(1,1)==44;
        report(n, ok);
    });

    run("add: dimension mismatch throws", [](const std::string& n) {
        Matrix<float> a(2, 2, 0.0f);
        Matrix<float> b(3, 2, 0.0f);
        bool threw = false;
        try { (void)linalg::add(a, b); } catch (...) { threw = true; }
        report(n, threw);
    });

    // ---------------- scale ----------------
    std::cout << "\n-- scale --\n";

    run("scale: all elements * alpha", [](const std::string& n) {
        Matrix<float> a(2, 2);
        a(0,0)=1; a(0,1)=2; a(1,0)=3; a(1,1)=4;
        Matrix<float> c = linalg::scale(a, 2.5f);
        bool ok = approx_equal(c(0,0), 2.5f)
               && approx_equal(c(0,1), 5.0f)
               && approx_equal(c(1,0), 7.5f)
               && approx_equal(c(1,1), 10.0f);
        report(n, ok);
    });

    run("scale by 0 gives zero matrix", [](const std::string& n) {
        Matrix<float> a(3, 3, 7.0f);
        Matrix<float> c = linalg::scale(a, 0.0f);
        bool ok = true;
        for (std::size_t i = 0; i < 3; ++i)
            for (std::size_t j = 0; j < 3; ++j)
                if (c(i,j) != 0.0f) ok = false;
        report(n, ok);
    });

    // ---------------- hadamard ----------------
    std::cout << "\n-- hadamard --\n";

    run("hadamard: elementwise product", [](const std::string& n) {
        Matrix<float> a(2, 2), b(2, 2);
        a(0,0)=1; a(0,1)=2; a(1,0)=3; a(1,1)=4;
        b(0,0)=5; b(0,1)=6; b(1,0)=7; b(1,1)=8;
        Matrix<float> c = linalg::hadamard(a, b);
        bool ok = c(0,0)==5 && c(0,1)==12 && c(1,0)==21 && c(1,1)==32;
        report(n, ok);
    });

    run("hadamard: dimension mismatch throws", [](const std::string& n) {
        Matrix<float> a(2, 3, 0.0f);
        Matrix<float> b(2, 2, 0.0f);
        bool threw = false;
        try { (void)linalg::hadamard(a, b); } catch (...) { threw = true; }
        report(n, threw);
    });

    // ---------------- transpose ----------------
    std::cout << "\n-- transpose --\n";

    run("transpose: 2x3 becomes 3x2", [](const std::string& n) {
        Matrix<float> a(2, 3);
        a(0,0)=1; a(0,1)=2; a(0,2)=3;
        a(1,0)=4; a(1,1)=5; a(1,2)=6;
        Matrix<float> t = linalg::transpose(a);
        bool shape_ok = (t.rows() == 3 && t.cols() == 2);
        bool vals_ok = t(0,0)==1 && t(1,0)==2 && t(2,0)==3
                    && t(0,1)==4 && t(1,1)==5 && t(2,1)==6;
        report(n, shape_ok && vals_ok);
    });

    run("transpose twice = original", [](const std::string& n) {
        Matrix<float> a(3, 4);
        for (std::size_t i = 0; i < 3; ++i)
            for (std::size_t j = 0; j < 4; ++j)
                a(i, j) = static_cast<float>(i * 10 + j);
        Matrix<float> tt = linalg::transpose(linalg::transpose(a));
        report(n, matrices_equal(a, tt));
    });

    // ---------------- gemv ----------------
    std::cout << "\n-- gemv --\n";

    run("gemv: 2x3 matrix * 3x1 vector", [](const std::string& n) {
        Matrix<float> a(2, 3);
        a(0,0)=1; a(0,1)=2; a(0,2)=3;
        a(1,0)=4; a(1,1)=5; a(1,2)=6;
        Matrix<float> x(3, 1);
        x(0,0)=1; x(1,0)=2; x(2,0)=3;
        Matrix<float> y = linalg::gemv(a, x);
        // expected: [1*1+2*2+3*3, 4*1+5*2+6*3] = [14, 32]
        bool ok = y.rows() == 2 && y.cols() == 1
               && approx_equal(y(0,0), 14.0f)
               && approx_equal(y(1,0), 32.0f);
        report(n, ok);
    });

    run("gemv: dimension mismatch throws", [](const std::string& n) {
        Matrix<float> a(2, 3, 0.0f);
        Matrix<float> x(2, 1, 0.0f);  // should be 3x1
        bool threw = false;
        try { (void)linalg::gemv(a, x); } catch (...) { threw = true; }
        report(n, threw);
    });

    // ---------------- matmul ----------------
    std::cout << "\n-- matmul --\n";

    run("matmul: 2x3 * 3x2", [](const std::string& n) {
        Matrix<float> a(2, 3);
        a(0,0)=1; a(0,1)=2; a(0,2)=3;
        a(1,0)=4; a(1,1)=5; a(1,2)=6;
        Matrix<float> b(3, 2);
        b(0,0)=7;  b(0,1)=8;
        b(1,0)=9;  b(1,1)=10;
        b(2,0)=11; b(2,1)=12;
        Matrix<float> c = linalg::matmul(a, b);
        // expected: [[58, 64], [139, 154]]
        bool ok = c.rows() == 2 && c.cols() == 2
               && approx_equal(c(0,0), 58.0f)
               && approx_equal(c(0,1), 64.0f)
               && approx_equal(c(1,0), 139.0f)
               && approx_equal(c(1,1), 154.0f);
        report(n, ok);
    });

    run("matmul: identity leaves matrix unchanged", [](const std::string& n) {
        Matrix<float> a(3, 3);
        a(0,0)=1; a(0,1)=2; a(0,2)=3;
        a(1,0)=4; a(1,1)=5; a(1,2)=6;
        a(2,0)=7; a(2,1)=8; a(2,2)=9;
        Matrix<float> I(3, 3, 0.0f);
        I(0,0) = I(1,1) = I(2,2) = 1.0f;
        Matrix<float> c = linalg::matmul(I, a);
        report(n, matrices_equal(a, c));
    });

    run("matmul: dimension mismatch throws", [](const std::string& n) {
        Matrix<float> a(2, 3, 0.0f);
        Matrix<float> b(4, 2, 0.0f);  // a.cols(3) != b.rows(4)
        bool threw = false;
        try { (void)linalg::matmul(a, b); } catch (...) { threw = true; }
        report(n, threw);
    });

    // ---------------- double precision sanity ----------------
    std::cout << "\n-- double precision --\n";

    run("double: add + scale + hadamard work", [](const std::string& n) {
        Matrix<double> a(2, 2, 1.0);
        Matrix<double> b(2, 2, 2.0);
        Matrix<double> s = linalg::scale(a, 3.0);
        Matrix<double> p = linalg::add(s, b);    // 3 + 2 = 5 everywhere
        Matrix<double> h = linalg::hadamard(p, b); // 5 * 2 = 10 everywhere
        bool ok = approx_equal(h(0,0), 10.0)
               && approx_equal(h(0,1), 10.0)
               && approx_equal(h(1,0), 10.0)
               && approx_equal(h(1,1), 10.0);
        report(n, ok);
    });

    // ---------------- stress tests (larger dimensions, with timing) ----------------
    std::cout << "\n-- stress --\n";

    auto time_ms = [](auto fn) {
        auto t0 = std::chrono::steady_clock::now();
        fn();
        auto t1 = std::chrono::steady_clock::now();
        return std::chrono::duration<double, std::milli>(t1 - t0).count();
    };

    run("stress: add 1024x1024", [&](const std::string& n) {
        const std::size_t N = 1024;
        Matrix<float> a(N, N, 1.5f);
        Matrix<float> b(N, N, 2.5f);
        Matrix<float> c;
        double ms = time_ms([&] { c = linalg::add(a, b); });
        bool shape_ok = c.rows() == N && c.cols() == N;
        bool corner_ok = approx_equal(c(0, 0), 4.0f)
                      && approx_equal(c(N/2, N/2), 4.0f)
                      && approx_equal(c(N-1, N-1), 4.0f);
        std::cout << "         (" << ms << " ms)\n";
        report(n, shape_ok && corner_ok);
    });

    run("stress: scale 1024x1024", [&](const std::string& n) {
        const std::size_t N = 1024;
        Matrix<float> a(N, N, 3.0f);
        Matrix<float> c;
        double ms = time_ms([&] { c = linalg::scale(a, 4.0f); });
        bool ok = c.rows() == N && c.cols() == N
               && approx_equal(c(0, 0), 12.0f)
               && approx_equal(c(N-1, N-1), 12.0f);
        std::cout << "         (" << ms << " ms)\n";
        report(n, ok);
    });

    run("stress: hadamard 1024x1024", [&](const std::string& n) {
        const std::size_t N = 1024;
        Matrix<float> a(N, N, 2.0f);
        Matrix<float> b(N, N, 5.0f);
        Matrix<float> c;
        double ms = time_ms([&] { c = linalg::hadamard(a, b); });
        bool ok = c.rows() == N && c.cols() == N
               && approx_equal(c(0, 0), 10.0f)
               && approx_equal(c(N-1, N-1), 10.0f);
        std::cout << "         (" << ms << " ms)\n";
        report(n, ok);
    });

    run("stress: transpose 512x1024 (non-square)", [&](const std::string& n) {
        const std::size_t R = 512, C = 1024;
        Matrix<float> a(R, C);
        for (std::size_t i = 0; i < R; ++i)
            for (std::size_t j = 0; j < C; ++j)
                a(i, j) = static_cast<float>(i * 1000 + j);
        Matrix<float> t;
        double ms = time_ms([&] { t = linalg::transpose(a); });
        bool shape_ok = t.rows() == C && t.cols() == R;
        // spot-check a few random cells
        bool vals_ok = approx_equal(t(0, 0), a(0, 0))
                    && approx_equal(t(500, 100), a(100, 500))
                    && approx_equal(t(C-1, R-1), a(R-1, C-1));
        std::cout << "         (" << ms << " ms)\n";
        report(n, shape_ok && vals_ok);
    });

    run("stress: transpose twice = original (512x512)", [&](const std::string& n) {
        const std::size_t N = 512;
        Matrix<float> a(N, N);
        for (std::size_t i = 0; i < N; ++i)
            for (std::size_t j = 0; j < N; ++j)
                a(i, j) = static_cast<float>(i) - static_cast<float>(j);
        Matrix<float> tt;
        double ms = time_ms([&] { tt = linalg::transpose(linalg::transpose(a)); });
        std::cout << "         (" << ms << " ms)\n";
        report(n, matrices_equal(a, tt));
    });

    run("stress: gemv 2048x2048 * 2048x1", [&](const std::string& n) {
        const std::size_t N = 2048;
        Matrix<float> a(N, N, 1.0f);  // all ones
        Matrix<float> x(N, 1, 2.0f);  // all twos
        Matrix<float> y;
        double ms = time_ms([&] { y = linalg::gemv(a, x); });
        // each output row = sum over N of (1 * 2) = 2*N
        const float expected = 2.0f * static_cast<float>(N);
        bool shape_ok = y.rows() == N && y.cols() == 1;
        bool vals_ok = approx_equal(y(0, 0), expected, 1e-2f)
                    && approx_equal(y(N/2, 0), expected, 1e-2f)
                    && approx_equal(y(N-1, 0), expected, 1e-2f);
        std::cout << "         (" << ms << " ms)\n";
        report(n, shape_ok && vals_ok);
    });

    run("stress: matmul 256x256 * 256x256 against identity", [&](const std::string& n) {
        const std::size_t N = 256;
        Matrix<float> a(N, N);
        for (std::size_t i = 0; i < N; ++i)
            for (std::size_t j = 0; j < N; ++j)
                a(i, j) = static_cast<float>((i * 31 + j) % 97);
        Matrix<float> I(N, N, 0.0f);
        for (std::size_t k = 0; k < N; ++k) I(k, k) = 1.0f;
        Matrix<float> c;
        double ms = time_ms([&] { c = linalg::matmul(a, I); });
        std::cout << "         (" << ms << " ms)\n";
        report(n, matrices_equal(a, c));
    });

    run("stress: matmul 256x256 ones = all-N matrix", [&](const std::string& n) {
        const std::size_t N = 256;
        Matrix<float> a(N, N, 1.0f);
        Matrix<float> b(N, N, 1.0f);
        Matrix<float> c;
        double ms = time_ms([&] { c = linalg::matmul(a, b); });
        // every entry of c should equal N (sum of N ones)
        const float expected = static_cast<float>(N);
        bool shape_ok = c.rows() == N && c.cols() == N;
        bool vals_ok = approx_equal(c(0, 0), expected, 1e-2f)
                    && approx_equal(c(N/2, N/2), expected, 1e-2f)
                    && approx_equal(c(N-1, N-1), expected, 1e-2f);
        std::cout << "         (" << ms << " ms)\n";
        report(n, shape_ok && vals_ok);
    });

    // ---------------- summary ----------------
    std::cout << "\n=== Summary ===\n";
    std::cout << "  passed: " << g_passed << "\n";
    std::cout << "  failed: " << g_failed << "\n";
    return g_failed == 0 ? 0 : 1;
}
