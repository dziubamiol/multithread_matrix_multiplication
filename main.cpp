#include <iostream>
#include <vector>
#include <future>

template<typename T>
concept can_mul = requires(T a, T b) { a * b; };


template<typename T>
requires can_mul<T>
auto mul_mat(std::vector<std::vector<T>> &a, std::vector<std::vector<T>> &b) {
    if (a.size() != b[0].size()) {
        std::cout << "Invalid matrix\n";
    }

    auto result = std::vector<std::vector<T>>(a[0].size(), std::vector<T>(b.size()));
    std::vector<std::future<void>> futures;

    for (int i = 0; i < a[0].size(); i++) {
        for (int j = 0; j < b.size(); j++) {
            futures.emplace_back(std::async(std::launch::async,
                                            [&a, &b, &result, i, j]() {
                                                result[i][j] = T{};
                                                for (int k = 0; k < a[0].size(); k++) {
                                                    result[i][j] += a[i][k] * b[k][j];
                                                }
                                            }));
        }
    }

    for (auto &f : futures) f.wait();

    return result;
}

void matrix() {
    auto a = std::vector<std::vector<int>>({std::vector<int>({3, 4}), std::vector<int>({3, 4})});
    auto b = a;
    auto result = mul_mat(a, b);
    std::cout << result[0][0] << std::endl;
}

int main() {
    matrix();

    return 0;
}
