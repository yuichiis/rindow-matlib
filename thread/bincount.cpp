#include "rindow/matlib.h"
#include "common.hpp"

using rindow::matlib::ParallelOperation;

namespace {

template <typename TI, typename TO>
class Bincount
{
private:
    static void kernel(
        ParallelOperation::cellInfo cell,
        int32_t trans,
        int32_t m,
        int32_t n,
        int32_t numClasses,
        const TI *input,
        TO *output
    )
    {
        if(trans == RINDOW_MATLIB_NO_TRANS) {
            // axis=-1: Parallelization unit is row i (0..m)
            // input [m,n] -> output [m,numClasses]
            for(int64_t i=cell.begin; i<cell.end; i++) {
                const TI *in  = input  + i * (int64_t)n;
                TO       *out = output + i * (int64_t)numClasses;

                for(int32_t c=0; c<numClasses; c++) {
                    out[c] = 0;
                }
                for(int32_t j=0; j<n; j++) {
                    TI v = in[j];
                    if(v >= 0 && v < (TI)numClasses) {
                        out[v]++;
                    }
                }
            }
        } else {
            // axis=0: Parallelization unit is column j (0..n)
            // input [m,n] -> output [numClasses,n]
            int64_t begin_j = cell.begin;
            int64_t end_j   = cell.end;

            for(int32_t c=0; c<numClasses; c++) {
                TO *out = output + (int64_t)c * n;
                for(int64_t j=begin_j; j<end_j; j++) {
                    out[j] = 0;
                }
            }
            // Make i the outer loop and j the inner loop for contiguous access to input
            for(int32_t i=0; i<m; i++) {
                const TI *in = input + (int64_t)i * n;
                for(int64_t j=begin_j; j<end_j; j++) {
                    TI v = in[j];
                    if(v >= 0 && v < (TI)numClasses) {
                        output[(int64_t)v * n + j]++;
                    }
                }
            }
        }
    }

public:
    static void execute(
        int32_t trans,
        int32_t m,
        int32_t n,
        int32_t numClasses,
        const TI *input,
        TO *output
    )
    {
        if(m <= 0 || n <= 0 || numClasses <= 0) {
            return;
        }
        int32_t parallel = (trans == RINDOW_MATLIB_NO_TRANS) ? m : n;

        ParallelOperation::execute(parallel, kernel, trans, m, n, numClasses, input, output);
    }
};

}

extern "C" {

void rindow_matlib_i32_bincount(
    int32_t trans, int32_t m, int32_t n, int32_t numClasses,
    const int32_t *input, int32_t *output)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Bincount<int32_t,int32_t>::execute(trans, m, n, numClasses, input, output);
    RINDOW_END_CLEAR_EXCEPTION;
}

void rindow_matlib_i64_bincount(
    int32_t trans, int32_t m, int32_t n, int32_t numClasses,
    const int64_t *input, int64_t *output)
{
    RINDOW_BEGIN_CLEAR_EXCEPTION;
    Bincount<int64_t,int64_t>::execute(trans, m, n, numClasses, input, output);
    RINDOW_END_CLEAR_EXCEPTION;
}

}