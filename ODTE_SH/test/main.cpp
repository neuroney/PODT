#include "benchmark_runner.h"
#include "ODTE_SH_tester.h"

int main(int argc, char **argv)
{
    return RunBenchmarkMain(argc, argv, [](const BenchmarkCase &benchmark_case, const BenchmarkConfig &config) {
        cout << "Number of non-leaf nodes: " << power2_ZZ(benchmark_case.depth) - 1 << endl;
        ODTE_TIME_TEST(benchmark_case.depth, benchmark_case.attributes, config.msgbit, config.cycles, config.debug);
    });
}
