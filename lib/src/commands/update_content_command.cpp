#include "commands/update_content_command.h"
#include "file_system.h"
#include "logger.h"
#include "interfaces/i_session.h"

namespace ffvms {

CommandResult UpdateContentCommand::execute(ISession& session, const std::vector<std::string>& params) {
    FileSystem& fs = session.get_file_system();
    std::string error = validate_params(params, get_param_requirements());
    if (!error.empty()) return CommandResult::Error(error);

    if (fs.update_content(params[0], params[1])) {
        return CommandResult::Ok();
    } else {
        return CommandResult::Error(Logger::get_logger().get_information());
    }
}

std::vector<ParamType> UpdateContentCommand::get_param_requirements() const {
    return {ParamType::STRING, ParamType::STRING};
}

std::string UpdateContentCommand::get_name() const {
    return "update_content";
}

std::string UpdateContentCommand::get_help() const {
    return "Update file content. Usage: update_content <filename> <content>";
}

} // namespace ffvms
