#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rindow/matlib.h"
#include "rindow/ndarray.h"
#include <stdbool.h>

using testing::ContainerEq;
using rindow::math::NDArray;
using rindow::math::ndarray_t;

namespace {

template <typename T>
class ImagecopyTest : public ::testing::Test {
protected:
    virtual void test_matlib_imagecopy(
        int32_t height,
        int32_t width,
        int32_t channels,
        float *a,
        float *b,
        int32_t channelsFirst,
        int32_t heightShift,
        int32_t widthShift,
        int32_t verticalFlip,
        int32_t horizontalFlip,
        int32_t rgbFlip
    ) {
        rindow_matlib_s_imagecopy(
            height,
            width,
            channels,
            a,
            b,
            channelsFirst,
            heightShift,
            widthShift,
            verticalFlip,
            horizontalFlip,
            rgbFlip
        );
    }
    virtual void test_matlib_imagecopy(
        int32_t height,
        int32_t width,
        int32_t channels,
        double *a,
        double *b,
        int32_t channelsFirst,
        int32_t heightShift,
        int32_t widthShift,
        int32_t verticalFlip,
        int32_t horizontalFlip,
        int32_t rgbFlip
    ) {
        rindow_matlib_d_imagecopy(
            height,
            width,
            channels,
            a,
            b,
            channelsFirst,
            heightShift,
            widthShift,
            verticalFlip,
            horizontalFlip,
            rgbFlip
        );
    }
    virtual void test_matlib_imagecopy(
        int32_t height,
        int32_t width,
        int32_t channels,
        uint8_t *a,
        uint8_t *b,
        int32_t channelsFirst,
        int32_t heightShift,
        int32_t widthShift,
        int32_t verticalFlip,
        int32_t horizontalFlip,
        int32_t rgbFlip
    ) {
        rindow_matlib_i8_imagecopy(
            height,
            width,
            channels,
            a,
            b,
            channelsFirst,
            heightShift,
            widthShift,
            verticalFlip,
            horizontalFlip,
            rgbFlip
        );
    }
};
typedef ::testing::Types<float, double, uint8_t> TestTypes;
TYPED_TEST_SUITE(ImagecopyTest, TestTypes);

TYPED_TEST(ImagecopyTest, Simple) {
    const int32_t channelsFirst  = false;
    int32_t height;
    int32_t width;
    int32_t channels;

    ndarray_t<TypeParam> a = 
        NDArray<TypeParam>::range((TypeParam)(3*3*1))->reshape({3,3,1});
    ndarray_t<TypeParam> b = NDArray<TypeParam>::zeros({3,3,1});

    if(channelsFirst==false) {
        height = a->shape()[0];
        width = a->shape()[1];
        channels = a->shape()[2];
    } else {
        channels = a->shape()[0];
        height = a->shape()[1];
        width = a->shape()[2];
    }

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        1,      // heightShift,
        0,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X1[3*3*1];
    std::copy_n(b->buffer()->begin(),3*3*1,X1);

    TypeParam R1[3*3*1] = {
        0,1,2,
        0,1,2,
        3,4,5
    };
    EXPECT_THAT(R1, ContainerEq(X1));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        -1,     // heightShift,
        0,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X2[3*3*1];
    std::copy_n(b->buffer()->begin(),3*3*1,X2);

    TypeParam R2[3*3*1] = {
        3,4,5,
        6,7,8,
        6,7,8
    };
    EXPECT_THAT(R2, ContainerEq(X2));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        0,      // heightShift,
        1,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X3[3*3*1];
    std::copy_n(b->buffer()->begin(),3*3*1,X3);

    TypeParam R3[3*3*1] = {
        0,0,1,
        3,3,4,
        6,6,7
    };
    EXPECT_THAT(R3, ContainerEq(X3));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        0,      // heightShift,
        -1,     // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X4[3*3*1];
    std::copy_n(b->buffer()->begin(),3*3*1,X4);

    TypeParam R4[3*3*1] = {
        1,2,2,
        4,5,5,
        7,8,8
    };
    EXPECT_THAT(R4, ContainerEq(X4));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        1,      // heightShift,
        1,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X5[3*3*1];
    std::copy_n(b->buffer()->begin(),3*3*1,X5);

    TypeParam R5[3*3*1] = {
        0,0,1,
        0,0,1,
        3,3,4
    };
    EXPECT_THAT(R5, ContainerEq(X5));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        -1,     // heightShift,
        -1,     // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X6[3*3*1];
    std::copy_n(b->buffer()->begin(),3*3*1,X6);

    TypeParam R6[3*3*1] = {
        4,5,5,
        7,8,8,
        7,8,8
    };
    EXPECT_THAT(R6, ContainerEq(X6));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        0,      // heightShift,
        0,      // widthShift,
        true,   // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X7[3*3*1];
    std::copy_n(b->buffer()->begin(),3*3*1,X7);

    TypeParam R7[3*3*1] = {
        6,7,8,
        3,4,5,
        0,1,2
    };
    EXPECT_THAT(R7, ContainerEq(X7));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        0,      // heightShift,
        0,      // widthShift,
        false,  // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X8[3*3*1];
    std::copy_n(b->buffer()->begin(),3*3*1,X8);

    TypeParam R8[3*3*1] = {
        2,1,0,
        5,4,3,
        8,7,6
    };
    EXPECT_THAT(R8, ContainerEq(X8));
}
TYPED_TEST(ImagecopyTest, Complex) {
    const int32_t channelsFirst  = false;
    int32_t height;
    int32_t width;
    int32_t channels;

    ndarray_t<TypeParam> a = 
        NDArray<TypeParam>::range((TypeParam)(4*4*1))->reshape({4,4,1});
    ndarray_t<TypeParam> b = NDArray<TypeParam>::zeros({4,4,1});

    if(channelsFirst==false) {
        height = a->shape()[0];
        width = a->shape()[1];
        channels = a->shape()[2];
    } else {
        channels = a->shape()[0];
        height = a->shape()[1];
        width = a->shape()[2];
    }

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        1,      // heightShift,
        0,      // widthShift,
        true,   // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X1[4*4*1];
    std::copy_n(b->buffer()->begin(),4*4*1,X1);

    TypeParam R1[4*4*1] = {
        12, 13, 14, 15,
        12, 13, 14, 15,
        8,  9,  10, 11,
        4,  5,   6,  7
    };
    EXPECT_THAT(R1, ContainerEq(X1));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        -1,     // heightShift,
        0,      // widthShift,
        true,   // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X2[4*4*1];
    std::copy_n(b->buffer()->begin(),4*4*1,X2);

    TypeParam R2[4*4*1] = {
        8, 9, 10, 11,
        4, 5, 6,  7,
        0, 1, 2,  3,
        0, 1, 2,  3
    };
    EXPECT_THAT(R2, ContainerEq(X2));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        0,      // heightShift,
        1,      // widthShift,
        false,  // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X3[4*4*1];
    std::copy_n(b->buffer()->begin(),4*4*1,X3);

    TypeParam R3[4*4*1] = {
         3,  3,  2,  1,
         7,  7,  6,  5,
        11, 11, 10,  9,
        15, 15, 14, 13
    };
    EXPECT_THAT(R3, ContainerEq(X3));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        0,      // heightShift,
        -1,     // widthShift,
        false,  // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X4[4*4*1];
    std::copy_n(b->buffer()->begin(),4*4*1,X4);

    TypeParam R4[4*4*1] = {
         2,  1,  0,  0,
         6,  5,  4,  4,
        10,  9,  8,  8,
        14, 13, 12, 12
    };
    EXPECT_THAT(R4, ContainerEq(X4));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        0,      // heightShift,
        0,      // widthShift,
        true,   // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X5[4*4*1];
    std::copy_n(b->buffer()->begin(),4*4*1,X5);

    TypeParam R5[4*4*1] = {
        15, 14, 13, 12,
        11, 10,  9,  8,
         7,  6 , 5,  4,
         3,  2 , 1,  0
    };
    EXPECT_THAT(R5, ContainerEq(X5));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        1,      // heightShift,
        0,      // widthShift,
        true,   // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X6[4*4*1];
    std::copy_n(b->buffer()->begin(),4*4*1,X6);

    TypeParam R6[4*4*1] = {
        15, 14, 13, 12,
        15, 14, 13, 12,
        11, 10,  9,  8,
         7,  6,  5,  4
    };
    EXPECT_THAT(R6, ContainerEq(X6));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        -1,     // heightShift,
        0,      // widthShift,
        true,   // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X7[4*4*1];
    std::copy_n(b->buffer()->begin(),4*4*1,X7);

    TypeParam R7[4*4*1] = {
            11, 10,  9,  8,
             7,  6,  5,  4,
             3,  2,  1,  0,
             3,  2,  1,  0
    };
    EXPECT_THAT(R7, ContainerEq(X7));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        0,      // heightShift,
        1,      // widthShift,
        true,   // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X8[4*4*1];
    std::copy_n(b->buffer()->begin(),4*4*1,X8);

    TypeParam R8[4*4*1] = {
            15, 15, 14, 13,
            11, 11, 10,  9,
             7,  7,  6,  5,
             3,  3,  2,  1
    };
    EXPECT_THAT(R8, ContainerEq(X8));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        0,      // heightShift,
        -1,     // widthShift,
        true,   // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X9[4*4*1];
    std::copy_n(b->buffer()->begin(),4*4*1,X9);

    TypeParam R9[4*4*1] = {
        14,13,12,12,
        10, 9, 8, 8,
         6, 5, 4, 4,
         2, 1, 0, 0
    };
    EXPECT_THAT(R9, ContainerEq(X9));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        1,      // heightShift,
        1,      // widthShift,
        true,   // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X10[4*4*1];
    std::copy_n(b->buffer()->begin(),4*4*1,X10);

    TypeParam R10[4*4*1] = {
        15, 15, 14, 13,
        15, 15, 14, 13,
        11, 11, 10,  9,
         7,  7,  6,  5
    };
    EXPECT_THAT(R10, ContainerEq(X10));

    // ==============================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        -1,     // heightShift,
        -1,     // widthShift,
        true,   // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X11[4*4*1];
    std::copy_n(b->buffer()->begin(),4*4*1,X11);

    TypeParam R11[4*4*1] = {
        10, 9, 8, 8,
         6, 5, 4, 4,
         2, 1, 0, 0,
         2, 1, 0, 0
    };
    EXPECT_THAT(R11, ContainerEq(X11));
}
TYPED_TEST(ImagecopyTest, WithRGB) {
    const int32_t channelsFirst  = false;
    int32_t height;
    int32_t width;
    int32_t channels;

    ndarray_t<TypeParam> a = NDArray<TypeParam>::array({
        1,2,3,
        1,2,3,

        4,5,6,
        4,5,6,

        7,8,9,
        7,8,9,
    })->reshape({3,2,3});
    ndarray_t<TypeParam> b = NDArray<TypeParam>::zeros({3,2,3});

    if(channelsFirst==false) {
        height = a->shape()[0];
        width = a->shape()[1];
        channels = a->shape()[2];
    } else {
        channels = a->shape()[0];
        height = a->shape()[1];
        width = a->shape()[2];
    }

    // ============heightShift==================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        1,      // heightShift,
        0,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X1[3*2*3];
    std::copy_n(b->buffer()->begin(),3*2*3,X1);

    TypeParam R1[3*2*3] = {
        1,2,3,
        1,2,3,

        1,2,3,
        1,2,3,

        4,5,6,
        4,5,6,
    };
    EXPECT_THAT(R1, ContainerEq(X1));

    // ===========flip rgb===================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        0,      // heightShift,
        0,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        true    // rgbFlip
    );
    TypeParam X2[3*2*3];
    std::copy_n(b->buffer()->begin(),3*2*3,X2);

    TypeParam R2[3*2*3] = {
        3,2,1,
        3,2,1,

        6,5,4,
        6,5,4,

        9,8,7,
        9,8,7,
    };
    EXPECT_THAT(R2, ContainerEq(X2));
}
TYPED_TEST(ImagecopyTest, WithRGBA) {
    const int32_t channelsFirst  = false;
    int32_t height;
    int32_t width;
    int32_t channels;

    ndarray_t<TypeParam> a = NDArray<TypeParam>::array({
        1,2,3,4,
        1,2,3,4,

        4,5,6,7,
        4,5,6,7,

        7,8,9,10,
        7,8,9,10,
    })->reshape({3,2,4});
    ndarray_t<TypeParam> b = NDArray<TypeParam>::zeros({3,2,4});

    if(channelsFirst==false) {
        height = a->shape()[0];
        width = a->shape()[1];
        channels = a->shape()[2];
    } else {
        channels = a->shape()[0];
        height = a->shape()[1];
        width = a->shape()[2];
    }

    // ============flip rgb with alpha==================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        0,      // heightShift,
        0,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        true    // rgbFlip
    );
    TypeParam X1[3*2*4];
    std::copy_n(b->buffer()->begin(),3*2*4,X1);

    TypeParam R1[3*2*4] = {
        3,2,1,4,
        3,2,1,4,

        6,5,4,7,
        6,5,4,7,

        9,8,7,10,
        9,8,7,10,
    };
    EXPECT_THAT(R1, ContainerEq(X1));
}
TYPED_TEST(ImagecopyTest, channelsFirst) {
    const int32_t channelsFirst  = true;
    int32_t height;
    int32_t width;
    int32_t channels;

    ndarray_t<TypeParam> a = NDArray<TypeParam>::array({
        1,2,3,
        4,5,6,

        11,12,13,
        14,15,16,

        21,22,23,
        24,25,26,
    })->reshape({3,2,3});
    ndarray_t<TypeParam> b = NDArray<TypeParam>::zeros({3,2,3});

    if(channelsFirst==false) {
        height = a->shape()[0];
        width = a->shape()[1];
        channels = a->shape()[2];
    } else {
        channels = a->shape()[0];
        height = a->shape()[1];
        width = a->shape()[2];
    }

    // ============heightShift==================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        1,      // heightShift,
        0,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X1[3*2*3];
    std::copy_n(b->buffer()->begin(),3*2*3,X1);

    TypeParam R1[3*2*3] = {
         1, 2, 3,
         1, 2, 3,

        11,12,13,
        11,12,13,

        21,22,23,
        21,22,23,
    };
    EXPECT_THAT(R1, ContainerEq(X1));

    // ===========widthShift===================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        0,      // heightShift,
        1,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X2[3*2*3];
    std::copy_n(b->buffer()->begin(),3*2*3,X2);

    TypeParam R2[3*2*3] = {
         1, 1, 2,
         4, 4, 5,

        11,11,12,
        14,14,15,

        21,21,22,
        24,24,25,
    };
    EXPECT_THAT(R2, ContainerEq(X2));

    // ===========verticalFlip===================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        0,      // heightShift,
        0,      // widthShift,
        true,   // verticalFlip,
        false,  // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X3[3*2*3];
    std::copy_n(b->buffer()->begin(),3*2*3,X3);

    TypeParam R3[3*2*3] = {
         4, 5, 6,
         1, 2, 3,

        14,15,16,
        11,12,13,

        24,25,26,
        21,22,23,
    };
    EXPECT_THAT(R3, ContainerEq(X3));

    // ===========horizontalFlip===================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        0,      // heightShift,
        0,      // widthShift,
        false,  // verticalFlip,
        true,   // horizontalFlip,
        false   // rgbFlip
    );
    TypeParam X4[3*2*3];
    std::copy_n(b->buffer()->begin(),3*2*3,X4);

    TypeParam R4[3*2*3] = {
         3, 2, 1,
         6, 5, 4,

        13,12,11,
        16,15,14,

        23,22,21,
        26,25,24,
    };
    EXPECT_THAT(R4, ContainerEq(X4));

    // ===========rgbFlip===================
    this->test_matlib_imagecopy(
        height,
        width,
        channels,
        a->data(),
        b->data(),
        channelsFirst,
        0,      // heightShift,
        0,      // widthShift,
        false,  // verticalFlip,
        false,  // horizontalFlip,
        true    // rgbFlip
    );
    TypeParam X5[3*2*3];
    std::copy_n(b->buffer()->begin(),3*2*3,X5);

    TypeParam R5[3*2*3] = {
        21,22,23,
        24,25,26,

        11,12,13,
        14,15,16,

        1,2,3,
        4,5,6,
    };
    EXPECT_THAT(R5, ContainerEq(X5));
}
}
