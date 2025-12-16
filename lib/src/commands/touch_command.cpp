#include "commands/touch_command.h"
#include "file_system.h"
#include "logger.h"
#include "interfaces/i_session.h"

namespace ffvms {

CommandResult TouchCommand::execute(ISession& session, const std::vector<std::string>& params) {
    FileSystem& fs = session.get_file_system();
    std::string error = validate_params(params, get_param_requirements());
    if (!error.empty()) return CommandResult::Error(error);

    if (fs.make_file(params[0])) {
        return CommandResult::Ok(); // Success, no output
    } else {
        return CommandResult::Error(Logger::get_logger().get_information());
    }
}

std::vector<ParamType> TouchCommand::get_param_requirements() const {
    return {ParamType::STRING};
}

std::string TouchCommand::get_name() const {
    return "touch";
}

std::string TouchCommand::get_help() const {
    return "Create a new file. Usage: touch <filename>";
}

} // namespace ffvms
