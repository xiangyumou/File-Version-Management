#ifndef VERSION_COMMAND_H
#define VERSION_COMMAND_H

#include "interfaces/i_command.h"

namespace ffvms {

class VersionCommand : public CommandBase {
public:
    CommandResult execute(FileSystem& fs, const std::vector<std::string>& params) override;
    std::vector<ParamType> get_param_requirements() const override;
    std::string get_name() const override;
    std::string get_help() const override;
    std::vector<std::string> get_aliases() const override;
};

} // namespace ffvms

#endif // VERSION_COMMAND_H
