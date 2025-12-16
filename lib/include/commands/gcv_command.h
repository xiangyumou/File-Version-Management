#ifndef GCV_COMMAND_H
#define GCV_COMMAND_H

#include "interfaces/i_command.h"

namespace ffvms {

class GcvCommand : public CommandBase {
public:
    CommandResult execute(ISession& session, const std::vector<std::string>& params) override;
    std::vector<ParamType> get_param_requirements() const override;
    std::string get_name() const override;
    std::string get_help() const override;
    std::vector<std::string> get_aliases() const override;
};

} // namespace ffvms

#endif // GCV_COMMAND_H
