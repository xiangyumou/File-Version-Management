#ifndef CDL_COMMAND_H
#define CDL_COMMAND_H

#include "interfaces/i_command.h"

namespace ffvms {

class CdlCommand : public CommandBase {
public:
    CommandResult execute(FileSystem& fs, const std::vector<std::string>& params) override;
    std::vector<ParamType> get_param_requirements() const override;
    std::string get_name() const override;
    std::string get_help() const override;
    std::vector<std::string> get_aliases() const override;
};

} // namespace ffvms

#endif // CDL_COMMAND_H
