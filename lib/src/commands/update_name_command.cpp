#include "commands/update_name_command.h"
#include "file_system.h"
#include "logger.h"
#include "interfaces/i_session.h"

namespace ffvms {

CommandResult UpdateNameCommand::execute(ISession& session, const std::vector<std::string>& params) {
    FileSystem& fs = session.get_file_system();
    std::string error = validate_params(params, get_param_requirements());
    if (!error.empty()) return CommandResult::Error(error);

    if (fs.update_name(params[0], params[1])) {
        return CommandResult::Ok();
    } else {
        return CommandResult::Error(Logger::get_logger().get_information());
    }
}

std::vector<ParamType> UpdateNameCommand::get_param_requirements() const {
    return {ParamType::STRING, ParamType::STRING};
}

std::string UpdateNameCommand::get_name() const {
    return "update_name";
}

std::string UpdateNameCommand::get_help() const {
    return "Rename a file or directory. Usage: update_name <old_name> <new_name>";
}

} // namespace ffvms
