#include "test_suite.h"

#include "uxs/crc32.h"
#include "uxs/io/byteseqdev.h"
#include "uxs/io/filebuf.h"
#include "uxs/io/ibuf_iterator.h"
#include "uxs/io/iflatbuf.h"
#include "uxs/io/obuf_iterator.h"
#include "uxs/io/oflatbuf.h"
#include "uxs/io/ziparch.h"
#include "uxs/io/zipfilebuf.h"
#include "uxs/string_alg.h"

#include <random>
#include <sstream>
#include <vector>

extern std::string g_testdata_path;

namespace {

class memdev : public uxs::iodevice {
 public:
    explicit memdev(uxs::iodevcaps caps, bool is_wide = false) : uxs::iodevice(caps), is_wide_(is_wide) {}
    memdev(const memdev& other)
        : uxs::iodevice(other.caps()), is_wide_(other.is_wide_),
          data_(other.data_.data(), other.data_.data() + to_word_count(other.top_)), top_(other.top_) {}
    template<typename CharT>
    est::span<const CharT> data() const {
        return est::as_span(reinterpret_cast<const CharT*>(data_.data()), top_ / sizeof(CharT));
    }

    int read(void* data, size_t sz, size_t& n_read) override {
        assert(pos_ <= top_ && top_ <= to_byte_count(data_.size()));
        n_read = std::min(sz, top_ - pos_);
        std::copy_n(reinterpret_cast<const uint8_t*>(data_.data()) + pos_, n_read, static_cast<uint8_t*>(data));
        pos_ += n_read;
        return 0;
    }

    int write(const void* data, size_t sz, size_t& n_written) override {
        assert(pos_ <= top_ && top_ <= to_byte_count(data_.size()));
        if (pos_ + sz > top_) { data_.resize(to_word_count(pos_ + sz)); }
        std::copy_n(static_cast<const uint8_t*>(data), sz, reinterpret_cast<uint8_t*>(data_.data()) + pos_);
        n_written = sz;
        pos_ += n_written, top_ = std::max(top_, pos_);
        return 0;
    }

    void* map(size_t& sz, bool wr) override {
        assert(pos_ <= top_ && top_ <= to_byte_count(data_.size()));
        if (wr) {
            if (to_byte_count(data_.size()) == pos_) { data_.resize(std::max<size_t>(8, (3 * data_.size()) >> 1)); }
            sz = to_byte_count(data_.size()) - pos_;
        } else {
            sz = top_ - pos_;
        }
        return reinterpret_cast<uint8_t*>(data_.data()) + pos_;
    }

    void advance(size_t n) override {
        assert(pos_ <= top_ && top_ <= to_byte_count(data_.size()));
        pos_ += n;
        assert(pos_ <= to_byte_count(data_.size()));
        top_ = std::max(top_, pos_);
    }

    int64_t seek(int64_t off, uxs::seekdir dir) override {
        assert(pos_ <= top_ && top_ <= to_byte_count(data_.size()));
        switch (dir) {
            case uxs::seekdir::beg: {
                VERIFY(off >= 0);
                pos_ = static_cast<size_t>(off);
            } break;
            case uxs::seekdir::curr: {
                VERIFY(off >= 0 || static_cast<size_t>(-off) <= pos_);
                pos_ += static_cast<size_t>(off);
            } break;
            case uxs::seekdir::end: {
                VERIFY(off >= 0 || static_cast<size_t>(-off) <= top_);
                pos_ = top_ + static_cast<size_t>(off);
            } break;
        }
        top_ = std::max(top_, pos_);
        if (top_ > to_byte_count(data_.size())) { data_.resize(to_word_count(top_)); }
        return pos_;
    }

    int ctrlesc_color(est::span<const uint8_t> v) override {
        if (is_wide_) {
            uxs::inline_wdynbuffer buf;
            buf += L"\033[";
            uxs::join_basic_strings(buf, v, ';', [](uxs::wmembuffer& s, uint8_t x) -> uxs::wmembuffer& {
                return uxs::to_basic_string(s, x);
            });
            buf += 'm';
            size_t n_written = 0;
            return write(buf.data(), buf.size() * sizeof(wchar_t), n_written) == 0 &&
                           n_written == buf.size() * sizeof(wchar_t) ?
                       0 :
                       -1;
        }
        uxs::inline_dynbuffer buf;
        buf += "\033[";
        uxs::join_basic_strings(
            buf, v, ';', [](uxs::membuffer& s, uint8_t x) -> uxs::membuffer& { return uxs::to_basic_string(s, x); });
        buf += 'm';
        size_t n_written = 0;
        return write(buf.data(), buf.size(), n_written) == 0 && n_written == buf.size() ? 0 : -1;
    }

    int truncate() override {
        top_ = pos_;
        data_.resize(to_word_count(top_));
        return 0;
    }

    int flush() override { return 0; }

 private:
    bool is_wide_ = false;
    std::vector<std::max_align_t> data_;
    size_t pos_ = 0, top_ = 0;

    static size_t to_word_count(size_t sz) { return (sz + sizeof(std::max_align_t) - 1) / sizeof(std::max_align_t); }
    static size_t to_byte_count(size_t sz) { return sz * sizeof(std::max_align_t); }
};

const int brute_N = 10000000;

template<typename CharT>
int test_iobuf_basics(uxs::iodevcaps caps, bool use_z_compression) {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    uxs::print("      \b\b\b\b\b\b").flush();

    uxs::basic_oflatbuf<CharT> ss;
    std::basic_ostringstream<CharT> ss_ref;
    while (ss_ref.tellp() < brute_N) {
        VERIFY(ss.tell() == static_cast<typename uxs::basic_iobuf<CharT>::pos_type>(ss_ref.tellp()));
        for (unsigned n = distribution(generator); n > 0; --n) {
            unsigned k = distribution(generator);
            if (k < 5) {
                std::string_view esc("\033[0;35m");
                ss.write(esc.begin(), esc.end());
                ss_ref << "\033[0;35m";
            } else {
                CharT ch = ' ' + static_cast<CharT>((k - 5) % ('z' - ' '));
                ss.put(ch);
                ss_ref.put(ch);
            }
        }
        for (unsigned n = 1 + distribution(generator) % 3; n > 0; --n) {
            ss.put('\n');
            ss_ref.put('\n');
        }
    }
    ss.flush();
    ss_ref.flush();

    std::basic_string<CharT> str(ss.data(), ss.size());
    VERIFY(str == ss_ref.str());

    memdev middev(caps, !std::is_same<CharT, char>::value);

    {
        uxs::basic_iflatbuf<CharT> ifile(str);
        uxs::basic_devbuf<CharT> middle(middev, uxs::iomode::out | uxs::iomode::cr_lf |
                                                    (use_z_compression ? uxs::iomode::z_compr : uxs::iomode::ctrl_esc));

        uxs::basic_ibuf_iterator<CharT> in(ifile), in_end{};
        uxs::basic_obuf_iterator<CharT> out(middle);
        std::copy(in, in_end, out);
    }

    if (use_z_compression) {
        uxs::basic_oflatbuf<CharT> ss2;
        memdev idev(middev);
        uxs::basic_devbuf<CharT> ifile(idev, uxs::iomode::in | uxs::iomode::z_compr);
        uxs::basic_ibuf_iterator<CharT> in(ifile), in_end{};
        uxs::basic_obuf_iterator<CharT> out(ss2);
        std::copy(in, in_end, out);

        auto data = ss2.view();
        VERIFY(data.size() > str.size());
        for (unsigned n = 1; n < data.size(); ++n) { VERIFY(data[n] != '\n' || data[n - 1] == '\r'); }
    } else {
        auto data = middev.data<CharT>();
        VERIFY(data.size() > str.size());
        for (unsigned n = 1; n < data.size(); ++n) { VERIFY(data[n] != '\n' || data[n - 1] == '\r'); }
    }

    uxs::basic_oflatbuf<CharT> ss2;

    {
        memdev idev(middev);
        uxs::basic_devbuf<CharT> ifile(idev, uxs::iomode::in | uxs::iomode::cr_lf |
                                                 (use_z_compression ? uxs::iomode::z_compr : uxs::iomode::none));

        uxs::basic_ibuf_iterator<CharT> in(ifile), in_end{};
        uxs::basic_obuf_iterator<CharT> out(ss2);
        std::copy(in, in_end, out);
        ss2.flush();
    }

    VERIFY(str == std::basic_string_view<CharT>(ss2.data(), ss2.size()));
    return 0;
}

template<typename CharT>
std::basic_string<CharT> make_string(const memdev& dev) {
    auto data = dev.template data<CharT>();
    return std::basic_string<CharT>(data.begin(), data.end());
}

template<typename CharT>
std::basic_string<CharT> make_string(const uxs::byteseqdev& dev) {
    auto data = dev.get()->make_vector();
    return std::basic_string<CharT>(reinterpret_cast<const CharT*>(data.data()), data.size() / sizeof(CharT));
}

template<typename CharT, typename MemDevT = memdev, typename... Args>
int test_iobuf_dev_sequential(Args&&... args) {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    uxs::print("      \b\b\b\b\b\b").flush();

    MemDevT dev(std::forward<Args>(args)...);
    std::basic_ostringstream<CharT> ss_ref;

    {
        uxs::basic_devbuf<CharT> ofile(dev, uxs::iomode::out);
        uxs::basic_obuf_iterator<CharT> out(ofile);
        std::ostreambuf_iterator<CharT> out_ref(ss_ref);

        while (ss_ref.tellp() < brute_N) {
            VERIFY(ofile.tell() == static_cast<typename uxs::basic_iobuf<CharT>::pos_type>(ss_ref.tellp()));
            CharT ch = ' ' + static_cast<CharT>(distribution(generator) % ('z' - ' '));
            *out = ch;
            *out_ref = ch;
        }
        ss_ref.flush();
    }

    std::basic_string<CharT> str = make_string<CharT>(dev);
    VERIFY(str.size() == brute_N);
    VERIFY(str == ss_ref.str());

    {
        uxs::basic_devbuf<CharT> ifile(dev, uxs::iomode::in);
        std::basic_istringstream<CharT> in_ss_ref(str);
        ifile.seek(0);

        uxs::basic_ibuf_iterator<CharT> in(ifile), in_end{};
        std::istreambuf_iterator<CharT> in_ref(in_ss_ref);
        VERIFY(in != in_end);
        do {
            VERIFY(ifile.tell() == static_cast<typename uxs::basic_iobuf<CharT>::pos_type>(in_ss_ref.tellg()));
            VERIFY(*in == *in_ref);
            ++in, ++in_ref;
        } while (in != in_end);
        VERIFY(ifile.peek() == uxs::basic_iobuf<CharT>::traits_type::eof());
        VERIFY(in_ss_ref.peek() == std::basic_istringstream<CharT>::traits_type::eof());
    }

    return 0;
}

template<typename CharT>
int test_iobuf_str_sequential() {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    uxs::print("      \b\b\b\b\b\b").flush();

    uxs::basic_oflatbuf<CharT> ofile;
    std::basic_ostringstream<CharT> ss_ref;

    {
        uxs::basic_obuf_iterator<CharT> out(ofile);
        std::ostreambuf_iterator<CharT> out_ref(ss_ref);

        while (ss_ref.tellp() < brute_N) {
            VERIFY(ofile.tell() == static_cast<typename uxs::basic_iobuf<CharT>::pos_type>(ss_ref.tellp()));
            CharT ch = ' ' + static_cast<CharT>(distribution(generator) % ('z' - ' '));
            *out = ch;
            *out_ref = ch;
        }
        ofile.flush();
        ss_ref.flush();
    }

    std::basic_string<CharT> str(ofile.data(), ofile.size());
    VERIFY(str.size() == brute_N);
    VERIFY(str == ss_ref.str());

    {
        uxs::basic_iflatbuf<CharT> ifile(str);
        std::basic_istringstream<CharT> in_ss_ref(str);

        uxs::basic_ibuf_iterator<CharT> in(ifile), in_end{};
        std::istreambuf_iterator<CharT> in_ref(in_ss_ref);
        VERIFY(in != in_end);
        do {
            VERIFY(ifile.tell() == static_cast<typename uxs::basic_iobuf<CharT>::pos_type>(in_ss_ref.tellg()));
            VERIFY(*in == *in_ref);
            ++in, ++in_ref;
        } while (in != in_end);
        VERIFY(ifile.peek() == uxs::basic_iobuf<CharT>::traits_type::eof());
        VERIFY(in_ss_ref.peek() == std::basic_istringstream<CharT>::traits_type::eof());
    }

    return 0;
}

template<typename CharT, typename MemDevT = memdev, typename... Args>
int test_iobuf_dev_sequential_block(Args&&... args) {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    uxs::print("      \b\b\b\b\b\b").flush();

    MemDevT dev(std::forward<Args>(args)...);
    std::basic_ostringstream<CharT> ss_ref;

    {
        uxs::basic_devbuf<CharT> ofile(dev, uxs::iomode::out);

        while (ss_ref.tellp() < brute_N) {
            VERIFY(ofile.tell() == static_cast<typename uxs::basic_iobuf<CharT>::pos_type>(ss_ref.tellp()));
            unsigned sz = distribution(generator);
            CharT buf[256];
            for (unsigned n = 0; n < sz; ++n) {
                buf[n] = ' ' + static_cast<CharT>(distribution(generator) % ('z' - ' '));
            }
            ofile.write(est::as_span(buf, sz));
            ss_ref.write(buf, sz);
        }
        ss_ref.flush();
    }

    std::basic_string<CharT> str = make_string<CharT>(dev);
    VERIFY(str.size() >= brute_N);
    VERIFY(str == ss_ref.str());

    {
        uxs::basic_devbuf<CharT> ifile(dev, uxs::iomode::in);
        std::basic_istringstream<CharT> in_ss_ref(str);
        ifile.seek(0);

        while (ifile) {
            VERIFY(ifile.tell() == static_cast<typename uxs::basic_iobuf<CharT>::pos_type>(in_ss_ref.tellg()));
            unsigned sz = distribution(generator);
            CharT buf1[256], buf2[256];
            size_t n_read = ifile.read(est::as_span(buf1, sz));
            in_ss_ref.read(buf2, sz);
            VERIFY(static_cast<std::streamsize>(n_read) == in_ss_ref.gcount());
            VERIFY(std::equal(buf1, buf1 + n_read, buf2));
        }
        VERIFY(ifile.peek() == uxs::basic_iobuf<CharT>::traits_type::eof());
        VERIFY(in_ss_ref.peek() == std::basic_istringstream<CharT>::traits_type::eof());
    }

    return 0;
}

template<typename CharT>
int test_iobuf_str_sequential_block() {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    uxs::print("      \b\b\b\b\b\b").flush();

    uxs::basic_oflatbuf<CharT> ofile;
    std::basic_ostringstream<CharT> ss_ref;

    {
        while (ss_ref.tellp() < brute_N) {
            VERIFY(ofile.tell() == static_cast<typename uxs::basic_iobuf<CharT>::pos_type>(ss_ref.tellp()));
            unsigned sz = distribution(generator);
            CharT buf[256];
            for (unsigned n = 0; n < sz; ++n) {
                buf[n] = ' ' + static_cast<CharT>(distribution(generator) % ('z' - ' '));
            }
            ofile.write(est::as_span(buf, sz));
            ss_ref.write(buf, sz);
        }
        ofile.flush();
        ss_ref.flush();
    }

    std::basic_string<CharT> str(ofile.data(), ofile.size());
    VERIFY(str.size() >= brute_N);
    VERIFY(str == ss_ref.str());

    {
        uxs::basic_iflatbuf<CharT> ifile(str);
        std::basic_istringstream<CharT> in_ss_ref(str);

        while (ifile) {
            VERIFY(ifile.tell() == static_cast<typename uxs::basic_iobuf<CharT>::pos_type>(in_ss_ref.tellg()));
            unsigned sz = distribution(generator);
            CharT buf1[256], buf2[256];
            size_t n_read = ifile.read(est::as_span(buf1, sz));
            in_ss_ref.read(buf2, sz);
            VERIFY(static_cast<std::streamsize>(n_read) == in_ss_ref.gcount());
            VERIFY(std::equal(buf1, buf1 + n_read, buf2));
        }
        VERIFY(ifile.peek() == uxs::basic_iobuf<CharT>::traits_type::eof());
        VERIFY(in_ss_ref.peek() == std::basic_istringstream<CharT>::traits_type::eof());
    }

    return 0;
}

template<typename CharT, typename MemDevT = memdev, typename... Args>
int test_iobuf_dev_random_block(Args&&... args) {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 1000000000);

    MemDevT dev(std::forward<Args>(args)...);
    uxs::basic_oflatbuf<CharT> ss_ref;

    int iter_count = brute_N;

    {
        uxs::basic_devbuf<CharT> ofile(dev, uxs::iomode::out);

        for (int i = 0, perc0 = -1; i < iter_count; ++i) {
            int perc = (500 * static_cast<int64_t>(i)) / iter_count;
            if (perc > perc0) {
                uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
                perc0 = perc;
            }

            VERIFY(ofile.tell() == ss_ref.tell());
            unsigned sz = distribution(generator) % 128;
            unsigned tot_size = static_cast<unsigned>(ss_ref.seek(0, uxs::seekdir::end));
            unsigned pos = distribution(generator) % (tot_size + std::max<unsigned>(tot_size / 1000, 16));
            CharT buf[256];
            for (unsigned n = 0; n < sz; ++n) {
                buf[n] = ' ' + static_cast<CharT>(distribution(generator) % ('z' - ' '));
            }

            ofile.seek(pos);
            ss_ref.seek(pos);
            VERIFY(ofile.tell() == ss_ref.tell());

            ofile.write(est::as_span(buf, sz));
            ss_ref.write(est::as_span(buf, sz));

            if (distribution(generator) % 1000 == 0) {
                ofile.truncate();
                ss_ref.truncate();
            }
        }
        ss_ref.flush();
    }

    std::basic_string<CharT> str = make_string<CharT>(dev);
    VERIFY(str == std::basic_string_view<CharT>(ss_ref.data(), ss_ref.size()));

    {
        uxs::basic_devbuf<CharT> ifile(dev, uxs::iomode::in);
        uxs::basic_iflatbuf<CharT> in_ss_ref(str);

        ifile.seek(0);
        for (int i = 0, perc0 = -1; i < iter_count; ++i) {
            int perc = 500 + (500 * static_cast<int64_t>(i)) / iter_count;
            if (perc > perc0) {
                uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
                perc0 = perc;
            }

            VERIFY(ifile.tell() == in_ss_ref.tell());
            unsigned sz = distribution(generator) % 128;
            unsigned pos = distribution(generator) % (str.size() > sz ? static_cast<unsigned>(str.size() - sz) : 0u);

            ifile.seek(pos);
            in_ss_ref.seek(pos);
            VERIFY(ifile.tell() == in_ss_ref.tell());

            CharT buf1[256], buf2[256];
            size_t n_read = ifile.read(est::as_span(buf1, sz));
            VERIFY(n_read == in_ss_ref.read(est::as_span(buf2, sz)));
            VERIFY(std::equal(buf1, buf1 + n_read, buf2));
            VERIFY(ifile.peek() == in_ss_ref.peek());
            ifile.clear();
            in_ss_ref.clear();
        }
    }

    return 0;
}

void test_iobuf_file_mode(uxs::iomode mode, std::string_view what_to_write, bool can_create_new,
                          bool can_open_when_existing, std::string_view what_to_write_when_existing,
                          std::string_view what_to_read_when_existing) {
    std::string fname = g_testdata_path + "test_file.txt";

    uxs::sysfile::remove(fname.c_str());

    if (!(mode & uxs::iomode::out)) {
        {
            uxs::filebuf ifile(fname.c_str(), mode);
            VERIFY(!ifile);
        }

        VERIFY(mode & uxs::iomode::in);
        uxs::filebuf ofile(fname.c_str(), "w");
        VERIFY(ofile);
        ofile.write(what_to_write);
        ofile.close();

        {
            uxs::filebuf ifile(fname.c_str(), mode);
            VERIFY(ifile);

            std::vector<char> data;
            data.resize(what_to_write.size());
            VERIFY(ifile.read(data) == what_to_write.size());
            VERIFY(std::equal(data.begin(), data.end(), what_to_write.data()));
        }

        uxs::sysfile::remove(fname.c_str());
        return;
    }

    {  // try create non-existing file
        uxs::filebuf ofile(fname.c_str(), mode);
        VERIFY(!!ofile == can_create_new);

        if (ofile) {
            ofile.write(what_to_write);
            ofile.close();

            uxs::filebuf ifile(fname.c_str(), "r");
            VERIFY(ifile);

            std::vector<char> data;
            data.resize(what_to_write.size());
            VERIFY(ifile.read(data) == what_to_write.size());
            VERIFY(std::equal(data.begin(), data.end(), what_to_write.data()));
        } else {
            VERIFY(ofile.open(fname.c_str(), "w"));
            ofile.write(what_to_write);
        }
    }

    {  // try to work with existing file
        uxs::filebuf ofile(fname.c_str(), mode);
        VERIFY(!!ofile == can_open_when_existing);

        if (ofile) {
            ofile.write(what_to_write_when_existing);
            ofile.close();

            uxs::filebuf ifile(fname.c_str(), "r");
            VERIFY(ifile);

            std::vector<char> data;
            data.resize(what_to_read_when_existing.size());
            VERIFY(ifile.read(data) == what_to_read_when_existing.size());
            VERIFY(std::equal(data.begin(), data.end(), what_to_read_when_existing.data()));
        }
    }

    uxs::sysfile::remove(fname.c_str());
}

void test_iobuf_file_mode(const char* mode, std::string_view what_to_write, bool can_create_new,
                          bool can_open_when_existing, std::string_view what_to_write_when_existing,
                          std::string_view what_to_read_when_existing) {
    test_iobuf_file_mode(uxs::detail::iomode_from_str(mode, uxs::iomode::none), what_to_write, can_create_new,
                         can_open_when_existing, what_to_write_when_existing, what_to_read_when_existing);
}

int test_iobuf_file_modes() {
    test_iobuf_file_mode(uxs::iomode::in, "Hello, world", false, false, "", "");

    test_iobuf_file_mode(uxs::iomode::out, "Hello, world", false, true, "HELLO", "HELLO, world");
    test_iobuf_file_mode(uxs::iomode::out | uxs::iomode::truncate, "Hello, world", false, true, "HELLO", "HELLO");
    test_iobuf_file_mode(uxs::iomode::out | uxs::iomode::truncate | uxs::iomode::append, "Hello, world", false, true,
                         "HELLO", "HELLO");
    test_iobuf_file_mode(uxs::iomode::out | uxs::iomode::append, "Hello, world", false, true, "HELLO",
                         "Hello, worldHELLO");

    test_iobuf_file_mode(uxs::iomode::out | uxs::iomode::exclusive, "Hello, world", false, true, "HELLO",
                         "HELLO, world");
    test_iobuf_file_mode(uxs::iomode::out | uxs::iomode::exclusive | uxs::iomode::truncate, "Hello, world", false, true,
                         "HELLO", "HELLO");
    test_iobuf_file_mode(uxs::iomode::out | uxs::iomode::exclusive | uxs::iomode::truncate | uxs::iomode::append,
                         "Hello, world", false, true, "HELLO", "HELLO");
    test_iobuf_file_mode(uxs::iomode::out | uxs::iomode::exclusive | uxs::iomode::append, "Hello, world", false, true,
                         "HELLO", "Hello, worldHELLO");

    test_iobuf_file_mode(uxs::iomode::out | uxs::iomode::create, "Hello, world", true, true, "HELLO", "HELLO, world");
    test_iobuf_file_mode(uxs::iomode::out | uxs::iomode::create | uxs::iomode::truncate, "Hello, world", true, true,
                         "HELLO", "HELLO");
    test_iobuf_file_mode(uxs::iomode::out | uxs::iomode::create | uxs::iomode::truncate | uxs::iomode::append,
                         "Hello, world", true, true, "HELLO", "HELLO");
    test_iobuf_file_mode(uxs::iomode::out | uxs::iomode::create | uxs::iomode::append, "Hello, world", true, true,
                         "HELLO", "Hello, worldHELLO");

    test_iobuf_file_mode(uxs::iomode::out | uxs::iomode::create | uxs::iomode::exclusive, "Hello, world", true, false,
                         "", "");
    test_iobuf_file_mode(uxs::iomode::out | uxs::iomode::create | uxs::iomode::exclusive | uxs::iomode::truncate,
                         "Hello, world", true, false, "", "");
    test_iobuf_file_mode(
        uxs::iomode::out | uxs::iomode::create | uxs::iomode::exclusive | uxs::iomode::truncate | uxs::iomode::append,
        "Hello, world", true, false, "", "");
    test_iobuf_file_mode(uxs::iomode::out | uxs::iomode::create | uxs::iomode::exclusive | uxs::iomode::append,
                         "Hello, world", true, false, "", "");

    test_iobuf_file_mode("r", "Hello, world", false, false, "", "");
    test_iobuf_file_mode("r+", "Hello, world", false, true, "HELLO", "HELLO, world");
    test_iobuf_file_mode("w", "Hello, world", true, true, "HELLO", "HELLO");
    test_iobuf_file_mode("w+", "Hello, world", true, true, "HELLO", "HELLO");
    test_iobuf_file_mode("a", "Hello, world", true, true, "HELLO", "Hello, worldHELLO");
    test_iobuf_file_mode("a+", "Hello, world", true, true, "HELLO", "Hello, worldHELLO");
    test_iobuf_file_mode("wx", "Hello, world", true, false, "", "");
    test_iobuf_file_mode("w+x", "Hello, world", true, false, "", "");
    test_iobuf_file_mode("ax", "Hello, world", true, false, "", "");
    test_iobuf_file_mode("a+x", "Hello, world", true, false, "", "");

    return 0;
}

int test_iobuf_file_sharing() {
    std::string fname = g_testdata_path + "test_file.txt";

    uxs::sysfile::remove(fname.c_str());

#ifdef WIN32
    {
        uxs::filebuf ofile1(fname.c_str(), "w");
        VERIFY(ofile1);

        uxs::filebuf ofile2(fname.c_str(), "w");
        VERIFY(!ofile2);
    }
#endif

    {
        uxs::filebuf ofile(fname.c_str(), "w");
        VERIFY(ofile);
        ofile.write(std::string_view{"Hello"}).flush();

        uxs::filebuf ifile1(fname.c_str(), "r");
        VERIFY(ifile1);
        {
            std::string s;
            s.resize(5);
            ifile1.read(est::as_span(&s[0], 5));
            VERIFY(s == "Hello");
        }

        uxs::filebuf ifile2(fname.c_str(), "r");
        VERIFY(ifile2);
        {
            std::string s;
            s.resize(5);
            ifile2.read(est::as_span(&s[0], 5));
            VERIFY(s == "Hello");
        }
    }

    {
        uxs::filebuf ifile(fname.c_str(), "r");
        VERIFY(ifile);

        uxs::filebuf ofile(fname.c_str(), "w");
        VERIFY(ofile);
        ofile.write(std::string_view{"Hello"}).flush();

        {
            std::string s;
            s.resize(5);
            ifile.read(est::as_span(&s[0], 5));
            VERIFY(s == "Hello");
        }
    }

    uxs::sysfile::remove(fname.c_str());
    return 0;
}

int test_iobuf_file_text_mode() {
    std::string fname = g_testdata_path + "test_file.txt";
    std::string_view txt = "hello\n\nhello\n";
    std::string_view crlf_txt = "hello\r\n\r\nhello\r\n";

    uxs::sysfile::remove(fname.c_str());

    uxs::filebuf ofile(fname.c_str(),
                       uxs::iomode::out | uxs::iomode::create | uxs::iomode::truncate | uxs::iomode::cr_lf);
    VERIFY(ofile);
    ofile.write(txt);
    ofile.close();

    {
        uxs::filebuf ifile(fname.c_str(), uxs::iomode::in);
        VERIFY(ifile);

        std::vector<char> data;
        data.resize(crlf_txt.size());
        VERIFY(ifile.read(data) == crlf_txt.size());
        VERIFY(std::equal(data.begin(), data.end(), crlf_txt.data()));
    }

    {
        uxs::filebuf ifile(fname.c_str(), uxs::iomode::in | uxs::iomode::cr_lf);
        VERIFY(ifile);

        std::vector<char> data;
        data.resize(txt.size());
        VERIFY(ifile.read(data) == txt.size());
        VERIFY(std::equal(data.begin(), data.end(), txt.data()));
    }

    uxs::sysfile::remove(fname.c_str());
    return 0;
}

int test_iobuf_zlib() {
    uxs::sysfile::remove((g_testdata_path + "zlib/test-1.bin").c_str());
    uxs::sysfile::remove((g_testdata_path + "zlib/test-2.bin").c_str());

    {
        uxs::sysfile ifile((g_testdata_path + "zlib/test.bin").c_str(), "r");
        uxs::u8filebuf ofile((g_testdata_path + "zlib/test-1.bin").c_str(), "wz");
        VERIFY(ifile && ofile);
        size_t n_read = 0;
        do {
            ofile.reserve();
            if (ifile.read(ofile.curr(), ofile.avail(), n_read) < 0) { return -1; }
            ofile.advance(n_read);
        } while (n_read);
    }

    {
        uxs::u8filebuf ifile((g_testdata_path + "zlib/test-1.bin").c_str(), "rz");
        uxs::sysfile ofile((g_testdata_path + "zlib/test-2.bin").c_str(), "w");
        VERIFY(ifile && ofile);
        size_t n_written = 0;
        while (ifile.peek() != uxs::u8iobuf::traits_type::eof()) {
            if (ofile.write(ifile.curr(), ifile.avail(), n_written) < 0) { return -1; }
            ifile.advance(n_written);
        }
    }

    {
        uxs::u8filebuf ifile1((g_testdata_path + "zlib/test.bin").c_str(), "r");
        uxs::u8filebuf ifile2((g_testdata_path + "zlib/test-2.bin").c_str(), "r");
        VERIFY(ifile1 && ifile2);
        uxs::u8ibuf_iterator in1(ifile1), in2(ifile2), in_end{};
        VERIFY(std::equal(in1, in_end, in2));
        VERIFY(ifile2.peek() == uxs::u8iobuf::traits_type::eof());
    }

    uxs::sysfile::remove((g_testdata_path + "zlib/test-1.bin").c_str());
    uxs::sysfile::remove((g_testdata_path + "zlib/test-2.bin").c_str());
    return 0;
}

template<typename MemDevT = memdev, typename... Args>
int test_iobuf_zlib_buf(Args&&... args) {
    uxs::sysfile::remove((g_testdata_path + "zlib/test-2.bin").c_str());

    MemDevT middev(std::forward<Args>(args)...);

    {
        uxs::sysfile ifile((g_testdata_path + "zlib/test.bin").c_str(), "r");
        uxs::u8devbuf ofile(middev, uxs::iomode::out | uxs::iomode::z_compr);
        VERIFY(ifile && ofile);
        size_t n_read = 0;
        do {
            ofile.reserve();
            if (ifile.read(ofile.curr(), ofile.avail(), n_read) < 0) { return -1; }
            ofile.advance(n_read);
        } while (n_read);
    }

    middev.seek(0, uxs::seekdir::beg);

    {
        uxs::u8devbuf ifile(middev, uxs::iomode::in | uxs::iomode::z_compr);
        uxs::sysfile ofile((g_testdata_path + "zlib/test-2.bin").c_str(), "w");
        VERIFY(ifile && ofile);
        size_t n_written = 0;
        while (ifile.peek() != uxs::u8iobuf::traits_type::eof()) {
            if (ofile.write(ifile.curr(), ifile.avail(), n_written) < 0) { return -1; }
            ifile.advance(n_written);
        }
    }

    {
        uxs::u8filebuf ifile1((g_testdata_path + "zlib/test.bin").c_str(), "r");
        uxs::u8filebuf ifile2((g_testdata_path + "zlib/test-2.bin").c_str(), "r");
        VERIFY(ifile1 && ifile2);
        uxs::u8ibuf_iterator in1(ifile1), in2(ifile2), in_end{};
        VERIFY(std::equal(in1, in_end, in2));
        VERIFY(ifile2.peek() == uxs::u8iobuf::traits_type::eof());
    }

    uxs::sysfile::remove((g_testdata_path + "zlib/test-2.bin").c_str());
    return 0;
}

int test_iobuf_libzip() {
    static std::string fnames[] = {"file1", "file2", "file3"};

    auto check = [] {
        uxs::ziparch arch((g_testdata_path + "libzip/arch.zip").c_str(), "r");

        for (const auto& fname : est::as_span(fnames)) {
            uxs::u8filebuf ifile1((g_testdata_path + "libzip/" + fname).c_str(), "r");
            VERIFY(ifile1);

            uxs::u8zipfilebuf ifile2(arch, fname.c_str(), "r");
            VERIFY(ifile2);

            uxs::u8ibuf_iterator it1{ifile1}, it2{ifile2}, it_end{};
            for (; it1 != it_end && it2 != it_end; ++it1, ++it2) { VERIFY(*it1 == *it2); }
            VERIFY(it1 == it_end && it2 == it_end);
        }
    };

    uxs::sysfile::remove((g_testdata_path + "libzip/arch.zip").c_str());

    {
        uxs::ziparch arch((g_testdata_path + "libzip/arch.zip").c_str(), "w");

        for (const auto& fname : est::as_span(fnames)) {
            uxs::u8filebuf ifile((g_testdata_path + "libzip/" + fname).c_str(), "r");
            VERIFY(ifile);

            uxs::zipfile ofile(arch, fname.c_str(), "w");
            VERIFY(ofile);

            while (ifile.peek() != uxs::ibuf::traits_type::eof()) {
                size_t written = 0;
                VERIFY(ofile.write(ifile.curr(), ifile.avail(), written) >= 0 && written == ifile.avail());
                ifile.advance(ifile.avail());
            }
        }

        uxs::zipfile_info info;
        for (uint64_t index = 0; arch.stat_file(index, info); ++index) {
            VERIFY(info.name == fnames[index]);
            VERIFY(info.index == index);
        }
    }

    {
        uxs::ziparch arch((g_testdata_path + "libzip/arch.zip").c_str(), "r");

        uxs::zipfile_info info;
        for (uint64_t index = 0; arch.stat_file(index, info); ++index) {
            uxs::u8filebuf ifile((g_testdata_path + "libzip/" + fnames[index]).c_str(), "r");
            VERIFY(ifile);

            const size_t sz = static_cast<size_t>(ifile.seek(0, uxs::seekdir::end));
            ifile.seek(0);

            std::vector<uint8_t> data;
            data.resize(sz);

            VERIFY(ifile.read(data) == sz);

            const auto crc32 = uxs::crc32_calc{}(data.begin(), data.end());

            VERIFY(info.name == fnames[index]);
            VERIFY(info.index == index);
            VERIFY(info.size == sz);
            VERIFY(info.crc == ~crc32);

            uxs::zipfile_info info_by_name;
            arch.stat_file(fnames[index].c_str(), info_by_name);
            VERIFY(info.name == info_by_name.name);
            VERIFY(info.index == info_by_name.index);
            VERIFY(info.size == info_by_name.size);
            VERIFY(info.crc == info_by_name.crc);
        }
    }

    check();

    {
        uxs::ziparch arch((g_testdata_path + "libzip/arch.zip").c_str(), "w");

        for (const auto& fname : est::as_span(fnames)) {
            uxs::u8filebuf ifile((g_testdata_path + "libzip/" + fname).c_str(), "r");
            VERIFY(ifile);

            const size_t sz = static_cast<size_t>(ifile.seek(0, uxs::seekdir::end));
            ifile.seek(0);

            std::vector<uint8_t> data;
            data.resize(sz);

            VERIFY(ifile.read(data) == sz);
            VERIFY(arch.add_file(fname.c_str(), data.data(), data.size()) >= 0);
        }

        uxs::zipfile_info info;
        for (uint64_t index = 0; arch.stat_file(index, info); ++index) {
            VERIFY(info.name == fnames[index]);
            VERIFY(info.index == index);
        }
    }

    check();

    uxs::sysfile::remove((g_testdata_path + "libzip/arch.zip").c_str());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "iobuf", test_iobuf_file_modes);
ADD_TEST_CASE("", "iobuf", test_iobuf_file_sharing);
ADD_TEST_CASE("", "iobuf", test_iobuf_file_text_mode);

ADD_TEST_CASE("1-bruteforce", "iobuf", []() { return test_iobuf_basics<char>(uxs::iodevcaps::none, false); });
ADD_TEST_CASE("1-bruteforce", "iobuf", []() { return test_iobuf_basics<wchar_t>(uxs::iodevcaps::none, false); });
#if defined(UXS_USE_ZLIB)
ADD_TEST_CASE("1-bruteforce", "zlib", []() { return test_iobuf_basics<char>(uxs::iodevcaps::none, true); });
ADD_TEST_CASE("1-bruteforce", "zlib", []() { return test_iobuf_basics<char>(uxs::iodevcaps::mappable, true); });
ADD_TEST_CASE("1-bruteforce", "zlib", []() { return test_iobuf_basics<wchar_t>(uxs::iodevcaps::none, true); });
ADD_TEST_CASE("1-bruteforce", "zlib", []() { return test_iobuf_basics<wchar_t>(uxs::iodevcaps::mappable, true); });
#endif

ADD_TEST_CASE("1-bruteforce", "iobuf", []() { return test_iobuf_dev_sequential<char>(uxs::iodevcaps::none); });
ADD_TEST_CASE("1-bruteforce", "iobuf", []() { return test_iobuf_dev_sequential<char>(uxs::iodevcaps::mappable); });
ADD_TEST_CASE("1-bruteforce", "iobuf", []() { return test_iobuf_dev_sequential<wchar_t>(uxs::iodevcaps::none); });
ADD_TEST_CASE("1-bruteforce", "iobuf", []() { return test_iobuf_dev_sequential<wchar_t>(uxs::iodevcaps::mappable); });
ADD_TEST_CASE("1-bruteforce", "iobuf", []() {
    uxs::byteseq seq;
    return (test_iobuf_dev_sequential<char, uxs::byteseqdev>(seq));
});
ADD_TEST_CASE("1-bruteforce", "iobuf", []() {
    uxs::byteseq seq;
    return (test_iobuf_dev_sequential<wchar_t, uxs::byteseqdev>(seq));
});

ADD_TEST_CASE("1-bruteforce", "iobuf", []() { return test_iobuf_str_sequential<char>(); });
ADD_TEST_CASE("1-bruteforce", "iobuf", []() { return test_iobuf_str_sequential<wchar_t>(); });

ADD_TEST_CASE("1-bruteforce", "iobuf", []() { return test_iobuf_dev_sequential_block<char>(uxs::iodevcaps::none); });
ADD_TEST_CASE("1-bruteforce", "iobuf", []() { return test_iobuf_dev_sequential_block<char>(uxs::iodevcaps::mappable); });
ADD_TEST_CASE("1-bruteforce", "iobuf", []() { return test_iobuf_dev_sequential_block<wchar_t>(uxs::iodevcaps::none); });
ADD_TEST_CASE("1-bruteforce", "iobuf",
              []() { return test_iobuf_dev_sequential_block<wchar_t>(uxs::iodevcaps::mappable); });
ADD_TEST_CASE("1-bruteforce", "iobuf", []() {
    uxs::byteseq seq;
    return (test_iobuf_dev_sequential_block<char, uxs::byteseqdev>(seq));
});
ADD_TEST_CASE("1-bruteforce", "iobuf", []() {
    uxs::byteseq seq;
    return (test_iobuf_dev_sequential_block<wchar_t, uxs::byteseqdev>(seq));
});

ADD_TEST_CASE("1-bruteforce", "iobuf", []() { return test_iobuf_str_sequential_block<char>(); });
ADD_TEST_CASE("1-bruteforce", "iobuf", []() { return test_iobuf_str_sequential_block<wchar_t>(); });

ADD_TEST_CASE("1-bruteforce", "iobuf", []() { return test_iobuf_dev_random_block<char>(uxs::iodevcaps::none); });
ADD_TEST_CASE("1-bruteforce", "iobuf", []() { return test_iobuf_dev_random_block<char>(uxs::iodevcaps::mappable); });
ADD_TEST_CASE("1-bruteforce", "iobuf", []() { return test_iobuf_dev_random_block<wchar_t>(uxs::iodevcaps::none); });
ADD_TEST_CASE("1-bruteforce", "iobuf", []() { return test_iobuf_dev_random_block<wchar_t>(uxs::iodevcaps::mappable); });
ADD_TEST_CASE("1-bruteforce", "iobuf", []() {
    uxs::byteseq seq;
    return (test_iobuf_dev_random_block<char, uxs::byteseqdev>(seq));
});
ADD_TEST_CASE("1-bruteforce", "iobuf", []() {
    uxs::byteseq seq;
    return (test_iobuf_dev_random_block<wchar_t, uxs::byteseqdev>(seq));
});

#if defined(UXS_USE_ZLIB)
ADD_TEST_CASE("1-bruteforce", "zlib", []() { return test_iobuf_zlib(); });
ADD_TEST_CASE("1-bruteforce", "zlib", []() { return test_iobuf_zlib_buf(uxs::iodevcaps::none); });
ADD_TEST_CASE("1-bruteforce", "zlib", []() { return test_iobuf_zlib_buf(uxs::iodevcaps::mappable); });
ADD_TEST_CASE("1-bruteforce", "zlib", []() {
    uxs::byteseq seq;
    return test_iobuf_zlib_buf<uxs::byteseqdev>(seq);
});
#endif

#if defined(UXS_USE_LIBZIP)
ADD_TEST_CASE("", "libzip", test_iobuf_libzip);
#endif
