/**
 * @file session.h
 * @brief Implementation of ISession
 */

#ifndef SESSION_H
#define SESSION_H

#include "interfaces/i_session.h"
#include "file_system.h"
#include <vector>
#include <string>

namespace ffvms {

class Session : public ISession {
private:
    FileSystem& file_system_;
    std::vector<std::string> current_path_;
    std::vector<std::string> previous_path_;

public:
    explicit Session(FileSystem& fs);
    ~Session() override = default;

    FileSystem& get_file_system() override;
    std::vector<std::string> get_current_path() const override;
    std::string get_current_path_string() const override;
    bool set_current_path(const std::vector<std::string>& path) override;
    std::vector<std::string> get_previous_path() const override;
    
    // Helper to resolve relative path against current path
    std::vector<std::string> resolve_path(const std::vector<std::string>& input_path) const;
};

} // namespace ffvms

#endif // SESSION_H
