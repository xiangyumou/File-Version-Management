#ifndef LS_COMMAND_H
#define LS_COMMAND_H

#include "interfaces/i_command.h"

namespace ffvms {

class LsCommand : public CommandBase {
public:
    CommandResult execute(FileSystem& fs, const std::vector<std::string>& params) override;
    std::vector<ParamType> get_param_requirements() const override;
    std::string get_name() const override;
    std::string get_help() const override;
    std::vector<std::string> get_aliases() const override;
};

} // namespace ffvms

#endif // LS_COMMAND_H
