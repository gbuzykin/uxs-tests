#include "test_suite.h"
#include "util/format.h"
#include "util/io/devbuf.h"
#include "util/io/iobuf_iterator.h"
#include "util/io/istringbuf.h"
#include "util/io/ostringbuf.h"

#include <random>
#include <sstream>

namespace {

class sdev : public util::iodevice {
 public:
    sdev() = default;
    explicit sdev(std::string str) : str_(str) {}
    const std::string& str() const { return str_; }

 protected:
    int read(void* data, size_type sz, size_type& n_read) override {
        n_read = std::min(sz, str_.size() - pos_);
        std::copy_n(&str_[pos_], n_read, reinterpret_cast<char*>(data));
        pos_ += n_read;
        return 0;
    }

    int write(const void* data, size_type sz, size_type& n_written) override {
        const char* p = reinterpret_cast<const char*>(data);
        size_type sz1 = std::min(str_.size() - pos_, sz);
        std::copy_n(p, sz1, &str_[pos_]);
        p += sz1;
        str_.append(p, sz - sz1);
        n_written = sz;
        pos_ += n_written;
        return 0;
    }

    int64_t seek(int64_t off, util::seekdir dir) override {
        switch (dir) {
            case util::seekdir::kBeg: {
                VERIFY(off >= 0);
                pos_ = off;
            } break;
            case util::seekdir::kCurr: {
                VERIFY(off >= 0 || static_cast<size_type>(-off) <= pos_);
                pos_ += off;
            } break;
            case util::seekdir::kEnd: {
                VERIFY(off >= 0 || static_cast<size_type>(-off) <= str_.size());
                pos_ = str_.size() + off;
            } break;
        }
        if (pos_ > str_.size()) { str_.resize(pos_); }
        return pos_;
    }

    int ctrlesc_color(util::span<uint8_t> v) override { return -1; }
    int flush() override { return 0; }

 private:
    std::string str_;
    size_type pos_ = 0;
};

const int brute_N = 10000000;

int test_iobuf_crlf() {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    util::stdbuf::out.write("      \b\b\b\b\b\b").flush();

    util::ostringbuf ss;
    std::ostringstream ss_ref;
    while (ss_ref.tellp() < brute_N) {
        VERIFY(ss.tell() == ss_ref.tellp());
        for (unsigned n = distribution(generator); n > 0; --n) {
            char ch = ' ' + static_cast<char>(distribution(generator) % ('z' - ' '));
            ss.put(ch);
            ss_ref.put(ch);
        }
        ss.put('\n');
        ss_ref.put('\n');
    }

    std::string str = ss.str();
    VERIFY(str == ss_ref.str());

    sdev middev;

    {
        util::istringbuf ifile(str);
        util::devbuf middle(middev, util::iomode::kOut | util::iomode::kCrLf);

        util::ibuf_iterator in(ifile), in_end{};
        util::obuf_iterator out(middle);
        std::copy(in, in_end, out);
    }

    VERIFY(middev.str().size() > str.size());
    for (unsigned n = 1; n < middev.str().size(); ++n) {
        VERIFY(middev.str()[n] != '\n' || middev.str()[n - 1] == '\r');
    }

    util::ostringbuf ss2;

    {
        sdev idev(middev.str());
        util::devbuf ifile(idev, util::iomode::kIn | util::iomode::kCrLf);

        util::ibuf_iterator in(ifile), in_end{};
        util::obuf_iterator out(ss2);
        std::copy(in, in_end, out);
    }

    VERIFY(str == ss2.str());
    return 0;
}

int test_iobuf_dev_sequential() {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    util::stdbuf::out.write("      \b\b\b\b\b\b").flush();

    sdev dev;
    std::ostringstream ss_ref;

    {
        util::devbuf ofile(dev, util::iomode::kOut);
        util::obuf_iterator out(ofile);
        std::ostreambuf_iterator<char> out_ref(ss_ref);

        while (ss_ref.tellp() < brute_N) {
            VERIFY(ofile.tell() == ss_ref.tellp());
            char ch = ' ' + static_cast<char>(distribution(generator) % ('z' - ' '));
            *out = ch;
            *out_ref = ch;
        }
        ss_ref.flush();
    }

    std::string str = dev.str();
    VERIFY(str.size() == brute_N);
    VERIFY(str == ss_ref.str());

    {
        util::devbuf ifile(dev, util::iomode::kIn);
        std::istringstream in_ss_ref(str);
        ifile.seek(0);

        util::ibuf_iterator in(ifile), in_end{};
        std::istreambuf_iterator<char> in_ref(in_ss_ref);
        VERIFY(in != in_end);
        do {
            VERIFY(ifile.tell() == in_ss_ref.tellg());
            VERIFY(*in == *in_ref);
            ++in, ++in_ref;
        } while (in != in_end);
        VERIFY(ifile.peek() == std::char_traits<char>::eof());
        VERIFY(in_ss_ref.peek() == std::char_traits<char>::eof());
    }

    return 0;
}

int test_iobuf_dev_sequential_str() {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    util::stdbuf::out.write("      \b\b\b\b\b\b").flush();

    util::ostringbuf ofile;
    std::ostringstream ss_ref;

    {
        util::obuf_iterator out(ofile);
        std::ostreambuf_iterator<char> out_ref(ss_ref);

        while (ss_ref.tellp() < brute_N) {
            VERIFY(ofile.tell() == ss_ref.tellp());
            char ch = ' ' + static_cast<char>(distribution(generator) % ('z' - ' '));
            *out = ch;
            *out_ref = ch;
        }
        ofile.flush();
        ss_ref.flush();
    }

    std::string str = ofile.str();
    VERIFY(str.size() == brute_N);
    VERIFY(str == ss_ref.str());

    {
        util::istringbuf ifile(str);
        std::istringstream in_ss_ref(str);

        util::ibuf_iterator in(ifile), in_end{};
        std::istreambuf_iterator<char> in_ref(in_ss_ref);
        VERIFY(in != in_end);
        do {
            VERIFY(ifile.tell() == in_ss_ref.tellg());
            VERIFY(*in == *in_ref);
            ++in, ++in_ref;
        } while (in != in_end);
        VERIFY(ifile.peek() == std::char_traits<char>::eof());
        VERIFY(in_ss_ref.peek() == std::char_traits<char>::eof());
    }

    return 0;
}

int test_iobuf_dev_sequential_block() {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    util::stdbuf::out.write("      \b\b\b\b\b\b").flush();

    sdev dev;
    std::ostringstream ss_ref;

    {
        util::devbuf ofile(dev, util::iomode::kOut);

        while (ss_ref.tellp() < brute_N) {
            VERIFY(ofile.tell() == ss_ref.tellp());
            unsigned sz = distribution(generator);
            char buf[256];
            for (unsigned n = 0; n < sz; ++n) {
                buf[n] = ' ' + static_cast<char>(distribution(generator) % ('z' - ' '));
            }
            ofile.write(util::as_span(buf, sz));
            ss_ref.write(buf, sz);
        }
        ss_ref.flush();
    }

    std::string str = dev.str();
    VERIFY(str.size() >= brute_N);
    VERIFY(str == ss_ref.str());

    {
        util::devbuf ifile(dev, util::iomode::kIn);
        std::istringstream in_ss_ref(str);
        ifile.seek(0);

        while (ifile) {
            VERIFY(ifile.tell() == in_ss_ref.tellg());
            unsigned sz = distribution(generator);
            char buf1[256], buf2[256];
            size_t n_read = ifile.read(util::as_span(buf1, sz));
            in_ss_ref.read(buf2, sz);
            VERIFY(static_cast<std::streamsize>(n_read) == in_ss_ref.gcount());
            VERIFY(std::equal(buf1, buf1 + n_read, buf2));
        }
        VERIFY(ifile.peek() == std::char_traits<char>::eof());
        VERIFY(in_ss_ref.peek() == std::char_traits<char>::eof());
    }

    return 0;
}

int test_iobuf_dev_sequential_block_str() {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    util::stdbuf::out.write("      \b\b\b\b\b\b").flush();

    util::ostringbuf ofile;
    std::ostringstream ss_ref;

    {
        while (ss_ref.tellp() < brute_N) {
            VERIFY(ofile.tell() == ss_ref.tellp());
            unsigned sz = distribution(generator);
            char buf[256];
            for (unsigned n = 0; n < sz; ++n) {
                buf[n] = ' ' + static_cast<char>(distribution(generator) % ('z' - ' '));
            }
            ofile.write(util::as_span(buf, sz));
            ss_ref.write(buf, sz);
        }
        ofile.flush();
        ss_ref.flush();
    }

    std::string str = ofile.str();
    VERIFY(str.size() >= brute_N);
    VERIFY(str == ss_ref.str());

    {
        util::istringbuf ifile(str);
        std::istringstream in_ss_ref(str);

        while (ifile) {
            VERIFY(ifile.tell() == in_ss_ref.tellg());
            unsigned sz = distribution(generator);
            char buf1[256], buf2[256];
            size_t n_read = ifile.read(util::as_span(buf1, sz));
            in_ss_ref.read(buf2, sz);
            VERIFY(static_cast<std::streamsize>(n_read) == in_ss_ref.gcount());
            VERIFY(std::equal(buf1, buf1 + n_read, buf2));
        }
        VERIFY(ifile.peek() == std::char_traits<char>::eof());
        VERIFY(in_ss_ref.peek() == std::char_traits<char>::eof());
    }

    return 0;
}

int test_iobuf_dev_random_block() {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 1000000000);

    sdev dev;
    util::ostringbuf ss_ref;

    int iter_count = brute_N;

    {
        util::devbuf ofile(dev, util::iomode::kOut);

        for (int i = 0, perc0 = -1; i < iter_count; ++i) {
            int perc = (500 * static_cast<int64_t>(i)) / iter_count;
            if (perc > perc0) {
                util::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
                perc0 = perc;
            }

            VERIFY(ofile.tell() == ss_ref.tell());
            unsigned sz = distribution(generator) % 128;
            unsigned tot_size = static_cast<unsigned>(ss_ref.seek(0, util::seekdir::kEnd));
            unsigned pos = distribution(generator) % (tot_size + std::max<unsigned>(tot_size / 1000, 16));
            char buf[256];
            for (unsigned n = 0; n < sz; ++n) {
                buf[n] = ' ' + static_cast<char>(distribution(generator) % ('z' - ' '));
            }

            ofile.seek(pos);
            ss_ref.seek(pos);
            VERIFY(ofile.tell() == ss_ref.tell());

            ofile.write(util::as_span(buf, sz));
            ss_ref.write(util::as_span(buf, sz));
        }
        ss_ref.flush();
    }

    std::string str = dev.str();
    VERIFY(str == ss_ref.str());

    {
        util::devbuf ifile(dev, util::iomode::kIn);
        util::istringbuf in_ss_ref(str);

        ifile.seek(0);
        for (int i = 0, perc0 = -1; i < iter_count; ++i) {
            int perc = 500 + (500 * static_cast<int64_t>(i)) / iter_count;
            if (perc > perc0) {
                util::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
                perc0 = perc;
            }

            VERIFY(ifile.tell() == in_ss_ref.tell());
            unsigned sz = distribution(generator) % 128;
            unsigned pos = distribution(generator) % std::max(0u, static_cast<unsigned>(str.size() - sz));

            ifile.seek(pos);
            in_ss_ref.seek(pos);
            VERIFY(ifile.tell() == in_ss_ref.tell());

            char buf1[256], buf2[256];
            size_t n_read = ifile.read(util::as_span(buf1, sz));
            VERIFY(n_read == in_ss_ref.read(util::as_span(buf2, sz)));
            VERIFY(std::equal(buf1, buf1 + n_read, buf2));
            VERIFY(ifile.peek() == in_ss_ref.peek());
            ifile.clear();
            in_ss_ref.clear();
        }
    }

    return 0;
}

}  // namespace

ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_crlf);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_dev_sequential);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_dev_sequential_str);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_dev_sequential_block);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_dev_sequential_block_str);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_dev_random_block);
