#ifndef PWD_COMMAND_H
#define PWD_COMMAND_H

#include "interfaces/i_command.h"

namespace ffvms {

class PwdCommand : public CommandBase {
public:
    CommandResult execute(FileSystem& fs, const std::vector<std::string>& params) override;
    std::vector<ParamType> get_param_requirements() const override;
    std::string get_name() const override;
    std::string get_help() const override;
};

} // namespace ffvms

#endif // PWD_COMMAND_H
