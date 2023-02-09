#include "test_suite.h"

#include "uxs/io/filebuf.h"
#include "uxs/io/iobuf_iterator.h"
#include "uxs/io/istringbuf.h"
#include "uxs/io/ostringbuf.h"

#include <random>
#include <sstream>
#include <vector>

extern std::string g_testdata_path;

namespace {

class memdev : public uxs::iodevice {
 public:
    explicit memdev(uxs::iodevcaps caps) : uxs::iodevice(caps) {}
    memdev(std::string_view str, uxs::iodevcaps caps)
        : uxs::iodevice(caps), data_(str.begin(), str.end()), top_(str.size()) {}
    std::string_view str() const {
        return std::string_view(reinterpret_cast<const char*>(data_.data()), std::max(top_, pos_));
    }

    int read(void* data, size_t sz, size_t& n_read) override {
        n_read = std::min(sz, top_ > pos_ ? top_ - pos_ : 0);
        if (n_read) { std::copy_n(data_.data() + pos_, n_read, reinterpret_cast<uint8_t*>(data)); }
        pos_ += n_read;
        return 0;
    }

    int write(const void* data, size_t sz, size_t& n_written) override {
        const char* p = reinterpret_cast<const char*>(data);
        size_t sz1 = std::min(data_.size() - pos_, sz);
        if (sz1) { std::copy_n(p, sz1, data_.data() + pos_); }
        data_.insert(data_.end(), p + sz1, p + sz);
        n_written = sz;
        pos_ += n_written;
        return 0;
    }

    void* map(size_t& sz, bool wr) override {
        if (wr && pos_ == data_.size()) { data_.resize(std::max<size_t>(8, (3 * data_.size()) >> 1)); }
        sz = wr ? data_.size() - pos_ : (top_ > pos_ ? top_ - pos_ : 0);
        return data_.data() + pos_;
    }

    int64_t seek(int64_t off, uxs::seekdir dir) override {
        top_ = std::max(top_, pos_);
        switch (dir) {
            case uxs::seekdir::kBeg: {
                VERIFY(off >= 0);
                pos_ = static_cast<size_t>(off);
            } break;
            case uxs::seekdir::kCurr: {
                VERIFY(off >= 0 || static_cast<size_t>(-off) <= pos_);
                pos_ += static_cast<size_t>(off);
            } break;
            case uxs::seekdir::kEnd: {
                VERIFY(off >= 0 || static_cast<size_t>(-off) <= top_);
                pos_ = top_ + static_cast<size_t>(off);
            } break;
        }
        if (pos_ > top_) { data_.resize(pos_); }
        return pos_;
    }

    int ctrlesc_color(uxs::span<uint8_t> v) override { return -1; }
    int flush() override { return 0; }

 private:
    std::vector<uint8_t> data_;
    size_t pos_ = 0, top_ = 0;
};

const int brute_N = 10000000;

int test_iobuf_crlf(uxs::iodevcaps caps) {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    uxs::stdbuf::out.write("      \b\b\b\b\b\b").flush();

    uxs::ostringbuf ss;
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

    memdev middev(caps);

    {
        uxs::istringbuf ifile(str);
        uxs::devbuf middle(middev, uxs::iomode::kOut | uxs::iomode::kCrLf);

        uxs::ibuf_iterator in(ifile), in_end{};
        uxs::obuf_iterator out(middle);
        std::copy(in, in_end, out);
    }

    VERIFY(middev.str().size() > str.size());
    for (unsigned n = 1; n < middev.str().size(); ++n) {
        VERIFY(middev.str()[n] != '\n' || middev.str()[n - 1] == '\r');
    }

    uxs::ostringbuf ss2;

    {
        memdev idev(middev.str(), caps);
        uxs::devbuf ifile(idev, uxs::iomode::kIn | uxs::iomode::kCrLf);

        uxs::ibuf_iterator in(ifile), in_end{};
        uxs::obuf_iterator out(ss2);
        std::copy(in, in_end, out);
    }

    VERIFY(str == ss2.str());
    return 0;
}

int test_iobuf_crlf_not_mappable() { return test_iobuf_crlf(uxs::iodevcaps::kNone); }
int test_iobuf_crlf_mappable() { return test_iobuf_crlf(uxs::iodevcaps::kMappable); }

int test_iobuf_dev_sequential(uxs::iodevcaps caps) {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    uxs::stdbuf::out.write("      \b\b\b\b\b\b").flush();

    memdev dev(caps);
    std::ostringstream ss_ref;

    {
        uxs::devbuf ofile(dev, uxs::iomode::kOut);
        uxs::obuf_iterator out(ofile);
        std::ostreambuf_iterator<char> out_ref(ss_ref);

        while (ss_ref.tellp() < brute_N) {
            VERIFY(ofile.tell() == ss_ref.tellp());
            char ch = ' ' + static_cast<char>(distribution(generator) % ('z' - ' '));
            *out = ch;
            *out_ref = ch;
        }
        ss_ref.flush();
    }

    std::string str(dev.str());
    VERIFY(str.size() == brute_N);
    VERIFY(str == ss_ref.str());

    {
        uxs::devbuf ifile(dev, uxs::iomode::kIn);
        std::istringstream in_ss_ref(str);
        ifile.seek(0);

        uxs::ibuf_iterator in(ifile), in_end{};
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

int test_iobuf_dev_sequential_not_mappable() { return test_iobuf_dev_sequential(uxs::iodevcaps::kNone); }
int test_iobuf_dev_sequential_mappable() { return test_iobuf_dev_sequential(uxs::iodevcaps::kMappable); }

int test_iobuf_dev_sequential_str() {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    uxs::stdbuf::out.write("      \b\b\b\b\b\b").flush();

    uxs::ostringbuf ofile;
    std::ostringstream ss_ref;

    {
        uxs::obuf_iterator out(ofile);
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
        uxs::istringbuf ifile(str);
        std::istringstream in_ss_ref(str);

        uxs::ibuf_iterator in(ifile), in_end{};
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

int test_iobuf_dev_sequential_block(uxs::iodevcaps caps) {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    uxs::stdbuf::out.write("      \b\b\b\b\b\b").flush();

    memdev dev(caps);
    std::ostringstream ss_ref;

    {
        uxs::devbuf ofile(dev, uxs::iomode::kOut);

        while (ss_ref.tellp() < brute_N) {
            VERIFY(ofile.tell() == ss_ref.tellp());
            unsigned sz = distribution(generator);
            char buf[256];
            for (unsigned n = 0; n < sz; ++n) {
                buf[n] = ' ' + static_cast<char>(distribution(generator) % ('z' - ' '));
            }
            ofile.write(uxs::as_span(buf, sz));
            ss_ref.write(buf, sz);
        }
        ss_ref.flush();
    }

    std::string str(dev.str());
    VERIFY(str.size() >= brute_N);
    VERIFY(str == ss_ref.str());

    {
        uxs::devbuf ifile(dev, uxs::iomode::kIn);
        std::istringstream in_ss_ref(str);
        ifile.seek(0);

        while (ifile) {
            VERIFY(ifile.tell() == in_ss_ref.tellg());
            unsigned sz = distribution(generator);
            char buf1[256], buf2[256];
            size_t n_read = ifile.read(uxs::as_span(buf1, sz));
            in_ss_ref.read(buf2, sz);
            VERIFY(static_cast<std::streamsize>(n_read) == in_ss_ref.gcount());
            VERIFY(std::equal(buf1, buf1 + n_read, buf2));
        }
        VERIFY(ifile.peek() == std::char_traits<char>::eof());
        VERIFY(in_ss_ref.peek() == std::char_traits<char>::eof());
    }

    return 0;
}

int test_iobuf_dev_sequential_block_not_mappable() { return test_iobuf_dev_sequential_block(uxs::iodevcaps::kNone); }
int test_iobuf_dev_sequential_block_mappable() { return test_iobuf_dev_sequential_block(uxs::iodevcaps::kMappable); }

int test_iobuf_dev_sequential_block_str() {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    uxs::stdbuf::out.write("      \b\b\b\b\b\b").flush();

    uxs::ostringbuf ofile;
    std::ostringstream ss_ref;

    {
        while (ss_ref.tellp() < brute_N) {
            VERIFY(ofile.tell() == ss_ref.tellp());
            unsigned sz = distribution(generator);
            char buf[256];
            for (unsigned n = 0; n < sz; ++n) {
                buf[n] = ' ' + static_cast<char>(distribution(generator) % ('z' - ' '));
            }
            ofile.write(uxs::as_span(buf, sz));
            ss_ref.write(buf, sz);
        }
        ofile.flush();
        ss_ref.flush();
    }

    std::string str = ofile.str();
    VERIFY(str.size() >= brute_N);
    VERIFY(str == ss_ref.str());

    {
        uxs::istringbuf ifile(str);
        std::istringstream in_ss_ref(str);

        while (ifile) {
            VERIFY(ifile.tell() == in_ss_ref.tellg());
            unsigned sz = distribution(generator);
            char buf1[256], buf2[256];
            size_t n_read = ifile.read(uxs::as_span(buf1, sz));
            in_ss_ref.read(buf2, sz);
            VERIFY(static_cast<std::streamsize>(n_read) == in_ss_ref.gcount());
            VERIFY(std::equal(buf1, buf1 + n_read, buf2));
        }
        VERIFY(ifile.peek() == std::char_traits<char>::eof());
        VERIFY(in_ss_ref.peek() == std::char_traits<char>::eof());
    }

    return 0;
}

int test_iobuf_dev_random_block(uxs::iodevcaps caps) {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 1000000000);

    memdev dev(caps);
    uxs::ostringbuf ss_ref;

    int iter_count = brute_N;

    {
        uxs::devbuf ofile(dev, uxs::iomode::kOut);

        for (int i = 0, perc0 = -1; i < iter_count; ++i) {
            int perc = (500 * static_cast<int64_t>(i)) / iter_count;
            if (perc > perc0) {
                uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
                perc0 = perc;
            }

            VERIFY(ofile.tell() == ss_ref.tell());
            unsigned sz = distribution(generator) % 128;
            unsigned tot_size = static_cast<unsigned>(ss_ref.seek(0, uxs::seekdir::kEnd));
            unsigned pos = distribution(generator) % (tot_size + std::max<unsigned>(tot_size / 1000, 16));
            char buf[256];
            for (unsigned n = 0; n < sz; ++n) {
                buf[n] = ' ' + static_cast<char>(distribution(generator) % ('z' - ' '));
            }

            ofile.seek(pos);
            ss_ref.seek(pos);
            VERIFY(ofile.tell() == ss_ref.tell());

            ofile.write(uxs::as_span(buf, sz));
            ss_ref.write(uxs::as_span(buf, sz));
        }
        ss_ref.flush();
    }

    std::string str(dev.str());
    VERIFY(str == ss_ref.str());

    {
        uxs::devbuf ifile(dev, uxs::iomode::kIn);
        uxs::istringbuf in_ss_ref(str);

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

            char buf1[256], buf2[256];
            size_t n_read = ifile.read(uxs::as_span(buf1, sz));
            VERIFY(n_read == in_ss_ref.read(uxs::as_span(buf2, sz)));
            VERIFY(std::equal(buf1, buf1 + n_read, buf2));
            VERIFY(ifile.peek() == in_ss_ref.peek());
            ifile.clear();
            in_ss_ref.clear();
        }
    }

    return 0;
}

int test_iobuf_dev_random_block_not_mappable() { return test_iobuf_dev_random_block(uxs::iodevcaps::kNone); }
int test_iobuf_dev_random_block_mappable() { return test_iobuf_dev_random_block(uxs::iodevcaps::kMappable); }

void test_iobuf_file_mode(uxs::iomode mode, std::string_view what_to_write, bool can_create_new,
                          bool can_open_when_existing, std::string_view what_to_write_when_existing,
                          std::string_view what_to_read_when_existing) {
    std::string fname = g_testdata_path + "test_file.txt";

    if (!(mode & uxs::iomode::kOut)) {
        {
            uxs::filebuf ifile(fname.c_str(), mode);
            VERIFY(!ifile);
        }

        VERIFY(mode & uxs::iomode::kIn);
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
    test_iobuf_file_mode(uxs::detail::iomode_from_str(mode, uxs::iomode::kNone), what_to_write, can_create_new,
                         can_open_when_existing, what_to_write_when_existing, what_to_read_when_existing);
}

int test_iobuf_file_modes() {
    test_iobuf_file_mode(uxs::iomode::kIn, "Hello, world", false, false, "", "");

    test_iobuf_file_mode(uxs::iomode::kOut, "Hello, world", false, true, "HELLO", "HELLO, world");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kTruncate, "Hello, world", false, true, "HELLO", "HELLO");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kTruncate | uxs::iomode::kAppend, "Hello, world", false, true,
                         "HELLO", "HELLO");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kAppend, "Hello, world", false, true, "HELLO",
                         "Hello, worldHELLO");

    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kExcl, "Hello, world", false, true, "HELLO", "HELLO, world");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kExcl | uxs::iomode::kTruncate, "Hello, world", false, true,
                         "HELLO", "HELLO");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kExcl | uxs::iomode::kTruncate | uxs::iomode::kAppend,
                         "Hello, world", false, true, "HELLO", "HELLO");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kExcl | uxs::iomode::kAppend, "Hello, world", false, true,
                         "HELLO", "Hello, worldHELLO");

    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kCreate, "Hello, world", true, true, "HELLO", "HELLO, world");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kCreate | uxs::iomode::kTruncate, "Hello, world", true, true,
                         "HELLO", "HELLO");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kCreate | uxs::iomode::kTruncate | uxs::iomode::kAppend,
                         "Hello, world", true, true, "HELLO", "HELLO");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kCreate | uxs::iomode::kAppend, "Hello, world", true, true,
                         "HELLO", "Hello, worldHELLO");

    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kCreate | uxs::iomode::kExcl, "Hello, world", true, false, "",
                         "");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kCreate | uxs::iomode::kExcl | uxs::iomode::kTruncate,
                         "Hello, world", true, false, "", "");
    test_iobuf_file_mode(
        uxs::iomode::kOut | uxs::iomode::kCreate | uxs::iomode::kExcl | uxs::iomode::kTruncate | uxs::iomode::kAppend,
        "Hello, world", true, false, "", "");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kCreate | uxs::iomode::kExcl | uxs::iomode::kAppend,
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

int test_iobuf_file_text_mode() {
    std::string fname = g_testdata_path + "test_file.txt";
    std::string_view txt = "hello\n\nhello\n";
    std::string_view crlf_txt = "hello\r\n\r\nhello\r\n";

    uxs::filebuf ofile(fname.c_str(),
                       uxs::iomode::kOut | uxs::iomode::kCreate | uxs::iomode::kTruncate | uxs::iomode::kCrLf);
    VERIFY(ofile);
    ofile.write(txt);
    ofile.close();

    {
        uxs::filebuf ifile(fname.c_str(), uxs::iomode::kIn);
        VERIFY(ifile);

        std::vector<char> data;
        data.resize(crlf_txt.size());
        VERIFY(ifile.read(data) == crlf_txt.size());
        VERIFY(std::equal(data.begin(), data.end(), crlf_txt.data()));
    }

    {
        uxs::filebuf ifile(fname.c_str(), uxs::iomode::kIn | uxs::iomode::kCrLf);
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
    {
        uxs::sysfile ifile((g_testdata_path + "zlib/test.bin").c_str(), "r");
        uxs::u8filebuf ofile((g_testdata_path + "zlib/test-1.bin").c_str(), "wz");
        VERIFY(ifile && ofile);
        size_t n_read = 0;
        do {
            ofile.reserve();
            if (ifile.read(ofile.first_avail(), ofile.avail(), n_read) < 0) { return -1; }
            ofile.advance(n_read);
        } while (n_read);
    }

    {
        uxs::u8filebuf ifile((g_testdata_path + "zlib/test-1.bin").c_str(), "rz");
        uxs::sysfile ofile((g_testdata_path + "zlib/test-2.bin").c_str(), "w");
        VERIFY(ifile && ofile);
        size_t n_written = 0;
        while (ifile.peek() != uxs::u8iobuf::traits_type::eof()) {
            if (ofile.write(ifile.first_avail(), ifile.avail(), n_written) < 0) { return -1; }
            ifile.advance(n_written);
        }
    }

    {
        uxs::u8filebuf ifile1((g_testdata_path + "zlib/test.bin").c_str(), "r");
        uxs::u8filebuf ifile2((g_testdata_path + "zlib/test-2.bin").c_str(), "r");
        VERIFY(ifile1 && ifile2);
        uxs::u8ibuf_iterator in1(ifile1), in2(ifile2), in_end{};
        VERIFY(std::equal(in1, in_end, in2));
        VERIFY(ifile2.peek() == std::char_traits<uint8_t>::eof());
    }
    uxs::sysfile::remove((g_testdata_path + "zlib/test-1.bin").c_str());
    uxs::sysfile::remove((g_testdata_path + "zlib/test-2.bin").c_str());
    return 0;
}

int test_iobuf_zlib_mappable(uxs::iodevcaps caps) {
    memdev middev(caps);

    {
        uxs::sysfile ifile((g_testdata_path + "zlib/test.bin").c_str(), "r");
        uxs::u8devbuf ofile(middev, uxs::iomode::kOut | uxs::iomode::kZCompr);
        VERIFY(ifile && ofile);
        size_t n_read = 0;
        do {
            ofile.reserve();
            if (ifile.read(ofile.first_avail(), ofile.avail(), n_read) < 0) { return -1; }
            ofile.advance(n_read);
        } while (n_read);
    }

    middev.seek(0, uxs::seekdir::kBeg);

    {
        uxs::u8devbuf ifile(middev, uxs::iomode::kIn | uxs::iomode::kZCompr);
        uxs::sysfile ofile((g_testdata_path + "zlib/test-2.bin").c_str(), "w");
        VERIFY(ifile && ofile);
        size_t n_written = 0;
        while (ifile.peek() != uxs::u8iobuf::traits_type::eof()) {
            if (ofile.write(ifile.first_avail(), ifile.avail(), n_written) < 0) { return -1; }
            ifile.advance(n_written);
        }
    }

    {
        uxs::u8filebuf ifile1((g_testdata_path + "zlib/test.bin").c_str(), "r");
        uxs::u8filebuf ifile2((g_testdata_path + "zlib/test-2.bin").c_str(), "r");
        VERIFY(ifile1 && ifile2);
        uxs::u8ibuf_iterator in1(ifile1), in2(ifile2), in_end{};
        VERIFY(std::equal(in1, in_end, in2));
        VERIFY(ifile2.peek() == std::char_traits<uint8_t>::eof());
    }
    uxs::sysfile::remove((g_testdata_path + "zlib/test-2.bin").c_str());
    return 0;
}

int test_iobuf_zlib_buf_not_mappable() { return test_iobuf_zlib_mappable(uxs::iodevcaps::kNone); }
int test_iobuf_zlib_buf_mappable() { return test_iobuf_zlib_mappable(uxs::iodevcaps::kMappable); }

}  // namespace

ADD_TEST_CASE("", "iobuf", test_iobuf_file_modes);
ADD_TEST_CASE("", "iobuf", test_iobuf_file_text_mode);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_crlf_not_mappable);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_dev_sequential_not_mappable);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_dev_sequential_str);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_dev_sequential_block_not_mappable);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_dev_sequential_block_str);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_dev_random_block_not_mappable);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_crlf_mappable);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_dev_sequential_mappable);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_dev_sequential_block_mappable);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_dev_random_block_mappable);
#if defined(USE_ZLIB)
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_zlib);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_zlib_buf_not_mappable);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_zlib_buf_mappable);
#endif
