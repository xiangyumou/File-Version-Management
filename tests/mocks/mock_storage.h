#ifndef MOCK_STORAGE_H
#define MOCK_STORAGE_H

#include "gmock/gmock.h"
#include "interfaces/i_storage.h"

namespace ffvms {
namespace test {

class MockStorage : public IStorage {
public:
    MOCK_METHOD(bool, save, (const std::string&, const DataTable&), (override));
    MOCK_METHOD(bool, load, (const std::string&, DataTable&, bool), (override));
};

} // namespace test
} // namespace ffvms
#endif // MOCK_STORAGE_H
