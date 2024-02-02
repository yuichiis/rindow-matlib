#include <iostream>
#include <iomanip>
#include <rindow/matlib.h>

void printMatrix(int m, int n, void *matrix)
{
    float *v = (float *)matrix;
    for(int i=0; i<m; i++) {
        for(int j=0; j<n; j++) {
            std::cout << std::setw(4) << v[i*n+j] << ",";
        }
        std::cout << std::endl;
    }
}
int main(int ac, char **av)
{
    const int M = 2;
    const int N = 3;
    // float
    float sX[N] = {1, 2, 3};
    float sY[M][N] = {{1,10,100}, {-1,-10,-100}};
    float alpha = 2.0;
    std::cout << "X:" << std::endl;
    printMatrix(1, N, sX);
    std::cout << std::endl << "Y:" << std::endl;
    printMatrix(M, N, sY);
    std::cout << std::endl << "alpha:" << std::endl;
    std::cout << std::setw(4) << alpha << std::endl;
    rindow_matlib_s_add(
        RINDOW_MATLIB_NO_TRANS, // int32_t trans,
        M,      // int32_t m,
        N,      // int32_t n,
        alpha,    // float alpha,
        (float *)&sX,     // float *x,
        1,      // int32_t incX,
        (float *)&sY,     // float *a,
        N       // int32_t ldA
    );
    std::cout << std::endl << "Results:" << std::endl;
    printMatrix(M, N, sY);

    switch(rindow_matlib_common_get_parallel()) {
        case RINDOW_MATLIB_SEQUENTIAL: {
            std::cout << "SEQUENTIAL" << std::endl;
            break;
        }
        case RINDOW_MATLIB_THREAD: {
            std::cout << "THREAD" << std::endl;
            break;
        }
        case RINDOW_MATLIB_OPENMP: {
            std::cout << "OPENMP" << std::endl;
            break;
        } default {
            std::cout << "UNKOWN" << std::endl;
            break;
        }
    }
    return 0;
}
