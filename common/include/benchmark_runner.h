#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

struct BenchmarkCase
{
    int depth;
    int attributes;
};

struct BenchmarkConfig
{
    std::vector<BenchmarkCase> cases;
    int msgbit = 10;
    int cycles = 5;
    bool debug = false;
};

inline BenchmarkConfig DefaultBenchmarkConfig()
{
    return {
        {
            {3, 13},
            {8, 9},
            {13, 13},
            {17, 57},
            {20, 784},
        },
        10,
        5,
        false,
    };
}

inline BenchmarkConfig QuickBenchmarkConfig()
{
    return {
        {
            {3, 4},
        },
        4,
        1,
        false,
    };
}

inline void PrintBenchmarkUsage(std::ostream &out, const char *program)
{
    out << "Usage: " << program << " [--quick] [--depth <n> --attributes <n>] [--msgbit <n>] [--cycles <n>] [--debug]\n";
    out << "  --quick         Run a single small benchmark case suitable for CI smoke tests.\n";
    out << "  --depth         Override the tree depth for a single custom benchmark case.\n";
    out << "  --attributes    Override the attribute count for a single custom benchmark case.\n";
    out << "  --msgbit        Set the message bit length (default: 10, quick: 4).\n";
    out << "  --cycles        Set the number of timing repetitions (default: 5, quick: 1).\n";
    out << "  --debug         Enable protocol debug mode.\n";
    out << "  --help          Show this message.\n";
}

inline bool ParsePositiveInt(const char *value, int &parsed)
{
    char *end = nullptr;
    const long candidate = std::strtol(value, &end, 10);
    if (end == value || *end != '\0' || candidate <= 0)
    {
        return false;
    }

    parsed = static_cast<int>(candidate);
    return true;
}

inline bool ParseBenchmarkArgs(int argc, char **argv, BenchmarkConfig &config, bool &show_help, std::string &error)
{
    config = DefaultBenchmarkConfig();
    show_help = false;
    bool single_case_requested = false;
    int single_depth = 0;
    int single_attributes = 0;

    for (int i = 1; i < argc; ++i)
    {
        const std::string arg = argv[i];
        if (arg == "--help")
        {
            show_help = true;
            return true;
        }

        if (arg == "--quick")
        {
            config = QuickBenchmarkConfig();
            continue;
        }

        if (arg == "--debug")
        {
            config.debug = true;
            continue;
        }

        if (i + 1 >= argc)
        {
            error = "missing value for argument: " + arg;
            return false;
        }

        int parsed = 0;
        if (!ParsePositiveInt(argv[i + 1], parsed))
        {
            error = "expected a positive integer after " + arg;
            return false;
        }

        if (arg == "--depth")
        {
            single_case_requested = true;
            single_depth = parsed;
        }
        else if (arg == "--attributes")
        {
            single_case_requested = true;
            single_attributes = parsed;
        }
        else if (arg == "--msgbit")
        {
            config.msgbit = parsed;
        }
        else if (arg == "--cycles")
        {
            config.cycles = parsed;
        }
        else
        {
            error = "unknown argument: " + arg;
            return false;
        }

        ++i;
    }

    if (single_case_requested)
    {
        if (single_depth == 0 || single_attributes == 0)
        {
            error = "--depth and --attributes must be provided together";
            return false;
        }

        config.cases = {{single_depth, single_attributes}};
    }

    return true;
}

template <typename Runner>
int RunBenchmarkMain(int argc, char **argv, Runner run_case)
{
    BenchmarkConfig config;
    bool show_help = false;
    std::string error;
    if (!ParseBenchmarkArgs(argc, argv, config, show_help, error))
    {
        std::cerr << error << '\n';
        PrintBenchmarkUsage(std::cerr, argv[0]);
        return 1;
    }

    if (show_help)
    {
        PrintBenchmarkUsage(std::cout, argv[0]);
        return 0;
    }

    for (const BenchmarkCase &benchmark_case : config.cases)
    {
        std::cout << "=== Testing depth: " << benchmark_case.depth
                  << ", N_attribute: " << benchmark_case.attributes << " ===\n";
        std::cout << "Message bit-length: " << config.msgbit
                  << ", repetitions: " << config.cycles << '\n';
        run_case(benchmark_case, config);
    }

    return 0;
}
