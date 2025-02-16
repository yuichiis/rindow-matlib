#include "rindow/matlib.h"
#include "common.h"
#include <vector>

namespace {

template <typename T>
class Einsum
{
private:
    static void calc_indices(
        int32_t depth,
        int32_t ndim,
        int32_t index,
        int32_t *sizeOfIndices,
        std::vector<int32_t> &indices
    )
    {
        for(int32_t axis=ndim-1; axis>=0; axis--) {
            int32_t size = sizeOfIndices[axis];
            indices[axis] = index % size;
            index /= size;
        }
    }

    static int32_t calc_index(
        int32_t depth,
        std::vector<int32_t> &indices,  
        int32_t *sizeOfIndices,
        int32_t ndim,
        int32_t *label,
        int32_t *shape
    )
    {
        int32_t index = 0;
        for(int32_t axis=0; axis<ndim; axis++) {
            int32_t idx;
            if(label==nullptr) {
                idx = axis;
            } else {
                idx = label[axis];
            }
            if(idx>=depth) {
                return -1; // label number is too large.
            }
            int32_t size;
            if(shape!=nullptr) {
                size = shape[axis]; // if broadcast then size is 1.
            } else {
                size = sizeOfIndices[idx]; // as is shape[$axis];
            }
            index *= size;
            if(size>1) {
                if(indices[idx]>=size) {
                    return -2; // indicator number is too large.
                }
                index += indices[idx];
            }
        }
        return index;
    }

    static int32_t next_indices(
        int32_t depth,
        int32_t ndim,
        int32_t *sizeOfIndices,
        std::vector<int32_t> &indices
    ) {
        int32_t i = depth - 1;
        while(i >= 0) {
            if(indices[i] < sizeOfIndices[i]-1) {
                break;
            }
            indices[i] = 0;
            i--;
        }
        if(i>=0) {
            indices[i]++;
        }
        if(i < ndim) {
            // done all
            return 0;
        }
        return 1;
    }

    static int32_t kernel(
        int32_t indexC,
        int32_t depth,
        int32_t *sizeOfIndices,
        T *a,
        int32_t ndimA,
        int32_t *labelA,
        T *b,
        int32_t ndimB,
        int32_t *labelB,
        T *c,
        int32_t ndimC,
        int32_t *shapeA,
        int32_t *shapeB
    )
    {
        std::vector<int32_t> indices(depth,0);
        calc_indices(depth,ndimC,indexC,sizeOfIndices,indices);
        T sumC = 0;
        for(;;) {
            int32_t indexA = calc_index(depth,indices,sizeOfIndices,ndimA,labelA,shapeA);
            if(indexA<0) {
                return indexA-1000;
            }
            int32_t indexB = calc_index(depth,indices,sizeOfIndices,ndimB,labelB,shapeB);
            if(indexB<0) {
                return indexB-2000;
            }
            sumC += a[indexA]*b[indexB];
            // next indices
            if(!next_indices(depth,ndimC,sizeOfIndices,indices)) {
                break;
            }
        }
        c[indexC] = sumC;
        return 0;
    }

    public:
    static int32_t execute(
        int32_t depth,
        int32_t *sizeOfIndices,
        T *a,
        int32_t ndimA,
        int32_t *labelA,
        T *b,
        int32_t ndimB,
        int32_t *labelB,
        T *c,
        int32_t ndimC,
        int32_t *shapeA,
        int32_t *shapeB
    )
    {
        if(depth<=0 || ndimA<=0 || ndimB<=0 || ndimC<=0) {
            return RINDOW_MATLIB_E_MEM_ALLOC_FAILURE;
        }
        if(depth<ndimC) {
            return RINDOW_MATLIB_E_INVALID_SHAPE_OR_PARAM;
        }
        int32_t sizeC=1;
        for(int32_t i=0;i<ndimC;i++) {
            sizeC *= sizeOfIndices[i];
        }

        int32_t errcode = 0;
        int32_t indexC;
        #pragma omp parallel for
        for(indexC=0; indexC<sizeC; indexC++) {
            int32_t ec = kernel(
                indexC,
                depth,
                sizeOfIndices,
                a,
                ndimA,
                labelA,
                b,
                ndimB,
                labelB,
                c,
                ndimC,
                shapeA,
                shapeB
            );
            if(ec) {
                errcode = ec;
            }
        }
        return errcode;
    }
};
}

extern "C" {
int32_t rindow_matlib_s_einsum(
    int32_t depth,
    int32_t *sizeOfIndices,
    float *a,
    int32_t ndimA,
    int32_t *labelA,
    float *b,
    int32_t ndimB,
    int32_t *labelB,
    float *c,
    int32_t ndimC,
    int32_t *shapeA,
    int32_t *shapeB
)
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Einsum<float>::execute(
        depth,
        sizeOfIndices,
        a,
        ndimA,
        labelA,
        b,
        ndimB,
        labelB,
        c,
        ndimC,
        shapeA,
        shapeB
    );
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}

int32_t rindow_matlib_d_einsum(
    int32_t depth,
    int32_t *sizeOfIndices,
    double *a,
    int32_t ndimA,
    int32_t *labelA,
    double *b,
    int32_t ndimB,
    int32_t *labelB,
    double *c,
    int32_t ndimC,
    int32_t *shapeA,
    int32_t *shapeB
    )
{
    int32_t errcode = RINDOW_MATLIB_E_SOME_EXCEPTION_OCCURRED;
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    errcode = Einsum<double>::execute(
        depth,
        sizeOfIndices,
        a,
        ndimA,
        labelA,
        b,
        ndimB,
        labelB,
        c,
        ndimC,
        shapeA,
        shapeB
    );
    RINDOW_END_CLEAR_EXCEPTION;
    return errcode;
}
}
