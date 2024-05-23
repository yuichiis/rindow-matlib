#ifndef RINDOW_MATLIB_PARALLELOPERATION_H_
#define RINDOW_MATLIB_PARALLELOPERATION_H_

#include <vector>
#include "ThreadPool.hpp"

namespace rindow {
namespace matlib {


template<typename T>
using ParallelResults = std::vector<std::future<T>>;

class ParallelOperation {
public:
    template<typename R,typename F, typename... Args>
    static void execute(
        int32_t size,
        R& results,
        F&& kernel, Args&&... args
    )
    {
        ThreadPool& pool = ThreadPool::getInstance();
        int32_t max_threads = (int32_t)pool.getMaxThreads();

        int32_t num_thread = std::min(size, max_threads);
        int32_t cell_size = size / num_thread;
        int32_t remainder = size - cell_size * num_thread;

        for(int32_t i = 0; i < num_thread; i++) {
            int32_t begin;
            int32_t end;
            begin = i * cell_size;
            if(i == num_thread - 1) {
                end = (i+1) * cell_size + remainder;
            } else {
                end = (i+1) * cell_size;
            }
            results.emplace_back(pool.enqueue(kernel, begin, end, args...));
        }
    }
};

}
}

#endif // RINDOW_MATLIB_PARALLELOPERATION_H_