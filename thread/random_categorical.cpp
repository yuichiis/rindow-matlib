#include "rindow/matlib.h"
#include "common.hpp"
#include <random>
#include <vector>
#include <algorithm>
#include <cmath>

using rindow::matlib::ParallelOperation;

namespace {

template <typename T>
class RandomCategorical
{
private:
    // Constructs a cumulative distribution for a single row prior to normalization and returns the total. Returns 0 if the probability cannot be defined.
    static T buildCdf(const T *row, int32_t numClasses, T *cdf)
    {
        T maxv = row[0];
        for(int32_t k = 1; k < numClasses; k++) {
            if(row[k] > maxv) {
                maxv = row[k];
            }
        }
        T total = T(0);
        for(int32_t k = 0; k < numClasses; k++) {
            total += std::exp(row[k] - maxv);
            cdf[k] = total;
        }
        if(!(total > T(0)) || !std::isfinite(total)) {
            return T(0);
        }
        return total;
    }

    // Determines the class corresponding to u (0 <= u < total) using binary search.
    static int32_t pick(const T *cdf, int32_t numClasses, T u)
    {
        int32_t k = static_cast<int32_t>(
            std::upper_bound(cdf, cdf + numClasses, u) - cdf
        );
        // Insurance against rounding errors
        return (k >= numClasses) ? numClasses - 1 : k;
    }

    // Parallelization by batch axis: cell.begin to cell.end is the range of rows
    static void kernelByBatch(
        ParallelOperation::cellInfo cell,
        int32_t numClasses,
        int32_t numSamples,
        const T *logits,
        int32_t *y,
        uint64_t seed
    )
    {
        std::mt19937_64 engine(seed + static_cast<uint64_t>(cell.id));
        std::uniform_real_distribution<T> dist(T(0), T(1));
        std::vector<T> cdf(numClasses);

        for(int32_t b = cell.begin; b < cell.end; b++) {
            const T *row = logits + static_cast<int64_t>(b) * numClasses;
            int32_t *out = y + static_cast<int64_t>(b) * numSamples;

            T total = buildCdf(row, numClasses, cdf.data());
            if(total <= T(0)) {
                std::fill(out, out + numSamples, 0);
                continue;
            }
            for(int32_t s = 0; s < numSamples; s++) {
                out[s] = pick(cdf.data(), numClasses, dist(engine) * total);
            }
        }
    }

    // Parallelization by sample axis: cell.begin to cell.end is the range of samples within one row. cdf is shared by all cells (read-only).
    static void kernelBySample(
        ParallelOperation::cellInfo cell,
        int32_t numClasses,
        const T *cdf,
        T total,
        int32_t *out,
        uint64_t seed
    )
    {
        std::mt19937_64 engine(seed + static_cast<uint64_t>(cell.id));
        std::uniform_real_distribution<T> dist(T(0), T(1));

        for(int32_t s = cell.begin; s < cell.end; s++) {
            out[s] = pick(cdf, numClasses, dist(engine) * total);
        }
    }

public:
    static void execute(
        int32_t batchSize,
        int32_t numClasses,
        int32_t numSamples,
        const T *logits,
        int32_t *y,
        uint64_t seed
    )
    {
        if(batchSize <= 0 || numClasses <= 0 || numSamples <= 0) {
            return;
        }
        if(seed==0) {
            std::random_device seed_gen;
            seed = (static_cast<uint64_t>(seed_gen()) << 32) | seed_gen();
        }

        if(batchSize >= numSamples) {
            ParallelOperation::execute(
                batchSize, kernelByBatch,
                numClasses, numSamples, logits, y, seed
            );
            return;
        }

        // batch < numSamples: rows are processed sequentially and samples within rows are parallelized
        std::vector<T> cdf(numClasses);
        for(int32_t b = 0; b < batchSize; b++) {
            const T *row = logits + static_cast<int64_t>(b) * numClasses;
            int32_t *out = y + static_cast<int64_t>(b) * numSamples;

            T total = buildCdf(row, numClasses, cdf.data());
            if(total <= T(0)) {
                std::fill(out, out + numSamples, 0);
                continue;
            }
            // Shift seed for each row to avoid overlapping random number sequences (multiple of golden ratio constant)
            uint64_t rowSeed = seed + static_cast<uint64_t>(b) * 0x9E3779B97F4A7C15ULL;
            ParallelOperation::execute(
                numSamples, kernelBySample,
                numClasses, static_cast<const T*>(cdf.data()), total, out, rowSeed
            );
        }
    }
};

}

extern "C" {
void rindow_matlib_s_randomcategorical(
    int32_t batchSize,
    int32_t numClasses,
    int32_t numSamples,
    const float *logits,
    int32_t *y,
    uint64_t seed
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    RandomCategorical<float>::execute(batchSize, numClasses, numSamples, logits, y, seed);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_d_randomcategorical(
    int32_t batchSize,
    int32_t numClasses,
    int32_t numSamples,
    const double *logits,
    int32_t *y,
    uint64_t seed
)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    RandomCategorical<double>::execute(batchSize, numClasses, numSamples, logits, y, seed);
    RINDOW_END_CLEAR_EXCEPTION;
}

}
