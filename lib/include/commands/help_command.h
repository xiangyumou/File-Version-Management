#ifndef HELP_COMMAND_H
#define HELP_COMMAND_H

#include "interfaces/i_command.h"
#include "command_registry.h"

namespace ffvms {

class HelpCommand : public CommandBase {
    const CommandRegistry& registry_;
public:
    explicit HelpCommand(const CommandRegistry& registry);
    CommandResult execute(FileSystem& fs, const std::vector<std::string>& params) override;
    std::vector<ParamType> get_param_requirements() const override;
    std::string get_name() const override;
    std::string get_help() const override;
};

} // namespace ffvms

#endif // HELP_COMMAND_H
