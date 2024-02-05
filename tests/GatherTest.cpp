#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdbool.h>
#include <numeric>
#include "Utils.h"

using testing::ContainerEq;
using RindowTest::Utils;

namespace {

template <typename T>
class GatherTest : public ::testing::Test {
protected:
    void calcShapes(
        std::initializer_list<int32_t> shapeX,
        std::initializer_list<int32_t> shapeA,
        int32_t *m,int32_t *n,int32_t *k,int32_t *numClass, int32_t *outputSize
        )
    {
        auto postfixShape = shapeA.begin();
        *numClass = *postfixShape++;
        *m = 1;
        *n = std::accumulate(shapeX.begin(), shapeX.end(), 1, std::multiplies<int32_t>());
        *k = std::accumulate(postfixShape, shapeA.end(), 1, std::multiplies<int32_t>());
        *outputSize = (*n)*(*k);
    }   

    void calcShapes(
        int32_t axis,
        std::initializer_list<int32_t> shapeX,
        std::initializer_list<int32_t> shapeA,
        int32_t *m,int32_t *n,int32_t *k,int32_t *numClass, int32_t *outputSize
        )
    {
        int32_t ndim = shapeA.size();
        if(axis<0) {
            axis = ndim+axis;
        }
        auto postfixShape = shapeA.begin();
        *m = 1;
        for(int32_t i=0;i<axis;i++) {
            *m *= *postfixShape;
        }
        *numClass = *postfixShape++;
        *n = std::accumulate(postfixShape, shapeA.end(), 1, std::multiplies<int32_t>());
        *k = 1;
        *outputShape = (*n)*(*k);
    }

    virtual int32_t test_matlib_gather(
        int32_t reverse, int32_t addMode, int32_t n, int32_t k,
        int32_t numClass, int32_t dtype, int32_t *x,
        float *a, float *b
    )
    {
        return rindow_matlib_s_gather(reverse,addMode,n,k,numClass,dtype,x,a,b);
    }

    virtual int32_t test_matlib_gather(
        int32_t reverse, int32_t addMode, int32_t n, int32_t k,
        int32_t numClass, int32_t dtype, int32_t *x,
        double *a, double *b
    )
    {
        return rindow_matlib_d_gather(reverse,addMode,n,k,numClass,dtype,x,a,b);
    }

    virtual int32_t test_matlib_gather(
        int32_t reverse, int32_t addMode, int32_t n, int32_t k,
        int32_t numClass, int32_t dtype, int32_t *x,
        int32_t *a, int32_t *b
    )
    {
        int32_t data_dtype = rindow_matlib_dtype_int32;
        return rindow_matlib_i_gather(reverse,addMode,n,k,numClass,dtype,x,data_dtype,a,b);
    }

};
typedef ::testing::Types<float, double, int32_t> TestTypes;
TYPED_TEST_SUITE(GatherTest, TestTypes);

TYPED_TEST(GatherTest, GatherNormal1Dby1D) {
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeX = 4;
    const int32_t shapeA = 10;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;

    int32_t X[shapeX] = {3,2,1,1};
    TypeParam A[shapeA] = {10,11,12,13,14,15,16,17,18,19};
    calcShapes({shapeX},{shapeA},&m,&n,&k,&numClass,&outputSize);
    TypeParam B[shapeX];
    
    ASSERT_EQ(1,      m);
    ASSERT_EQ(shapeX, n);
    ASSERT_EQ(1,      k);
    ASSERT_EQ(shapeA, numClass);
    ASSERT_EQ(shapeX, outputSize);

    int32_t rc = test_matlib_gather(reverse,addMode,n,k,numClass,dtype,X,A,B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeX] = {13,12,11,11};
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherTest, GatherNormal1Dby2D) {
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeX0 = 2;
    const int32_t shapeX1 = 3;
    const int32_t shapeA = 10;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;

    int32_t X[shapeX0*shapeX1] = {
        3,2,1,
        1,2,3
    };
    TypeParam A[shapeA] = {10,11,12,13,14,15,16,17,18,19};
    calcShapes({shapeX0,shapeX1},{shapeA},&m,&n,&k,&numClass,&outputSize);
    TypeParam B[shapeX0*shapeX1];
    
    ASSERT_EQ(1,                m);
    ASSERT_EQ(shapeX0*shapeX1,  n);
    ASSERT_EQ(1,                k);
    ASSERT_EQ(shapeA,           numClass);
    ASSERT_EQ(shapeX0*shapeX1,  outputSize);

    int32_t rc = test_matlib_gather(reverse,addMode,n,k,numClass,dtype,X,A,B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeX0*shapeX1] = {
        13,12,11,
        11,12,13
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherTest, GatherNormal2Dby1D) {
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeX = 2;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 3;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;

    int32_t X[shapeX] = {
        3,1
    };
    TypeParam A[shapeA0*shapeA1] = {
        0, 0, 3,
        0, 0, 4,
        0, 2, 0,
        1, 0, 0
    };
    calcShapes({shapeX},{shapeA0,shapeA1},&m,&n,&k,&numClass,&outputSize);
    TypeParam B[shapeX*shapeA1];
    
    ASSERT_EQ(1,                m);
    ASSERT_EQ(shapeX,           n);
    ASSERT_EQ(shapeA1,          k);
    ASSERT_EQ(shapeA0,          numClass);
    ASSERT_EQ(shapeX*shapeA1,   outputSize);

    int32_t rc = test_matlib_gather(reverse,addMode,n,k,numClass,dtype,X,A,B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeX*shapeA1] = {
        1,0,0,
        0,0,4
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherTest, GatherNormal2Dby2D) {
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeX0 = 2;
    const int32_t shapeX1 = 3;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 3;
    const int32_t dtype = rindow_matlib_dtype_int32;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;

    int32_t X[shapeX0*shapeX1] = {
        2, 1, 0,
        1, 2, 3,
    };
    TypeParam A[shapeA0*shapeA1] = {
        1, 0, 0,
        0, 2, 0,
        0, 0, 3,
        4, 0, 0
    };
    calcShapes({shapeX0,shapeX1},{shapeA0,shapeA1},&m,&n,&k,&numClass,&outputSize);
    TypeParam B[shapeX0*shapeX1*shapeA1];
    
    ASSERT_EQ(1,                m);
    ASSERT_EQ(shapeX0*shapeX1,  n);
    ASSERT_EQ(shapeA1,          k);
    ASSERT_EQ(shapeA0,          numClass);
    ASSERT_EQ(shapeX0*shapeX1*shapeA1,  outputSize);

    int32_t rc = test_matlib_gather(reverse,addMode,n,k,numClass,dtype,X,A,B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeX0*shapeX1*shapeA1] = {
        0,0,3,
        0,2,0,
        1,0,0,

        0,2,0,
        0,0,3,
        4,0,0,
    };
    EXPECT_THAT(R1, ContainerEq(B));
}

}
