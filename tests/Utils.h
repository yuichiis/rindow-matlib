#ifndef RINDOWTEST_UTILS_H
#define RINDOWTEST_UTILS_H
#include <initializer_list>
#include <memory>
#include <type_traits>
#include <stdbool.h>

namespace RindowTest {
class Utils {
public:
    template <typename T>
    static std::unique_ptr<T[]> fill(std::initializer_list<int> sizes, T value);

    template <typename T>
    static std::unique_ptr<T[]> zeros(std::initializer_list<int> sizes);

    template <typename T>
    static std::unique_ptr<T[]> ones(std::initializer_list<int> sizes);

    template <typename T>
    static std::unique_ptr<T[]> array(std::initializer_list<T> values);

    template <typename T>
    static bool isclose(int32_t n, T *trues, T *x);

    template <typename T>
    static bool isclose(int32_t n, T *trues, T *x, T rtol);

    template <typename T>
    static bool isclose(int32_t n, T *trues, T *x, T rtol, T atol);
}; // class Utils

} // namespace RindowTest    

#endif // RINDOWTEST_UTILS_H
