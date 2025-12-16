/**
 * @file result.h
 * @brief Result type for error handling without exceptions
 * 
 * Provides a Result<T, E> template for representing either a successful
 * value or an error, similar to Rust's Result or C++23's std::expected.
 */

#ifndef FFVMS_CORE_RESULT_H
#define FFVMS_CORE_RESULT_H

#include <variant>
#include <string>
#include <stdexcept>
#include <utility>

namespace ffvms {

/**
 * @brief A type representing either a success value or an error
 * 
 * @tparam T The success value type
 * @tparam E The error type (defaults to std::string)
 * 
 * Usage:
 * @code
 * Result<int> divide(int a, int b) {
 *     if (b == 0) return Result<int>::Error("Division by zero");
 *     return Result<int>::Ok(a / b);
 * }
 * 
 * auto result = divide(10, 2);
 * if (result.is_ok()) {
 *     std::cout << result.value();
 * }
 * @endcode
 */
template<typename T, typename E = std::string>
class Result {
public:
    /// @brief Check if result contains a success value
    [[nodiscard]] bool is_ok() const noexcept {
        return std::holds_alternative<T>(data_);
    }

    /// @brief Check if result contains an error
    [[nodiscard]] bool is_error() const noexcept {
        return std::holds_alternative<E>(data_);
    }

    /// @brief Get the success value (throws if error)
    [[nodiscard]] T& value() & {
        if (is_error()) {
            throw std::runtime_error("Attempted to access value of error Result");
        }
        return std::get<T>(data_);
    }

    /// @brief Get the success value (throws if error)
    [[nodiscard]] const T& value() const& {
        if (is_error()) {
            throw std::runtime_error("Attempted to access value of error Result");
        }
        return std::get<T>(data_);
    }

    /// @brief Get the success value with move semantics
    [[nodiscard]] T&& value() && {
        if (is_error()) {
            throw std::runtime_error("Attempted to access value of error Result");
        }
        return std::get<T>(std::move(data_));
    }

    /// @brief Get the error (throws if success)
    [[nodiscard]] E& error() & {
        if (is_ok()) {
            throw std::runtime_error("Attempted to access error of success Result");
        }
        return std::get<E>(data_);
    }

    /// @brief Get the error (throws if success)
    [[nodiscard]] const E& error() const& {
        if (is_ok()) {
            throw std::runtime_error("Attempted to access error of success Result");
        }
        return std::get<E>(data_);
    }

    /// @brief Get value or return a default
    [[nodiscard]] T value_or(T default_value) const& {
        if (is_ok()) {
            return std::get<T>(data_);
        }
        return default_value;
    }

    /// @brief Create a success Result
    [[nodiscard]] static Result Ok(T value) {
        Result result;
        result.data_ = std::move(value);
        return result;
    }

    /// @brief Create an error Result
    [[nodiscard]] static Result Error(E error) {
        Result result;
        result.data_ = std::move(error);
        return result;
    }

    /// @brief Implicit conversion to bool (true if ok)
    explicit operator bool() const noexcept {
        return is_ok();
    }

private:
    std::variant<T, E> data_;
};

/**
 * @brief Specialization for void success type
 * 
 * Used when a function only needs to indicate success/failure without
 * returning a value.
 */
template<typename E>
class Result<void, E> {
public:
    [[nodiscard]] bool is_ok() const noexcept { return !has_error_; }
    [[nodiscard]] bool is_error() const noexcept { return has_error_; }

    [[nodiscard]] const E& error() const& {
        if (!has_error_) {
            throw std::runtime_error("Attempted to access error of success Result");
        }
        return error_;
    }

    [[nodiscard]] static Result Ok() {
        Result result;
        result.has_error_ = false;
        return result;
    }

    [[nodiscard]] static Result Error(E error) {
        Result result;
        result.has_error_ = true;
        result.error_ = std::move(error);
        return result;
    }

    explicit operator bool() const noexcept { return is_ok(); }

private:
    bool has_error_ = false;
    E error_;
};

}  // namespace ffvms

#endif // FFVMS_CORE_RESULT_H
