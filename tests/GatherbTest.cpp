#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include <stdbool.h>
#include <numeric>

using testing::ContainerEq;

namespace {

template <typename T>
class GatherbTest : public ::testing::Test {
protected:
    int32_t calcGatherbShapes(
        std::initializer_list<int32_t> shapeA,
        std::initializer_list<int32_t> shapeX,
        int32_t axis,
        int32_t *m,int32_t *n,int32_t *k,int32_t *numClass, int32_t *outputSize
        )
    {
        int32_t ndimBatch = (int32_t)shapeX.size();
        int32_t ndimA = (int32_t)shapeA.size();
        if(axis<0) {
            return -1;
        }
        if(ndimBatch==0||ndimA==0) {
            return -2;
        }
        if(ndimA<=ndimBatch) {
            return -3;
        }
        *m = std::accumulate(shapeA.begin(), shapeA.begin()+ndimBatch, 1, std::multiplies<int32_t>());
        if(*m!=std::accumulate(shapeX.begin(), shapeX.end(), 1, std::multiplies<int32_t>())) {
            return -4;
        }
        if(ndimA<ndimBatch+axis+1) {
            return -5;
        }
        *n = std::accumulate(shapeA.begin()+ndimBatch, shapeA.begin()+ndimBatch+axis, 1, std::multiplies<int32_t>());
        *numClass = std::accumulate(shapeA.begin()+ndimBatch+axis, shapeA.begin()+ndimBatch+axis+1, 1, std::multiplies<int32_t>());
        *k = std::accumulate(shapeA.begin()+ndimBatch+axis+1, shapeA.end(), 1, std::multiplies<int32_t>());
        *outputSize = (*m)*(*n)*(*k);
        return 0;
    }   

    int32_t calcScatterbShapes(
        std::initializer_list<int32_t> shapeX,
        std::initializer_list<int32_t> shapeA,
        int32_t numClass,
        int32_t axis,
        int32_t *m,int32_t *n,int32_t *k, int32_t *outputSize
        )
    {
        int32_t ndimBatch = (int32_t)shapeX.size();
        int32_t ndimA = (int32_t)shapeA.size();
        if(axis<0) {
            return -1;
        }
        if(ndimBatch==0||ndimA==0) {
            return -2;
        }
        if(ndimA<ndimBatch+axis) {
            return -3;
        }

        *m = std::accumulate(shapeA.begin(), shapeA.begin()+ndimBatch, 1, std::multiplies<int32_t>());
        if(*m!=std::accumulate(shapeX.begin(), shapeX.end(), 1, std::multiplies<int32_t>())) {
            return -4;
        }
        *n = std::accumulate(shapeA.begin()+ndimBatch, shapeA.begin()+ndimBatch+axis, 1, std::multiplies<int32_t>());
        *k = std::accumulate(shapeA.begin()+ndimBatch+axis, shapeA.end(), 1, std::multiplies<int32_t>());
        *outputSize = (*m)*(*n)*numClass*(*k);

        return 0;
    }   

    virtual int32_t test_matlib_gatherb(
        int32_t reverse, int32_t addMode, int32_t m, int32_t n, int32_t k,
        int32_t numClass,
        float *a, int32_t *x, float *b
    )
    {
        return rindow_matlib_s_gatherb(reverse,addMode,m,n,k,numClass,a,x,b);
    }

    virtual int32_t test_matlib_gatherb(
        int32_t reverse, int32_t addMode, int32_t m, int32_t n, int32_t k,
        int32_t numClass,
        double *a, int32_t *x, double *b
    )
    {
        return rindow_matlib_d_gatherb(reverse,addMode,m,n,k,numClass,a,x,b);
    }

    virtual int32_t test_matlib_gatherb(
        int32_t reverse, int32_t addMode, int32_t m, int32_t n, int32_t k,
        int32_t numClass,
        int32_t *a, int32_t *x, int32_t *b
    )
    {
        int32_t dtype = rindow_matlib_dtype_int32;
        return rindow_matlib_i_gatherb(reverse,addMode,m,n,k,numClass,dtype,a,x,b);
    }

};
typedef ::testing::Types<float, double, int32_t> TestTypes;
TYPED_TEST_SUITE(GatherbTest, TestTypes);

TYPED_TEST(GatherbTest, GatherbNormal2Dby1DAxis0) {
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 3;
    const int32_t shapeX = 4;
    const int32_t axis = 0;
    const int32_t shapeB = 4;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;

    TypeParam A[shapeA0][shapeA1] = {
        {0,0,3},
        {0,0,4},
        {0,2,0},
        {1,0,0}
    };
    int32_t X[shapeX] = {2,2,1,0};
    this->calcGatherbShapes({shapeA0,shapeA1},{shapeX},axis,&m,&n,&k,&numClass,&outputSize);
    TypeParam B[shapeB];
    
    ASSERT_EQ(4,        m);
    ASSERT_EQ(1,        n);
    ASSERT_EQ(shapeA1,  numClass);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(shapeX,   outputSize);

    int32_t rc = this->test_matlib_gatherb(reverse,addMode,m,n,k,numClass,(TypeParam*)A,X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB] = {3,4,2,1};
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, GatherbNormal3Dby1DAxis0) {
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 3;
    const int32_t shapeA2 = 2;
    const int32_t shapeX  = 4;
    const int32_t axis = 0;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 2;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;

    TypeParam A[shapeA0][shapeA1][shapeA2] = {
        {{0,0}, {0,0}, {3,3}},
        {{0,0}, {0,0}, {4,4}},
        {{0,0}, {2,2}, {0,0}},
        {{1,1}, {0,0}, {0,0}}
    };
    int32_t X[shapeX] = {2,2,1,0};
    this->calcGatherbShapes({shapeA0,shapeA1,shapeA2},{shapeX},axis,&m,&n,&k,&numClass,&outputSize);
    TypeParam B[shapeB0][shapeB1];

    ASSERT_EQ(shapeX,           m);
    ASSERT_EQ(1,                n);
    ASSERT_EQ(shapeA1,          numClass);
    ASSERT_EQ(shapeA2,          k);
    ASSERT_EQ(shapeB0*shapeB1,  outputSize);

    int32_t rc = this->test_matlib_gatherb(reverse,addMode,m,n,k,numClass,(TypeParam*)A,X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1] = {
        {3,3},
        {4,4},
        {2,2},
        {1,1}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}

TYPED_TEST(GatherbTest, GatherbNormal3Dby1DAxis1) {
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 3;
    const int32_t shapeX = 4;
    const int32_t axis = 1;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 2;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;

    TypeParam A[shapeA0][shapeA1][shapeA2] = {
        {{ 0, 0, 3},
         { 0, 0,13}},
        {{ 0, 0, 4},
         { 0, 0,14}},
        {{ 0, 2, 0},
         { 0,12, 0}},
        {{ 1, 0, 0},
         {11, 0, 0}}
    };
    int32_t X[shapeX] = {2,2,1,0};
    this->calcGatherbShapes({shapeA0,shapeA1,shapeA2},{shapeX},axis,&m,&n,&k,&numClass,&outputSize);
    TypeParam B[shapeB0][shapeB1];
    
    ASSERT_EQ(shapeX,       m);
    ASSERT_EQ(shapeA1,      n);
    ASSERT_EQ(shapeA2,      numClass);
    ASSERT_EQ(1,            k);
    ASSERT_EQ(shapeX*shapeA1,   outputSize);

    int32_t rc = this->test_matlib_gatherb(reverse,addMode,m,n,k,numClass,(TypeParam*)A,X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1] = {
        {3,13},
        {4,14},
        {2,12},
        {1,11}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, GatherbNormal4Dby1DAxis1) {
    const int32_t reverse = false; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 3;
    const int32_t shapeA3 = 2;
    const int32_t shapeX  = 4;
    const int32_t axis = 1;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 2;
    const int32_t shapeB2 = 2;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t numClass;
    int32_t outputSize;

    TypeParam A[shapeA0][shapeA1][shapeA2][shapeA3] = {
        {{{ 0, 0},{ 0, 0},{ 3, 3}},
         {{ 0, 0},{ 0, 0},{13,13}}},
        {{{ 0, 0},{ 0, 0},{ 4, 4}},
         {{ 0, 0},{ 0, 0},{14,14}}},
        {{{ 0, 0},{ 2, 2},{ 0, 0}},
         {{ 0, 0},{12,12},{ 0, 0}}},
        {{{ 1, 1},{ 0, 0},{ 0, 0}},
         {{11,11},{ 0, 0},{ 0, 0}}}
    };
    int32_t X[shapeX] = {2,2,1,0};
    this->calcGatherbShapes({shapeA0,shapeA1,shapeA2,shapeA3},{shapeX},axis,&m,&n,&k,&numClass,&outputSize);
    TypeParam B[shapeB0][shapeB1][shapeB2];
    
    ASSERT_EQ(shapeX,   m);
    ASSERT_EQ(shapeA1,  n);
    ASSERT_EQ(shapeA2,  numClass);
    ASSERT_EQ(shapeA3,  k);
    ASSERT_EQ(shapeB0*shapeB1*shapeB2,  outputSize);

    int32_t rc = this->test_matlib_gatherb(reverse,addMode,m,n,k,numClass,(TypeParam*)A,X,(TypeParam*)B);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2] = {
        {{3,3},{13,13}},
        {{4,4},{14,14}},
        {{2,2},{12,12}},
        {{1,1},{11,11}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, ScatterbNormal1Dby1DAxis0) {
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t shapeA = 4;
    const int32_t shapeX = 4;
    const int32_t numClass = 3;
    const int32_t axis = 0;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 3;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;

    int32_t X[shapeX] = {0,1,2,0};
    TypeParam A[shapeA] = {1,2,3,4};
    TypeParam B[shapeB0][shapeB1] = {
        {0,0,0},
        {0,0,0},
        {0,0,0},
        {0,0,0}
    };
    this->calcScatterbShapes({shapeX},{shapeA},numClass,axis,&m,&n,&k,&outputSize);
    
    ASSERT_EQ(shapeX,   m);
    ASSERT_EQ(1,        n);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(shapeB0*shapeB1, outputSize);

    int32_t rc = this->test_matlib_gatherb(reverse,addMode,m,n,k,numClass,(TypeParam*)B,X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1] = {
        {1,0,0},
        {0,2,0},
        {0,0,3},
        {4,0,0}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, ScatterbNormal2Dby1DAxis0) {
    const int32_t reverse = true; 
    const int32_t addMode = false;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 2;
    const int32_t shapeX = 4;
    const int32_t numClass = 3;
    const int32_t axis = 0;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 3;
    const int32_t shapeB2 = 2;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;

    TypeParam A[shapeA0][shapeA1] = {
        {3,3},
        {4,4},
        {2,2},
        {1,1}
    };
    int32_t X[shapeX] = {2,2,1,0};
    TypeParam B[shapeB0][shapeB1][shapeB2] = {
        {{0,0}, {0,0}, {0,0}},
        {{0,0}, {0,0}, {0,0}},
        {{0,0}, {0,0}, {0,0}},
        {{0,0}, {0,0}, {0,0}}
    };
    this->calcScatterbShapes({shapeX},{shapeA0,shapeA1},numClass,axis,&m,&n,&k,&outputSize);
    
    ASSERT_EQ(shapeX,       m);
    ASSERT_EQ(1,            n);
    ASSERT_EQ(shapeA1,      k);
    ASSERT_EQ(shapeB0*shapeB1*shapeB2, outputSize);

    int32_t rc = this->test_matlib_gatherb(reverse,addMode,m,n,k,numClass,(TypeParam*)B,X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2] = {
        {{0,0}, {0,0}, {3,3}},
        {{0,0}, {0,0}, {4,4}},
        {{0,0}, {2,2}, {0,0}},
        {{1,1}, {0,0}, {0,0}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, testScatterbNormal2Dby1DAxis1) {
    const int32_t reverse = true;
    const int32_t addMode = false;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 2;
    const int32_t shapeX = 4;
    const int32_t numClass = 3;
    const int32_t axis = 1;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 2;
    const int32_t shapeB2 = 3;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;
    int32_t rc;

    TypeParam A[shapeA0][shapeA1] = {
        {3,13},
        {4,14},
        {2,12},
        {1,11}
    };
    int32_t X[shapeX] = {2,2,1,0};
    TypeParam B[shapeB0][shapeB1][shapeB2] = {
        {{ 0, 0, 0},
         { 0, 0, 0}},
        {{ 0, 0, 0},
         { 0, 0, 0}},
        {{ 0, 0, 0},
         { 0, 0, 0}},
        {{ 0, 0, 0},
         { 0, 0, 0}}
    };
    rc = this->calcScatterbShapes({shapeX},{shapeA0,shapeA1},numClass,axis,&m,&n,&k,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(shapeX,   m);
    ASSERT_EQ(shapeA1,  n);
    ASSERT_EQ(1,        k);
    ASSERT_EQ(shapeB0*shapeB1*shapeB2, outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,m,n,k,numClass,(TypeParam*)B,X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2] = {
        {{ 0, 0, 3},
         { 0, 0,13}},
        {{ 0, 0, 4},
         { 0, 0,14}},
        {{ 0, 2, 0},
         { 0,12, 0}},
        {{ 1, 0, 0},
         {11, 0, 0}}
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
TYPED_TEST(GatherbTest, testScatterbNormal3Dby1DAxis1) {
    const int32_t reverse = true;
    const int32_t addMode = false;
    const int32_t shapeA0 = 4;
    const int32_t shapeA1 = 2;
    const int32_t shapeA2 = 2;
    const int32_t shapeX = 4;
    const int32_t numClass = 3;
    const int32_t axis = 1;
    const int32_t shapeB0 = 4;
    const int32_t shapeB1 = 2;
    const int32_t shapeB2 = 3;
    const int32_t shapeB3 = 2;
    int32_t m;
    int32_t n;
    int32_t k;
    int32_t outputSize;
    int32_t rc;

    int32_t X[shapeX] = {2,2,1,0};
    TypeParam A[shapeA0][shapeA1][shapeA2] = {
        {{3,3},{13,13}},
        {{4,4},{14,14}},
        {{2,2},{12,12}},
        {{1,1},{11,11}}        
    };
    TypeParam B[shapeB0][shapeB1][shapeB2][shapeB3] = {
        {{{ 0, 0},{ 0, 0},{ 0, 0}},
         {{ 0, 0},{ 0, 0},{ 0, 0}}},
        {{{ 0, 0},{ 0, 0},{ 0, 0}},
         {{ 0, 0},{ 0, 0},{ 0, 0}}},
        {{{ 0, 0},{ 0, 0},{ 0, 0}},
         {{ 0, 0},{ 0, 0},{ 0, 0}}},
        {{{ 0, 0},{ 0, 0},{ 0, 0}},
         {{ 0, 0},{ 0, 0},{ 0, 0}}}
    };
    rc = this->calcScatterbShapes({shapeX},{shapeA0,shapeA1,shapeA2},numClass,axis,&m,&n,&k,&outputSize);
    ASSERT_EQ(0,rc);
    
    ASSERT_EQ(shapeX,   m);
    ASSERT_EQ(shapeA1,  n);
    ASSERT_EQ(shapeA2,  k);
    ASSERT_EQ(shapeB0*shapeB1*shapeB2*shapeB3,   outputSize);

    rc = this->test_matlib_gatherb(reverse,addMode,m,n,k,numClass,(TypeParam*)B,X,(TypeParam*)A);
    ASSERT_EQ(0,rc);

    TypeParam R1[shapeB0][shapeB1][shapeB2][shapeB3] = {
        {{{ 0, 0},{ 0, 0},{ 3, 3}},
         {{ 0, 0},{ 0, 0},{13,13}}},
        {{{ 0, 0},{ 0, 0},{ 4, 4}},
         {{ 0, 0},{ 0, 0},{14,14}}},
        {{{ 0, 0},{ 2, 2},{ 0, 0}},
         {{ 0, 0},{12,12},{ 0, 0}}},
        {{{ 1, 1},{ 0, 0},{ 0, 0}},
         {{11,11},{ 0, 0},{ 0, 0}}},
    };
    EXPECT_THAT(R1, ContainerEq(B));
}
}
