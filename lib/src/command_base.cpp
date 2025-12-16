/**
 * @file command_base.cpp
 * @brief Base command implementation with common utilities
 */

#include "interfaces/i_command.h"
#include <cctype>

namespace ffvms {

bool CommandBase::is_all_digits(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!std::isdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

unsigned long long CommandBase::str_to_ull(const std::string& s) {
    unsigned long long result = 0;
    for (char c : s) {
        result = result * 10 + (c - '0');
    }
    return result;
}

std::string CommandBase::validate_params(const std::vector<std::string>& params,
                                         const std::vector<ParamType>& requirements) const {
    if (params.size() < requirements.size()) {
        return "Parameters are insufficient. " + std::to_string(requirements.size()) + 
               " parameters were required but only " + std::to_string(params.size()) + 
               " were provided.";
    }

    for (size_t i = 0; i < requirements.size(); i++) {
        if (requirements[i] == ParamType::INT || requirements[i] == ParamType::ULL) {
            if (!is_all_digits(params[i])) {
                return "The " + std::to_string(i + 1) + "th parameter must be an integer.";
            }
            if (requirements[i] == ParamType::INT && params[i].size() > 9) {
                return "The " + std::to_string(i + 1) + "th argument has a maximum of 9 digits.";
            }
            if (requirements[i] == ParamType::ULL && params[i].size() > 18) {
                return "The " + std::to_string(i + 1) + "th argument has a maximum of 18 digits.";
            }
        }
    }

    return "";  // Valid
}

}  // namespace ffvms
