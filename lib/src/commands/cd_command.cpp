#include "commands/cd_command.h"
#include "file_system.h"
#include "logger.h"
#include "interfaces/i_session.h"

namespace ffvms {

CommandResult CdCommand::execute(ISession& session, const std::vector<std::string>& params) {
    std::string error = validate_params(params, get_param_requirements());
    if (!error.empty()) return CommandResult::Error(error);

    if (session.change_directory(params[0])) {
        return CommandResult::Ok();
    } else {
        return CommandResult::Error(Logger::get_logger().get_information());
    }
}

std::vector<ParamType> CdCommand::get_param_requirements() const {
    return {ParamType::STRING};
}

std::string CdCommand::get_name() const {
    return "cd";
}

std::string CdCommand::get_help() const {
    return "Change current directory. Usage: cd <dirname>";
}

std::vector<std::string> CdCommand::get_aliases() const {
    return {"change_directory"};
}

} // namespace ffvms
