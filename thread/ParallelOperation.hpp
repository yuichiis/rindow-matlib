#ifndef RINDOW_MATLIB_PARALLELOPERATION_H_
#define RINDOW_MATLIB_PARALLELOPERATION_H_

#include <vector>
#include <exception>
#include <stdexcept>
#include "ThreadPool.hpp"

namespace rindow {
namespace matlib {


template<typename T>
using ParallelResults = std::vector<std::future<T>>;

class ParallelOperation {
public:
    template<typename R,typename F, typename... Args>
    static void enqueue(
        int32_t size,
        R& results,
        F&& kernel, Args&&... args
    )
    {
        ThreadPool& pool = ThreadPool::getInstance();
        int32_t max_threads = (int32_t)pool.getMaxThreads();
        int32_t num_thread;
        if(size < max_threads) {
            num_thread = size;
        } else {
            num_thread = max_threads;
        }
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

    template<typename F, typename... Args>
    static void execute(
        int32_t size,
        F&& kernel, Args&&... args
    )
    {
        ParallelResults<void> results;
        ParallelOperation::enqueue(size, results, kernel, args...);
        std::exception_ptr ep = nullptr;
        for(auto && result: results) {
            try {
                result.get();
            } catch(...) {
                if(ep!=nullptr) {
                    ep = std::current_exception();
                }
            }
        }
        if(ep!=nullptr) {
            std::rethrow_exception(ep);
        }
    }

    template<typename R,typename F, typename... Args>
    static R reduceNotZero(
        int32_t size,
        F&& kernel, Args&&... args
    )
    {
        R resultValue = 0;
    
        ParallelResults<R> results;
        ParallelOperation::enqueue(size, results, kernel, args...);
        std::exception_ptr ep = nullptr;
        for(auto && result: results) {
            try {
                R res = result.get();
                if(resultValue!=0) {
                    resultValue = res;
                }
            } catch(...) {
                if(ep!=nullptr) {
                    ep = std::current_exception();
                }
            }
        }
        if(ep!=nullptr) {
            std::rethrow_exception(ep);
        }

        return resultValue;
    }

    template<typename R,typename F, typename... Args>
    static R reduceSum(
        int32_t size,
        F&& kernel, Args&&... args
    )
    {
        R resultValue = 0;
    
        ParallelResults<R> results;
        ParallelOperation::enqueue(size, results, kernel, args...);
        std::exception_ptr ep = nullptr;
        for(auto && result: results) {
            try {
                R res = result.get();
                resultValue += res;
            } catch(...) {
                if(ep!=nullptr) {
                    ep = std::current_exception();
                }
            }
        }
        if(ep!=nullptr) {
            std::rethrow_exception(ep);
        }

        return resultValue;
    }
};

}
}

#endif // RINDOW_MATLIB_PARALLELOPERATION_H_