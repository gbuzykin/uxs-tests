#include "test_suite.h"
#include "util/string_cvt.h"
#include "util/vector.h"

#include <random>

#if defined(_MSC_VER) && __cplusplus >= 201703L
#    include <charconv>
#endif

namespace {

int test_string_cvt_0() {
    std::string s;

    VERIFY((s = util::to_string<double>(1.2345672222, util::scvt_fp::kFixed)) == "1.234567");
    VERIFY((s = util::to_string<double>(1.2345677777, util::scvt_fp::kFixed)) == "1.234568");
    VERIFY((s = util::to_string<double>(1.2345, util::scvt_fp::kFixed)) == "1.234500");
    VERIFY((s = util::to_string<double>(0.12345, util::scvt_fp::kFixed)) == "0.123450");
    VERIFY((s = util::to_string<double>(0.00124, util::scvt_fp::kFixed)) == "0.001240");
    VERIFY((s = util::to_string<double>(23.34563444, util::scvt_fp::kFixed)) == "23.345634");
    VERIFY((s = util::to_string<double>(23.34563464, util::scvt_fp::kFixed)) == "23.345635");
    VERIFY((s = util::to_string<double>(23.3456, util::scvt_fp::kFixed)) == "23.345600");
    VERIFY((s = util::to_string<double>(999.9999997, util::scvt_fp::kFixed)) == "1000.000000");
    VERIFY((s = util::to_string<double>(0, util::scvt_fp::kFixed)) == "0.000000");
    VERIFY((s = util::to_string<double>(0, util::scvt_fp::kFixed, 0)) == "0");

    VERIFY((s = util::to_string<double>(1000000000, util::scvt_fp::kFixed)) == "1000000000.000000");
    VERIFY((s = util::to_string<double>(231000000000000, util::scvt_fp::kFixed)) == "231000000000000.000000");

    VERIFY((s = util::to_string<double>(345., util::scvt_fp::kFixed)) == "345.000000");
    VERIFY((s = util::to_string<double>(345.234465876, util::scvt_fp::kFixed)) == "345.234466");
    VERIFY((s = util::to_string<double>(34.5234465876, util::scvt_fp::kFixed)) == "34.523447");
    VERIFY((s = util::to_string<double>(3.45234465876, util::scvt_fp::kFixed)) == "3.452345");
    VERIFY((s = util::to_string<double>(0.345234465876, util::scvt_fp::kFixed)) == "0.345234");
    VERIFY((s = util::to_string<double>(0.0234465876, util::scvt_fp::kFixed)) == "0.023447");
    VERIFY((s = util::to_string<double>(0.00234465876, util::scvt_fp::kFixed)) == "0.002345");
    VERIFY((s = util::to_string<double>(0.000234465876, util::scvt_fp::kFixed)) == "0.000234");
    VERIFY((s = util::to_string<double>(0.0000234465876, util::scvt_fp::kFixed)) == "0.000023");
    VERIFY((s = util::to_string<double>(0.00000234465876, util::scvt_fp::kFixed)) == "0.000002");
    VERIFY((s = util::to_string<double>(0.000000234465876, util::scvt_fp::kFixed)) == "0.000000");

    VERIFY((s = util::to_string<double>(345., util::scvt_fp::kFixed, 0)) == "345");
    VERIFY((s = util::to_string<double>(345.234465876, util::scvt_fp::kFixed, 0)) == "345");
    VERIFY((s = util::to_string<double>(34.5234465876, util::scvt_fp::kFixed, 0)) == "35");
    VERIFY((s = util::to_string<double>(3.45234465876, util::scvt_fp::kFixed, 0)) == "3");
    VERIFY((s = util::to_string<double>(0.345234465876, util::scvt_fp::kFixed, 0)) == "0");
    VERIFY((s = util::to_string<double>(0.0234465876, util::scvt_fp::kFixed, 0)) == "0");

    VERIFY((s = util::to_string<double>(0, util::scvt_fp::kScientific)) == "0.000000e+00");
    VERIFY((s = util::to_string<double>(0, util::scvt_fp::kScientific, 0)) == "0e+00");
    VERIFY((s = util::to_string<double>(0, util::scvt_fp::kGeneral)) == "0");

    VERIFY((s = util::to_string<double>(999999.9999996, util::scvt_fp::kGeneral)) == "1e+06");
    VERIFY((s = util::to_string<double>(3455, util::scvt_fp::kGeneral, 3)) == "3.46e+03");
    VERIFY((s = util::to_string<double>(3445, util::scvt_fp::kGeneral, 3)) == "3.44e+03");

    VERIFY((s = util::to_string<double>(0.003, util::scvt_fp::kGeneral)) == "0.003");
    VERIFY((s = util::to_string<double>(0.003000001, util::scvt_fp::kGeneral)) == "0.003");
    VERIFY((s = util::to_string<double>(0.00300001, util::scvt_fp::kGeneral)) == "0.00300001");
    VERIFY((s = util::to_string<double>(0.0030001, util::scvt_fp::kGeneral)) == "0.0030001");
    VERIFY((s = util::to_string<double>(0.003001, util::scvt_fp::kGeneral)) == "0.003001");

    VERIFY((s = util::to_string<double>(0.3, util::scvt_fp::kGeneral)) == "0.3");
    VERIFY((s = util::to_string<double>(0.3000001, util::scvt_fp::kGeneral)) == "0.3");
    VERIFY((s = util::to_string<double>(0.300001, util::scvt_fp::kGeneral)) == "0.300001");
    VERIFY((s = util::to_string<double>(0.30001, util::scvt_fp::kGeneral)) == "0.30001");
    VERIFY((s = util::to_string<double>(0.3001, util::scvt_fp::kGeneral)) == "0.3001");

    VERIFY((s = util::to_string<double>(345., util::scvt_fp::kGeneral)) == "345");
    VERIFY((s = util::to_string<double>(345.0001, util::scvt_fp::kGeneral)) == "345");
    VERIFY((s = util::to_string<double>(345.001, util::scvt_fp::kGeneral)) == "345.001");
    VERIFY((s = util::to_string<double>(345.01, util::scvt_fp::kGeneral)) == "345.01");
    VERIFY((s = util::to_string<double>(345.1, util::scvt_fp::kGeneral)) == "345.1");

    VERIFY((s = util::to_string<double>(3.e23, util::scvt_fp::kGeneral)) == "3e+23");
    VERIFY((s = util::to_string<double>(3.000001e23, util::scvt_fp::kGeneral)) == "3e+23");
    VERIFY((s = util::to_string<double>(3.00001e23, util::scvt_fp::kGeneral)) == "3.00001e+23");
    VERIFY((s = util::to_string<double>(3.0001e23, util::scvt_fp::kGeneral)) == "3.0001e+23");
    VERIFY((s = util::to_string<double>(3.001e23, util::scvt_fp::kGeneral)) == "3.001e+23");

    VERIFY((s = util::to_string<double>(345., util::scvt_fp::kGeneral, 0)) == "3e+02");
    VERIFY((s = util::to_string<double>(345.234465876, util::scvt_fp::kGeneral, 0)) == "3e+02");
    VERIFY((s = util::to_string<double>(34.5234465876, util::scvt_fp::kGeneral, 0)) == "3e+01");
    VERIFY((s = util::to_string<double>(3.45234465876, util::scvt_fp::kGeneral, 0)) == "3");
    VERIFY((s = util::to_string<double>(0.345234465876, util::scvt_fp::kGeneral, 0)) == "0.3");
    VERIFY((s = util::to_string<double>(0.0234465876, util::scvt_fp::kGeneral, 0)) == "0.02");

    VERIFY((s = util::to_string<double>(0.0000006, util::scvt_fp::kFixed)) == "0.000001");
    VERIFY((s = util::to_string<double>(0.000000500001, util::scvt_fp::kFixed)) == "0.000001");
    VERIFY((s = util::to_string<double>(0.0000004, util::scvt_fp::kFixed)) == "0.000000");
    VERIFY((s = util::to_string<double>(0.00000049999, util::scvt_fp::kFixed)) == "0.000000");
    VERIFY((s = util::to_string<double>(0.00000006, util::scvt_fp::kFixed)) == "0.000000");

    double d = 2000000. / 7.;  // 285714.28571428573923185468

    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kScientific, 17)) == "2.85714285714285739e+05");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kScientific, 16)) == "2.8571428571428574e+05");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kScientific, 15)) == "2.857142857142857e+05");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kScientific, 13)) == "2.8571428571429e+05");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kScientific, 2)) == "2.86e+05");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kScientific, 1)) == "2.9e+05");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kScientific, 0)) == "3e+05");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kScientific, 18)) == "2.857142857142857390e+05");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kScientific, 20)) == "2.85714285714285739000e+05");

    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kFixed, 12)) == "285714.285714285739");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kFixed, 11)) == "285714.28571428574");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kFixed, 10)) == "285714.2857142857");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kFixed, 8)) == "285714.28571429");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kFixed, 2)) == "285714.29");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kFixed, 1)) == "285714.3");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kFixed, 0)) == "285714");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kFixed, 13)) == "285714.2857142857390");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kFixed, 15)) == "285714.285714285739000");

    d = 285714.;

    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kFixed, 8)) == "285714.00000000");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kFixed, 2)) == "285714.00");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kFixed, 1)) == "285714.0");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kFixed, 0)) == "285714");

    d = 285714e+20;  // 28571400000000001222639616.00000000

    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kFixed, 8)) == "28571400000000001200000000.00000000");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kFixed, 2)) == "28571400000000001200000000.00");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kFixed, 1)) == "28571400000000001200000000.0");
    VERIFY((s = util::to_string<double>(d, util::scvt_fp::kFixed, 0)) == "28571400000000001200000000");

    VERIFY((s = util::to_string<double>(0.0006382, util::scvt_fp::kFixed, 7)) == "0.0006382");
    VERIFY((s = util::to_string<double>(0.0006382, util::scvt_fp::kFixed, 6)) == "0.000638");
    VERIFY((s = util::to_string<double>(0.0006382, util::scvt_fp::kFixed, 5)) == "0.00064");
    VERIFY((s = util::to_string<double>(0.0006382, util::scvt_fp::kFixed, 4)) == "0.0006");
    VERIFY((s = util::to_string<double>(0.0006382, util::scvt_fp::kFixed, 3)) == "0.001");
    VERIFY((s = util::to_string<double>(0.0006382, util::scvt_fp::kFixed, 2)) == "0.00");
    VERIFY((s = util::to_string<double>(0.0006382, util::scvt_fp::kFixed, 1)) == "0.0");
    VERIFY((s = util::to_string<double>(0.0006382, util::scvt_fp::kFixed, 0)) == "0");

    VERIFY((s = util::to_string<double>(0.6382, util::scvt_fp::kFixed, 4)) == "0.6382");
    VERIFY((s = util::to_string<double>(0.6382, util::scvt_fp::kFixed, 3)) == "0.638");
    VERIFY((s = util::to_string<double>(0.6382, util::scvt_fp::kFixed, 2)) == "0.64");
    VERIFY((s = util::to_string<double>(0.6382, util::scvt_fp::kFixed, 1)) == "0.6");
    VERIFY((s = util::to_string<double>(0.6382, util::scvt_fp::kFixed, 0)) == "1");

    VERIFY((s = util::to_string<double>(0.0004382, util::scvt_fp::kFixed, 3)) == "0.000");
    VERIFY((s = util::to_string<double>(0.4382, util::scvt_fp::kFixed, 0)) == "0");

    VERIFY(util::to_string<int>(10) == "10");
    VERIFY(util::to_string<int>(-25510) == "-25510");

    VERIFY(util::to_string<unsigned>(10) == "10");
    VERIFY(util::to_string<unsigned>(-25510) == "4294941786");

    VERIFY(fabs(util::from_string<float>(util::to_string<float>(0.2355f)) - 0.2355f) < 0.000001);
    VERIFY(fabs(util::from_string<double>(util::to_string<double>(-123.56e-1)) - -123.56e-1) < 0.000001);

    VERIFY(util::to_string(util::from_string<double>("2.22507385850720139e-308")) == "2.22507e-308");
    VERIFY(util::to_string(util::from_string<double>("2.22507385850720138e-308")) == "2.22507e-308");
    VERIFY(util::to_string(util::from_string<double>("1.11253692925360070e-308")) == "1.11254e-308");
    VERIFY(util::to_string(util::from_string<double>("1.11253692925360069e-308")) == "1.11254e-308");
    VERIFY(util::to_string(util::from_string<double>("5.56268464626800346e-309")) == "5.56268e-309");
    VERIFY(util::to_string(util::from_string<double>("5.56268464626800345e-309")) == "5.56268e-309");
    VERIFY(util::to_string(util::from_string<double>("5.30498947741318079e-315")) == "5.30499e-315");
    VERIFY(util::to_string(util::from_string<double>("5.30498947741318078e-315")) == "5.30499e-315");
    VERIFY(util::to_string(util::from_string<double>("5.05923221341436462e-321")) == "5.05923e-321");
    VERIFY(util::to_string(util::from_string<double>("5.05923221341436461e-321")) == "5.05923e-321");
    VERIFY(util::to_string(util::from_string<double>("1.97626258336498618e-323")) == "1.97626e-323");
    VERIFY(util::to_string(util::from_string<double>("1.97626258336498617e-323")) == "1.97626e-323");
    VERIFY(util::to_string(util::from_string<double>("9.88131291682493089e-324")) == "9.88131e-324");
    VERIFY(util::to_string(util::from_string<double>("9.88131291682493088e-324")) == "9.88131e-324");
    VERIFY(util::to_string(util::from_string<double>("4.94065645841246545e-324")) == "4.94066e-324");
    VERIFY(util::to_string(util::from_string<double>("4.94065645841246544e-324")) == "4.94066e-324");
    VERIFY(util::to_string(util::from_string<double>("2.47032822920623273e-324")) == "4.94066e-324");
    VERIFY(util::to_string(util::from_string<double>("2.47032822920623272e-324")) == "0");

    VERIFY(util::to_string<double>(std::numeric_limits<double>::infinity()) == "inf");
    VERIFY(util::to_string<double>(-std::numeric_limits<double>::infinity()) == "-inf");
    VERIFY(util::to_string<double>(std::numeric_limits<double>::quiet_NaN()).find("nan") != std::string::npos);

    VERIFY(std::isinf(util::from_string<double>("inf")));
    VERIFY(std::isinf(util::from_string<double>("+inf")));
    VERIFY(std::isinf(-util::from_string<double>("-inf")));
    VERIFY(std::isnan(util::from_string<double>("nan")));
    VERIFY(std::isnan(util::from_string<double>("+nan")));
    VERIFY(std::isnan(util::from_string<double>("-nan")));

    VERIFY(util::from_string<int>(" \t 10") == 10);
    VERIFY(util::from_string<int>("  \t  -25510") == -25510);
    VERIFY(util::from_string<int>("  \t  +2510") == 2510);

    VERIFY(util::from_string<unsigned>(" \t  10") == 10);
    VERIFY(util::from_string<unsigned>("\t    25510") == 25510);

    VERIFY(fabs(util::from_string<float>(" \t 0.2355") - 0.2355f) < 0.000001);
    VERIFY(fabs(util::from_string<float>(" \t .2355") - 0.2355f) < 0.000001);
    VERIFY(fabs(util::from_string<float>(" \t .3") - 0.3f) < 0.000001);
    VERIFY(fabs(util::from_string<double>("   \t  -123.56e-1") - -123.56e-1) < 0.000001);
    VERIFY(fabs(util::from_string<double>("   \t  -123.56e-0047") - -123.56e-47) < 0.000001);

    VERIFY(util::from_string<bool>(" \t   true") == true);
    VERIFY(util::from_string<bool>(" \t   false") == false);
    VERIFY(util::from_string<bool>(" \t   1") == true);
    VERIFY(util::from_string<bool>(" \t   001") == true);
    VERIFY(util::from_string<bool>(" \t   000") == false);

    util::vector<std::tuple<std::string_view, int, double>> d_tst;
    d_tst.emplace_back(" \t   fhjjh", 5, 12345.);
    d_tst.emplace_back(" \t   +fhjjh", 5, 12345.);
    d_tst.emplace_back(" \t   -fhjjh", 5, 12345.);
    d_tst.emplace_back(" \t   .fhjjh", 5, 12345.);
    d_tst.emplace_back(" \t   +.fhjjh", 5, 12345.);
    d_tst.emplace_back(" \t   -.fhjjh", 5, 12345.);
    d_tst.emplace_back(" \t   +", 5, 12345.);
    d_tst.emplace_back(" \t   -", 5, 12345.);
    d_tst.emplace_back(" \t   .", 5, 12345.);
    d_tst.emplace_back(" \t   +.", 5, 12345.);
    d_tst.emplace_back(" \t   -.", 5, 12345.);
    d_tst.emplace_back(" \t   .123fhjjh", 9, 0.123);
    d_tst.emplace_back(" \t   +.123fhjjh", 10, 0.123);
    d_tst.emplace_back(" \t   -.123fhjjh", 10, -0.123);
    d_tst.emplace_back(" \t   123wert", 8, 123.);
    d_tst.emplace_back(" \t   +123wert", 9, 123.);
    d_tst.emplace_back(" \t   -123wert", 9, -123.);
    d_tst.emplace_back(" \t   123.wert", 9, 123.);
    d_tst.emplace_back(" \t   123.0wert", 10, 123.);
    d_tst.emplace_back(" \t   123.1wert", 10, 123.1);
    d_tst.emplace_back(" \t   123.1esd", 10, 123.1);
    d_tst.emplace_back(" \t   123.1e1sd", 12, 1231.);

    for (const auto& el : d_tst) {
        double d = 12345;
        VERIFY(util::string_converter<double>::from_string(std::get<0>(el), d) ==
                   std::get<0>(el).data() + std::get<1>(el) &&
               d == std::get<2>(el));
    }

    util::vector<std::tuple<std::string_view, int, int>> i_tst;
    i_tst.emplace_back(" \t   fhjjh", 5, 12345);
    i_tst.emplace_back(" \t   +fhjjh", 5, 12345);
    i_tst.emplace_back(" \t   -fhjjh", 5, 12345);
    i_tst.emplace_back(" \t   +", 5, 12345);
    i_tst.emplace_back(" \t   -", 5, 12345);
    i_tst.emplace_back(" \t   123wert", 8, 123);
    i_tst.emplace_back(" \t   +123wert", 9, 123);
    i_tst.emplace_back(" \t   -123wert", 9, -123);

    for (const auto& el : i_tst) {
        double i = 12345;
        VERIFY(util::string_converter<double>::from_string(std::get<0>(el), i) ==
                   std::get<0>(el).data() + std::get<1>(el) &&
               i == std::get<2>(el));
    }
    return 0;
}

// --------------------------------------------

void test_bruteforce1(int iter_count) {
    std::array<char, 128> buf;
    std::default_random_engine generator;
    std::uniform_int_distribution<uint64_t> distribution(4, (1ull << 52) - 2);

    int N_err = 1;

    for (int iter = 0, perc0 = -1; iter < iter_count; ++iter) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            std::cout << std::setw(3) << (perc / 10) << "." << std::setw(0) << (perc % 10) << "%\b\b\b\b\b\b"
                      << std::flush;
            perc0 = perc;
        }

        for (int k = 0; k <= 140; ++k) {
            int exp = 1023 - 70 + k;
            uint64_t mantissa = 0;
            if (iter > 0) {
                if (iter <= 52) {
                    mantissa = 1ull << (iter - 1);
                } else if (iter <= 103) {
                    mantissa = ((1ull << 52) - 1) >> (iter - 53);
                } else {
                    mantissa = distribution(generator);
                }
            }
            uint64_t uval = mantissa | (static_cast<uint64_t>(exp) << 52);
            auto val = *reinterpret_cast<double*>(&uval);

            for (int prec = 18; prec >= 0; --prec) {
                auto s = util::to_string(val, util::scvt_fp::kFixed, prec);
                int n_digs = static_cast<int>(s.length());
                if (s[0] == '0') {
                    n_digs = 0;
                } else {
                    auto pos = s.find('.');
                    if (pos != std::string::npos) { n_digs = static_cast<int>(pos); }
                }

                n_digs += prec;

                if (n_digs <= 18) {
#if defined(_MSC_VER) && __cplusplus >= 201703L
                    auto result = std::to_chars(buf.data(), buf.data() + buf.size(), val, std::chars_format::fixed,
                                                prec);
                    std::string s_ref(buf.data(), result.ptr);
#else
                    int result = sprintf(buf.data(), "%.*lf", prec, val);
                    std::string s_ref(buf.data(), buf.data() + result);
#endif

                    if (s != s_ref) {
                        auto u64 = *reinterpret_cast<uint64_t*>(&val);
                        int exp = static_cast<int>((u64 >> 52) & 2047) - 1023;
                        u64 &= (1ull << 52) - 1;
                        std::cout << std::endl << "k = " << k << " iter = " << iter << " prec = " << prec << std::endl;
                        std::cout << s << std::endl << s_ref << std::endl;
                        std::cout << "mantissa = " << u64 << std::endl;
                        std::cout << "exp = " << exp << std::endl;
                        VERIFY(--N_err > 0);
                    }

                    double val1 = 0, val2 = 0;
                    const char* p = util::string_converter<double>::from_string(s, val1);
                    VERIFY(p == s.data() + s.size());
#if defined(_MSC_VER) && __cplusplus >= 201703L
                    std::from_chars(s.data(), s.data() + s.size(), val2);
#else
                    sscanf(s.c_str(), "%lf", &val2);
#endif
                    if (val1 != val2) {
                        std::cout << std::endl << s << std::endl;
                        printf("%.*lf\n", prec, val1);
                        printf("%.*lf\n", prec, val2);
                        VERIFY(--N_err > 0);
                    }
                }
            }
        }
    }
}

void test_bruteforce2(bool general, int iter_count) {
    std::array<char, 128> buf;
    std::default_random_engine generator;
    std::uniform_int_distribution<uint64_t> distribution(4, (1ull << 52) - 2);

    int N_err = 1;

    for (int iter = 0, perc0 = -1; iter < iter_count; ++iter) {
        int perc = (1000 * static_cast<int64_t>(iter)) / iter_count;
        if (perc > perc0) {
            std::cout << std::setw(3) << (perc / 10) << "." << std::setw(0) << (perc % 10) << "%\b\b\b\b\b\b"
                      << std::flush;
            perc0 = perc;
        }

        for (int k = 0; k <= 2046; ++k) {
            int exp = k;
            uint64_t mantissa = 0;
            if (iter > 0) {
                if (iter <= 52) {
                    mantissa = 1ull << (iter - 1);
                } else if (iter <= 103) {
                    mantissa = ((1ull << 52) - 1) >> (iter - 53);
                } else {
                    mantissa = distribution(generator);
                }
            }
            uint64_t uval = mantissa | (static_cast<uint64_t>(exp) << 52);
            auto val = *reinterpret_cast<double*>(&uval);

            for (int prec = general ? 18 : 17; prec >= 1; --prec) {
                auto s = util::to_string(val, general ? util::scvt_fp::kGeneral : util::scvt_fp::kScientific, prec);
#if defined(_MSC_VER) && __cplusplus >= 201703L
                auto result = std::to_chars(buf.data(), buf.data() + buf.size(), val,
                                            general ? std::chars_format::general : std::chars_format::scientific, prec);
                std::string s_ref(buf.data(), result.ptr);
#else
                int result = sprintf(buf.data(), general ? "%.*lg" : "%.*le", prec, val);
                std::string s_ref(buf.data(), buf.data() + result);
#endif
                if (s != s_ref) {
                    auto u64 = *reinterpret_cast<uint64_t*>(&val);
                    int exp = static_cast<int>((u64 >> 52) & 2047) - 1023;
                    u64 &= (1ull << 52) - 1;
                    std::cout << std::endl << "k = " << k << " iter = " << iter << " prec = " << prec << std::endl;
                    std::cout << s << std::endl << s_ref << std::endl;
                    std::cout << "mantissa = " << u64 << std::endl;
                    std::cout << "exp = " << exp << std::endl;
                    VERIFY(--N_err > 0);
                }

                double val1 = 0, val2 = 0;
                const char* p = util::string_converter<double>::from_string(s, val1);
                VERIFY(p == s.data() + s.size());
#if defined(_MSC_VER) && __cplusplus >= 201703L
                std::from_chars(s.data(), s.data() + s.size(), val2);
#else
                sscanf(s.c_str(), "%lf", &val2);
#endif
                if (val1 != val2 || (prec == 18 && val1 != val)) {
                    std::cout << std::endl << s << std::endl;
                    printf(general ? "%.*lg\n" : "%.*le\n", prec, val1);
                    printf(general ? "%.*lg\n" : "%.*le\n", prec, val2);
                    VERIFY(--N_err > 0);
                }
            }
        }
    }
}

#if defined(NDEBUG)
const int brute_N = 20000;
#else   // defined(NDEBUG)
const int brute_N = 200;
#endif  // defined(NDEBUG)

int test_string_cvt_1() {
    test_bruteforce1(25 * brute_N);
    return 0;
}
int test_string_cvt_2() {
    test_bruteforce2(false, brute_N);
    return 0;
}
int test_string_cvt_3() {
    test_bruteforce2(true, brute_N);
    return 0;
}

// --------------------------------------------

int perf(int iter_count) {
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(1., 10.);
    std::uniform_int_distribution<int> exp_distribution(-324, 308);

    double eps = 0;
    const int prec = 18;

    auto start = std::clock();
    for (int iter = 0; iter < iter_count; ++iter) {
        auto val = distribution(generator) * std::pow(10, exp_distribution(generator));
        auto s = util::to_string(val, util::scvt_fp::kGeneral, prec);
        auto val1 = util::from_string<double>(s);
        eps = std::max(std::fabs((val - val1) / val), eps);
    }

    return eps == 0 ? static_cast<int>(std::clock() - start) : 0;
}

int perf_std(int iter_count) {
    std::array<char, 128> buf;
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(1., 10.);
    std::uniform_int_distribution<int> exp_distribution(-324, 308);

    double eps = 0;
    const int prec = 18;

    auto start = std::clock();
    for (int iter = 0; iter < iter_count; ++iter) {
        auto val = distribution(generator) * std::pow(10, exp_distribution(generator));

#if defined(_MSC_VER) && __cplusplus >= 201703L
        auto result = std::to_chars(buf.data(), buf.data() + buf.size(), val, std::chars_format::general, prec);
        std::string s(buf.data(), result.ptr);
#else
        int result = sprintf(buf.data(), "%.*lg", prec, val);
        std::string s(buf.data(), buf.data() + result);
#endif

        double val1 = 0;
#if defined(_MSC_VER) && __cplusplus >= 201703L
        std::from_chars(s.data(), s.data() + s.size(), val1);
#else
        sscanf(s.c_str(), "%lf", &val1);
#endif
        eps = std::max(std::fabs((val - val1) / val), eps);
    }

    return eps == 0 ? static_cast<int>(std::clock() - start) : 0;
}

const int perf_N = 1000000;
int test_perf() { return perf(perf_N); }
int test_perf_std() { return perf_std(perf_N); }

}  // namespace

ADD_TEST_CASE("", "string convertion", test_string_cvt_0);

ADD_TEST_CASE("1-bruteforce", "string convertion", test_string_cvt_1);
ADD_TEST_CASE("1-bruteforce", "string convertion", test_string_cvt_2);
ADD_TEST_CASE("1-bruteforce", "string convertion", test_string_cvt_3);

ADD_TEST_CASE("2-perf", "string convertion", test_perf);
ADD_TEST_CASE("2-perf", "<std> string convertion", test_perf_std);
