#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdlib.h>
#include <cmath>
#include <limits>
#include <vector>

using testing::ContainerEq;

namespace {

template <typename T>
class RandomCategoricalTest : public ::testing::Test {
protected:
    virtual void test_matlib_randomcategorical(
        int32_t batchSize,
        int32_t numClasses,
        int32_t numSamples,
        const float *logits,
        int32_t *y,
        uint64_t seed)
    {
        rindow_matlib_s_randomcategorical(batchSize,numClasses,numSamples,logits,y,seed);
    }
    virtual void test_matlib_randomcategorical(
        int32_t batchSize,
        int32_t numClasses,
        int32_t numSamples,
        const double *logits,
        int32_t *y,
        uint64_t seed)
    {
        rindow_matlib_d_randomcategorical(batchSize,numClasses,numSamples,logits,y,seed);
    }

    // Count the number of samples for each class in the specified batch.
    // If an out-of-range value is found, it will fail instead of falling back to the -1 index.
    std::vector<int32_t> countClasses(
        const std::vector<int32_t> &y,
        int32_t batch,
        int32_t numClasses,
        int32_t numSamples)
    {
        std::vector<int32_t> counts(numClasses, 0);
        for (int32_t s = 0; s < numSamples; s++) {
            int32_t v = y[(size_t)batch*numSamples + s];
            EXPECT_GE(v, 0);
            EXPECT_LT(v, numClasses);
            if (v >= 0 && v < numClasses) {
                counts[v]++;
            }
        }
        return counts;
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(RandomCategoricalTest, TestTypes);

// When all classes have the same logits, it follows a uniform distribution
TYPED_TEST(RandomCategoricalTest, uniform) {
    const int32_t batchSize  = 1;
    const int32_t numClasses = 4;
    const int32_t numSamples = 10000;
    const uint64_t seed = 0;

    std::vector<TypeParam> logits(batchSize*numClasses, (TypeParam)0);
    std::vector<int32_t> y(batchSize*numSamples, -1);

    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples,
        logits.data(), y.data(), seed
    );

    std::vector<int32_t> counts = this->countClasses(y, 0, numClasses, numSamples);
    for (int32_t k = 0; k < numClasses; k++) {
        TypeParam ratio = (TypeParam)counts[k] / (TypeParam)numSamples;
        std::cout << "class" << k << "=" << ratio << std::endl;
        EXPECT_LT(std::abs(ratio - (TypeParam)0.25), (TypeParam)0.03);
    }
}

// When logits = log(p), the sampling frequency approaches p
TYPED_TEST(RandomCategoricalTest, distribution) {
    const int32_t batchSize  = 1;
    const int32_t numClasses = 3;
    const int32_t numSamples = 20000;
    const uint64_t seed = 0;

    const TypeParam p[numClasses] = {(TypeParam)0.1, (TypeParam)0.2, (TypeParam)0.7};
    std::vector<TypeParam> logits(batchSize*numClasses);
    for (int32_t k = 0; k < numClasses; k++) {
        logits[k] = std::log(p[k]);
    }
    std::vector<int32_t> y(batchSize*numSamples, -1);

    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples,
        logits.data(), y.data(), seed
    );

    std::vector<int32_t> counts = this->countClasses(y, 0, numClasses, numSamples);
    for (int32_t k = 0; k < numClasses; k++) {
        TypeParam ratio = (TypeParam)counts[k] / (TypeParam)numSamples;
        std::cout << "class" << k << "=" << ratio << " (expected " << p[k] << ")" << std::endl;
        EXPECT_LT(std::abs(ratio - p[k]), (TypeParam)0.03);
    }
}

// Logits do not need to be normalized (adding a constant does not change the distribution)
TYPED_TEST(RandomCategoricalTest, unnormalized) {
    const int32_t batchSize  = 1;
    const int32_t numClasses = 3;
    const int32_t numSamples = 20000;
    const uint64_t seed = 0;

    const TypeParam p[numClasses] = {(TypeParam)0.1, (TypeParam)0.2, (TypeParam)0.7};
    std::vector<TypeParam> logits(batchSize*numClasses);
    for (int32_t k = 0; k < numClasses; k++) {
        logits[k] = std::log(p[k]) + (TypeParam)5.0;
    }
    std::vector<int32_t> y(batchSize*numSamples, -1);

    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples,
        logits.data(), y.data(), seed
    );

    std::vector<int32_t> counts = this->countClasses(y, 0, numClasses, numSamples);
    for (int32_t k = 0; k < numClasses; k++) {
        TypeParam ratio = (TypeParam)counts[k] / (TypeParam)numSamples;
        EXPECT_LT(std::abs(ratio - p[k]), (TypeParam)0.03);
    }
}

// Batches are sampled independently from different distributions, and the output is in [batchSize, numSamples] order
TYPED_TEST(RandomCategoricalTest, batch) {
    const int32_t batchSize  = 2;
    const int32_t numClasses = 3;
    const int32_t numSamples = 1000;
    const uint64_t seed = 0;

    // batch0 will almost certainly choose class0, and batch1 will almost certainly choose class2
    std::vector<TypeParam> logits = {
        (TypeParam)50, (TypeParam)0,  (TypeParam)0,
        (TypeParam)0,  (TypeParam)0,  (TypeParam)50,
    };
    std::vector<int32_t> y(batchSize*numSamples, -1);

    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples,
        logits.data(), y.data(), seed
    );

    std::vector<int32_t> counts0 = this->countClasses(y, 0, numClasses, numSamples);
    std::vector<int32_t> counts1 = this->countClasses(y, 1, numClasses, numSamples);
    EXPECT_EQ(numSamples, counts0[0]);
    EXPECT_EQ(numSamples, counts1[2]);
}

// Classes with -inf logits have probability 0 and are not selected
TYPED_TEST(RandomCategoricalTest, minus_infinity) {
    const int32_t batchSize  = 1;
    const int32_t numClasses = 4;
    const int32_t numSamples = 10000;
    const uint64_t seed = 0;

    const TypeParam ninf = -std::numeric_limits<TypeParam>::infinity();
    std::vector<TypeParam> logits = {(TypeParam)0, ninf, (TypeParam)0, ninf};
    std::vector<int32_t> y(batchSize*numSamples, -1);

    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples,
        logits.data(), y.data(), seed
    );

    std::vector<int32_t> counts = this->countClasses(y, 0, numClasses, numSamples);
    EXPECT_EQ(0, counts[1]);
    EXPECT_EQ(0, counts[3]);
    EXPECT_EQ(numSamples, counts[0] + counts[2]);
    EXPECT_LT(std::abs((TypeParam)counts[0]/(TypeParam)numSamples - (TypeParam)0.5), (TypeParam)0.03);
}

// Even with large logits, it is sampled correctly without overflowing
TYPED_TEST(RandomCategoricalTest, large_logits) {
    const int32_t batchSize  = 1;
    const int32_t numClasses = 3;
    const int32_t numSamples = 10000;
    const uint64_t seed = 0;

    std::vector<TypeParam> logits = {(TypeParam)1000, (TypeParam)1000, (TypeParam)-1000};
    std::vector<int32_t> y(batchSize*numSamples, -1);

    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples,
        logits.data(), y.data(), seed
    );

    std::vector<int32_t> counts = this->countClasses(y, 0, numClasses, numSamples);
    EXPECT_EQ(0, counts[2]);
    EXPECT_LT(std::abs((TypeParam)counts[0]/(TypeParam)numSamples - (TypeParam)0.5), (TypeParam)0.03);
}

// When there is only one class, it always returns 0
TYPED_TEST(RandomCategoricalTest, single_class) {
    const int32_t batchSize  = 3;
    const int32_t numClasses = 1;
    const int32_t numSamples = 100;
    const uint64_t seed = 0;

    std::vector<TypeParam> logits = {(TypeParam)0.5, (TypeParam)-3, (TypeParam)10};
    std::vector<int32_t> y(batchSize*numSamples, -1);

    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples,
        logits.data(), y.data(), seed
    );

    for (size_t i = 0; i < y.size(); i++) {
        EXPECT_EQ(0, y[i]);
    }
}

// Specifying a seed allows for reproducibility, and different or unspecified seeds (0) will produce different results
TYPED_TEST(RandomCategoricalTest, seed) {
    const int32_t batchSize  = 2;
    const int32_t numClasses = 10;
    const int32_t numSamples = 500;

    std::vector<TypeParam> logits(batchSize*numClasses, (TypeParam)0);
    std::vector<int32_t> y1(batchSize*numSamples, -1);
    std::vector<int32_t> y2(batchSize*numSamples, -1);
    std::vector<int32_t> y3(batchSize*numSamples, -1);
    std::vector<int32_t> y4(batchSize*numSamples, -1);
    std::vector<int32_t> y5(batchSize*numSamples, -1);

    // Same seed    same result
    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples, logits.data(), y1.data(), 123);
    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples, logits.data(), y2.data(), 123);
    EXPECT_THAT(y1, ContainerEq(y2));

    // Different seed    different result
    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples, logits.data(), y3.data(), 456);
    EXPECT_THAT(y1, testing::Not(ContainerEq(y3)));

    // seed=0 is not reinitialized, so calling it consecutively will produce different results
    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples, logits.data(), y4.data(), 0);
    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples, logits.data(), y5.data(), 0);
    EXPECT_THAT(y4, testing::Not(ContainerEq(y5)));
}

// Even if only the upper 32 bits of the seed differ, the result will change (uint64_t seed is used)
TYPED_TEST(RandomCategoricalTest, seed_upper_bits) {
    const int32_t batchSize  = 1;
    const int32_t numClasses = 10;
    const int32_t numSamples = 500;

    std::vector<TypeParam> logits(batchSize*numClasses, (TypeParam)0);
    std::vector<int32_t> y1(batchSize*numSamples, -1);
    std::vector<int32_t> y2(batchSize*numSamples, -1);

    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples, logits.data(), y1.data(), (uint64_t)1);
    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples, logits.data(), y2.data(), ((uint64_t)1 << 32) | 1);
    EXPECT_THAT(y1, testing::Not(ContainerEq(y2)));
}
// ---- Large batch size tests (parallelized along the batch axis) ----

// With a large batch and 1 sample, each row must be sampled from its own logits, in row order.
// Row i almost certainly picks class cls(i), so any mixing between rows or misordering is detected.
TYPED_TEST(RandomCategoricalTest, large_batch_ordering) {
    const int32_t batchSize  = 10000;
    const int32_t numClasses = 5;
    const int32_t numSamples = 1;
    const uint64_t seed = 0;

    std::vector<TypeParam> logits(batchSize*numClasses, (TypeParam)0);
    std::vector<int32_t> expected(batchSize);
    for (int32_t i = 0; i < batchSize; i++) {
        int32_t cls = (i*7 + 3) % numClasses;
        expected[i] = cls;
        logits[(size_t)i*numClasses + cls] = (TypeParam)50;
    }
    std::vector<int32_t> y(batchSize*numSamples, -1);

    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples,
        logits.data(), y.data(), seed
    );

    EXPECT_THAT(y, ContainerEq(expected));
}

// Large batch with several samples per row (batchSize >= numSamples).
// Every sample in row i must belong to the class chosen for row i.
TYPED_TEST(RandomCategoricalTest, large_batch_multi_samples) {
    const int32_t batchSize  = 2000;
    const int32_t numClasses = 6;
    const int32_t numSamples = 8;
    const uint64_t seed = 0;

    std::vector<TypeParam> logits(batchSize*numClasses, (TypeParam)0);
    std::vector<int32_t> expected(batchSize*numSamples);
    for (int32_t i = 0; i < batchSize; i++) {
        int32_t cls = (i*5 + 1) % numClasses;
        logits[(size_t)i*numClasses + cls] = (TypeParam)50;
        for (int32_t s = 0; s < numSamples; s++) {
            expected[(size_t)i*numSamples + s] = cls;
        }
    }
    std::vector<int32_t> y(batchSize*numSamples, -1);

    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples,
        logits.data(), y.data(), seed
    );

    EXPECT_THAT(y, ContainerEq(expected));
}

// With a large batch and 1 sample per row, the frequency over all rows approaches p.
// This also checks that rows handled by different threads are not correlated with each other.
TYPED_TEST(RandomCategoricalTest, large_batch_distribution) {
    const int32_t batchSize  = 30000;
    const int32_t numClasses = 3;
    const int32_t numSamples = 1;
    const uint64_t seed = 0;

    const TypeParam p[numClasses] = {(TypeParam)0.1, (TypeParam)0.2, (TypeParam)0.7};
    std::vector<TypeParam> logits(batchSize*numClasses);
    for (int32_t i = 0; i < batchSize; i++) {
        for (int32_t k = 0; k < numClasses; k++) {
            logits[(size_t)i*numClasses + k] = std::log(p[k]);
        }
    }
    std::vector<int32_t> y(batchSize*numSamples, -1);

    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples,
        logits.data(), y.data(), seed
    );

    std::vector<int32_t> counts(numClasses, 0);
    for (size_t i = 0; i < y.size(); i++) {
        EXPECT_GE(y[i], 0);
        EXPECT_LT(y[i], numClasses);
        if (y[i] >= 0 && y[i] < numClasses) {
            counts[y[i]]++;
        }
    }
    for (int32_t k = 0; k < numClasses; k++) {
        TypeParam ratio = (TypeParam)counts[k] / (TypeParam)(batchSize*numSamples);
        std::cout << "class" << k << "=" << ratio << " (expected " << p[k] << ")" << std::endl;
        EXPECT_LT(std::abs(ratio - p[k]), (TypeParam)0.03);
    }
}

// Rows with identical logits must not all produce the same value (no duplicated random stream across rows)
TYPED_TEST(RandomCategoricalTest, large_batch_rows_differ) {
    const int32_t batchSize  = 10000;
    const int32_t numClasses = 10;
    const int32_t numSamples = 1;
    const uint64_t seed = 0;

    std::vector<TypeParam> logits(batchSize*numClasses, (TypeParam)0);
    std::vector<int32_t> y(batchSize*numSamples, -1);

    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples,
        logits.data(), y.data(), seed
    );

    std::vector<int32_t> counts(numClasses, 0);
    for (size_t i = 0; i < y.size(); i++) {
        if (y[i] >= 0 && y[i] < numClasses) {
            counts[y[i]]++;
        }
    }
    for (int32_t k = 0; k < numClasses; k++) {
        EXPECT_GT(counts[k], 0);
        EXPECT_LT(std::abs((TypeParam)counts[k]/(TypeParam)batchSize - (TypeParam)0.1), (TypeParam)0.03);
    }
}

// Reproducibility with a large batch: the same seed gives the same result, a different seed does not
TYPED_TEST(RandomCategoricalTest, large_batch_seed) {
    const int32_t batchSize  = 10000;
    const int32_t numClasses = 10;
    const int32_t numSamples = 2;

    std::vector<TypeParam> logits(batchSize*numClasses, (TypeParam)0);
    std::vector<int32_t> y1(batchSize*numSamples, -1);
    std::vector<int32_t> y2(batchSize*numSamples, -1);
    std::vector<int32_t> y3(batchSize*numSamples, -1);

    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples, logits.data(), y1.data(), 123);
    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples, logits.data(), y2.data(), 123);
    EXPECT_THAT(y1, ContainerEq(y2));

    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples, logits.data(), y3.data(), 456);
    EXPECT_THAT(y1, testing::Not(ContainerEq(y3)));
}

// Boundary between the batch-axis and sample-axis parallelization (batchSize == numSamples).
// Both axes are large; every sample of row i must still belong to row i's class.
TYPED_TEST(RandomCategoricalTest, batch_equals_samples) {
    const int32_t batchSize  = 300;
    const int32_t numClasses = 4;
    const int32_t numSamples = 300;
    const uint64_t seed = 0;

    std::vector<TypeParam> logits(batchSize*numClasses, (TypeParam)0);
    std::vector<int32_t> expected(batchSize*numSamples);
    for (int32_t i = 0; i < batchSize; i++) {
        int32_t cls = (i*3 + 2) % numClasses;
        logits[(size_t)i*numClasses + cls] = (TypeParam)50;
        for (int32_t s = 0; s < numSamples; s++) {
            expected[(size_t)i*numSamples + s] = cls;
        }
    }
    std::vector<int32_t> y(batchSize*numSamples, -1);

    this->test_matlib_randomcategorical(
        batchSize, numClasses, numSamples,
        logits.data(), y.data(), seed
    );

    EXPECT_THAT(y, ContainerEq(expected));
}
}