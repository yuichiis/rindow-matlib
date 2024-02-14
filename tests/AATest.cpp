#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdbool.h>

using testing::ContainerEq;

namespace {

template <typename T>
class AATest : public ::testing::Test {
protected:
    virtual void test_exec(T value)
    {
        EXPECT_EQ(0,value);
    }
};
typedef ::testing::Types<float, double> TestTypes;
TYPED_TEST_SUITE(AATest, TestTypes);

TYPED_TEST(AATest, data1) {
    TypeParam value = 1;
    this->test_exec(value);
}
TYPED_TEST(AATest, data2) {
    TypeParam value = 0;
    this->test_exec(value);
}
}
