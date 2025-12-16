#include "commands/help_command.h"
#include "file_system.h"
#include "interfaces/i_session.h"

namespace ffvms {

HelpCommand::HelpCommand(const CommandRegistry& registry) : registry_(registry) {}

CommandResult HelpCommand::execute(ISession& session, const std::vector<std::string>& params) {
    FileSystem& fs = session.get_file_system();
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
