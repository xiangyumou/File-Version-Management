#ifndef MOCK_LOGGER_H
#define MOCK_LOGGER_H

#include "gmock/gmock.h"
#include "interfaces/i_logger.h"

namespace ffvms {
namespace test {

class MockLogger : public ILogger {
public:
    MOCK_METHOD(void, log, (const std::string&, LogLevel, int), (override));
    MOCK_METHOD(const std::string&, get_last_message, (), (const, override));
    MOCK_METHOD(void, set_information, (const std::string&), (override));
    MOCK_METHOD(const std::string&, get_information, (), (const, override));
};

} // namespace test
} // namespace ffvms

#endif // MOCK_LOGGER_H
