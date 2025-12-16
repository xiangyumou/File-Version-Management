#include "commands/mkdir_command.h"
#include "file_system.h"
#include "logger.h"

namespace ffvms {

CommandResult MkdirCommand::execute(FileSystem& fs, const std::vector<std::string>& params) {
    std::string error = validate_params(params, get_param_requirements());
    if (!error.empty()) return CommandResult::Error(error);

    if (fs.make_dir(params[0])) {
        return CommandResult::Ok();
    } else {
        return CommandResult::Error(Logger::get_logger().get_information());
    }
}

std::vector<ParamType> MkdirCommand::get_param_requirements() const {
    return {ParamType::STRING};
}

std::string MkdirCommand::get_name() const {
    return "mkdir";
}

std::string MkdirCommand::get_help() const {
    return "Create a new directory. Usage: mkdir <dirname>";
}

} // namespace ffvms
