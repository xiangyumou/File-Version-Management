#ifndef FIND_COMMAND_H
#define FIND_COMMAND_H

#include "interfaces/i_command.h"

namespace ffvms {

class FindCommand : public CommandBase {
public:
    CommandResult execute(FileSystem& fs, const std::vector<std::string>& params) override;
    std::vector<ParamType> get_param_requirements() const override;
    std::string get_name() const override;
    std::string get_help() const override;
};

} // namespace ffvms

#endif // FIND_COMMAND_H
