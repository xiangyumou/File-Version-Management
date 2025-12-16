#ifndef SWITCH_VERSION_COMMAND_H
#define SWITCH_VERSION_COMMAND_H

#include "interfaces/i_command.h"

namespace ffvms {

class SwitchVersionCommand : public CommandBase {
public:
    CommandResult execute(ISession& session, const std::vector<std::string>& params) override;
    std::vector<ParamType> get_param_requirements() const override;
    std::string get_name() const override;
    std::string get_help() const override;
};

} // namespace ffvms

#endif // SWITCH_VERSION_COMMAND_H
