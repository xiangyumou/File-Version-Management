#include "commands/help_command.h"
#include "file_system.h"

namespace ffvms {

HelpCommand::HelpCommand(const CommandRegistry& registry) : registry_(registry) {}

CommandResult HelpCommand::execute(FileSystem& fs, const std::vector<std::string>& params) {
    return CommandResult::Ok(registry_.get_help());
}

std::vector<ParamType> HelpCommand::get_param_requirements() const {
    return {};
}

std::string HelpCommand::get_name() const {
    return "help";
}

std::string HelpCommand::get_help() const {
    return "Display this help message.";
}

} // namespace ffvms
