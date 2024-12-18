#include "test_suite.h"

#include "uxs/cli/parser.h"
#include "uxs/io/filebuf.h"

extern std::string g_testdata_path;

namespace {

int test_1() {
    bool rec = false, utf16 = false;
    std::string infile = "", fmt = "csv";

    auto options = (uxs::cli::option({"-r", "--recursive"}).set(rec) % "convert files recursively") &
                   (uxs::cli::option({"-o"}) & uxs::cli::value("<output format>", fmt)) &
                   (uxs::cli::option({"-utf16"}).set(utf16) % "use UTF-16 encoding");

    auto cli = uxs::cli::command("run_tpu") << uxs::cli::value("<input file>", infile) << options.clone();

    std::string full_file_name = g_testdata_path + "cli/test-001.txt";

    uxs::filebuf ifile(full_file_name.c_str(), "r");
    VERIFY(ifile);

    const size_t sz = static_cast<size_t>(ifile.seek(0, uxs::seekdir::end));
    ifile.seek(0);

    std::string txt;
    txt.resize(sz);
    txt.resize(ifile.read(uxs::as_span(&txt[0], sz)));

    VERIFY(txt == cli->make_man_page(uxs::cli::text_coloring::no_color));

    const char* cmd[] = {"convert", "--recursive", "input", "-o", "fmt", "-utf16"};
    auto result = cli->parse(sizeof(cmd) / sizeof(cmd[0]), cmd);

    VERIFY(result && result.argc_parsed == 6 && result.node == cli.get());
    VERIFY(rec == true);
    VERIFY(utf16 == true);
    VERIFY(infile == "input");
    VERIFY(fmt == fmt);
    return 0;
}

int test_2() {
    enum class mode { make, find, help };
    mode selected = mode::help;
    std::vector<std::string> input;
    std::string dict, out;
    bool split = false, progr = false, ver = false;

    auto dictionary = uxs::cli::required({"-dict"}) & uxs::cli::value("<dictionary>", dict);

    auto makeMode = uxs::cli::command("make").set(selected, mode::make)
                    << uxs::cli::values("<wordfile>", input) << dictionary.clone()
                    << (uxs::cli::option({"--progress", "-p"}).set(progr)) % "show progress";

    auto findMode = uxs::cli::command("find").set(selected, mode::find)
                    << uxs::cli::values("<infile>", input) << dictionary.clone()
                    << (uxs::cli::option({"-o", "--output"}) & uxs::cli::value("<outfile>", out)) %
                           "write to file instead of stdout"
                    << (uxs::cli::option({"-split"}).set(split, true) |
                        uxs::cli::option({"-nosplit"}).set(split, false)) %
                           "(do not) split output";

    auto cli = uxs::cli::command("finder") << std::move(makeMode) << std::move(findMode)
                                           << (uxs::cli::command("help").set(selected, mode::help)
                                               << uxs::cli::option({"-v", "--version"}).set(ver) % "show version");

    std::string full_file_name = g_testdata_path + "cli/test-002.txt";

    uxs::filebuf ifile(full_file_name.c_str(), "r");
    VERIFY(ifile);

    const size_t sz = static_cast<size_t>(ifile.seek(0, uxs::seekdir::end));
    ifile.seek(0);

    std::string txt;
    txt.resize(sz);
    txt.resize(ifile.read(uxs::as_span(&txt[0], sz)));

    VERIFY(txt == cli->get_subcommands().find("make")->second->make_man_page(uxs::cli::text_coloring::no_color) +
                      cli->get_subcommands().find("find")->second->make_man_page(uxs::cli::text_coloring::no_color) +
                      cli->get_subcommands().find("help")->second->make_man_page(uxs::cli::text_coloring::no_color));

    {
        const char* cmd[] = {"finder", "make", "in1", "in2", "-p", "in3", "-dict", "dict"};

        input.clear();
        dict = "", progr = false;

        auto result = cli->parse(sizeof(cmd) / sizeof(cmd[0]), cmd);

        VERIFY(result && result.argc_parsed == 8 && result.node == cli->get_subcommands().find("make")->second.get());
        VERIFY(selected == mode::make);
        VERIFY(progr == true);
        VERIFY(input == std::vector<std::string>{"in1", "in2", "in3"});
        VERIFY(dict == "dict");
    }

    {
        const char* cmd[] = {"finder", "find", "in1", "--output", "output", "-split", "-dict", "dict"};

        input.clear();
        dict = "", out = "", split = false;

        auto result = cli->parse(sizeof(cmd) / sizeof(cmd[0]), cmd);

        VERIFY(result && result.argc_parsed == 8 && result.node == cli->get_subcommands().find("find")->second.get());
        VERIFY(selected == mode::find);
        VERIFY(split == true);
        VERIFY(input == std::vector<std::string>{"in1"});
        VERIFY(out == "output");
        VERIFY(dict == "dict");
    }

    {
        const char* cmd[] = {"finder", "find", "in1", "--output", "output", "-nosplit", "-dict", "dict"};

        input.clear();
        dict = "", out = "", split = true;

        auto result = cli->parse(sizeof(cmd) / sizeof(cmd[0]), cmd);

        VERIFY(result && result.argc_parsed == 8 && result.node == cli->get_subcommands().find("find")->second.get());
        VERIFY(selected == mode::find);
        VERIFY(split == false);
        VERIFY(input == std::vector<std::string>{"in1"});
        VERIFY(out == "output");
        VERIFY(dict == "dict");
    }

    {
        const char* cmd[] = {"finder", "find", "in1", "-split", "in2", "-nosplit", "-dict", "dict"};

        input.clear();

        auto result = cli->parse(sizeof(cmd) / sizeof(cmd[0]), cmd);

        VERIFY(result.status == uxs::cli::parsing_status::conflicting_option && result.argc_parsed == 8 &&
               result.node->get_type() == uxs::cli::node_type::option &&
               static_cast<const uxs::cli::basic_option<char>&>(*result.node).get_keys()[0] == "-nosplit");
        VERIFY(selected == mode::find);
        VERIFY(input == std::vector<std::string>{"in1", "in2"});
        VERIFY(dict == "dict");
    }

    {
        const char* cmd[] = {"finder", "help", "-v"};

        ver = false;

        auto result = cli->parse(sizeof(cmd) / sizeof(cmd[0]), cmd);

        VERIFY(result && result.argc_parsed == 3 && result.node == cli->get_subcommands().find("help")->second.get());
        VERIFY(selected == mode::help);
        VERIFY(ver == true);
    }

    return 0;
}

int test_3() {
    int dev = 0, n = 4, m = 0;
    bool ver = false, help = false;
    std::string prog;
    std::vector<std::string> inputs, outputs, ref_data;
    auto cli = uxs::cli::command("run_tpu")
               << uxs::cli::overview("TPU device runner\n") << uxs::cli::overview("Author: gbuzykin@yandex.ru\n")
               << (uxs::cli::required({"-p"}) & uxs::cli::value("<program>", prog)) % "Program path."
               << (uxs::cli::required({"-i"}) & uxs::cli::values("<inputs>...", inputs)) % "Network input file(s)."
               << (uxs::cli::option({"-o"}) & uxs::cli::values("<outputs>...", outputs)) % "Network output file(s)."
               << (uxs::cli::option({"-c"}) & uxs::cli::values("<ref_data>...", ref_data)) %
                      "Network reference file(s) to compare output."
               << (uxs::cli::option({"-d"}) & uxs::cli::value("<device>", dev)) % "Use device file."
               << uxs::cli::option({"-t"}) % "Print execution timing."
               << uxs::cli::option({"-r", "--raw"}) % "Raw mode. Do not process input data (no padding/depadding)."
               << uxs::cli::option({"--save-raw"}) % "Save raw tensor(s)."
               << uxs::cli::option({"--perf"}) % "Measure performance."
               << (uxs::cli::option({"--n-proc="}) & uxs::cli::value("<n>", n)) %
                      "Number of parallel inference workers (4 by default)."
               << (uxs::cli::option({"--optional"}) & uxs::cli::value("<m>", m).optional())
               << uxs::cli::option({"-V", "--version"}).set(ver) % "Print version."
               << uxs::cli::option({"-h", "--help"}).set(help) % "Print help."
               << uxs::cli::command("help") % "Print this message or the help for the given subcommand."
               << uxs::cli::command("info") % "Print info.";

    std::string full_file_name = g_testdata_path + "cli/test-003.txt";

    uxs::filebuf ifile(full_file_name.c_str(), "r");
    VERIFY(ifile);

    const size_t sz = static_cast<size_t>(ifile.seek(0, uxs::seekdir::end));
    ifile.seek(0);

    std::string txt;
    txt.resize(sz);
    txt.resize(ifile.read(uxs::as_span(&txt[0], sz)));

    VERIFY(txt == cli->make_man_page(uxs::cli::text_coloring::no_color));

    {
        const char* cmd[] = {"run_tpu", "-p", "program.tpu", "-iin1.bin", "in2.bin"};
        auto result = cli->parse(sizeof(cmd) / sizeof(cmd[0]), cmd);
        VERIFY(result && result.argc_parsed == 5 && result.node == cli.get());
        VERIFY(prog == "program.tpu");
        VERIFY(inputs == std::vector<std::string>{"in1.bin", "in2.bin"});
    }

    {
        const char* cmd[] = {"run_tpu", "-help"};
        auto result = cli->parse(sizeof(cmd) / sizeof(cmd[0]), cmd);
        VERIFY(result.status == uxs::cli::parsing_status::unknown_option && result.argc_parsed == 1);
    }

    {
        const char* cmd[] = {"run_tpu", "--n-proc="};
        auto result = cli->parse(sizeof(cmd) / sizeof(cmd[0]), cmd);
        VERIFY(result.status == uxs::cli::parsing_status::invalid_value && result.argc_parsed == 2);
    }

    {
        const char* cmd[] = {"run_tpu", "--n-proc=a"};
        auto result = cli->parse(sizeof(cmd) / sizeof(cmd[0]), cmd);
        VERIFY(result.status == uxs::cli::parsing_status::invalid_value && result.argc_parsed == 1);
    }

    {
        prog.clear();
        inputs.clear();
        const char* cmd[] = {"run_tpu", "-p", "program.tpu", "-iin1.bin", "--n-proc=7"};
        auto result = cli->parse(sizeof(cmd) / sizeof(cmd[0]), cmd);
        VERIFY(result && result.argc_parsed == 5);
        VERIFY(n == 7);
        VERIFY(prog == "program.tpu");
        VERIFY(inputs == std::vector<std::string>{"in1.bin"});
    }

    {
        prog.clear();
        inputs.clear();
        const char* cmd[] = {"run_tpu", "-p", "program.tpu", "-iin1.bin", "--n-proc=11"};
        auto result = cli->parse(sizeof(cmd) / sizeof(cmd[0]), cmd);
        VERIFY(result && result.argc_parsed == 5);
        VERIFY(n == 11);
        VERIFY(prog == "program.tpu");
        VERIFY(inputs == std::vector<std::string>{"in1.bin"});
    }

    {
        prog.clear();
        inputs.clear();
        const char* cmd[] = {"run_tpu", "-p", "program.tpu", "-iin1.bin", "--n-proc=", "17"};
        auto result = cli->parse(sizeof(cmd) / sizeof(cmd[0]), cmd);
        VERIFY(result && result.argc_parsed == 6);
        VERIFY(n == 17);
        VERIFY(prog == "program.tpu");
        VERIFY(inputs == std::vector<std::string>{"in1.bin"});
    }

    {
        prog.clear();
        inputs.clear();
        const char* cmd[] = {"run_tpu", "-p", "program.tpu", "-iin1.bin", "-help"};
        auto result = cli->parse(sizeof(cmd) / sizeof(cmd[0]), cmd);
        VERIFY(result && result.argc_parsed == 5);
        VERIFY(prog == "program.tpu");
        VERIFY(inputs == std::vector<std::string>{"in1.bin", "-help"});
    }

    {
        prog.clear();
        inputs.clear();
        const char* cmd[] = {"run_tpu", "-p", "program.tpu", "-iin1.bin", "--optional"};
        auto result = cli->parse(sizeof(cmd) / sizeof(cmd[0]), cmd);
        VERIFY(result && result.argc_parsed == 5);
        VERIFY(prog == "program.tpu");
        VERIFY(inputs == std::vector<std::string>{"in1.bin"});
    }

    {
        const char* cmd[] = {"run_tpu", "--optionala"};
        auto result = cli->parse(sizeof(cmd) / sizeof(cmd[0]), cmd);
        VERIFY(result.status == uxs::cli::parsing_status::invalid_value && result.argc_parsed == 1);
    }

    {
        prog.clear();
        inputs.clear();
        const char* cmd[] = {"run_tpu", "-p", "program.tpu", "-iin1.bin", "--optional14"};
        auto result = cli->parse(sizeof(cmd) / sizeof(cmd[0]), cmd);
        VERIFY(result && result.argc_parsed == 5);
        VERIFY(m == 14);
        VERIFY(prog == "program.tpu");
        VERIFY(inputs == std::vector<std::string>{"in1.bin"});
    }

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "cli", test_1);
ADD_TEST_CASE("", "cli", test_2);
ADD_TEST_CASE("", "cli", test_3);
