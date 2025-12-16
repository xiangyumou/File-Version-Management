#include "commands/switch_version_command.h"
#include "file_system.h"
#include "logger.h"

namespace ffvms {

CommandResult SwitchVersionCommand::execute(FileSystem& fs, const std::vector<std::string>& params) {
    std::string error = validate_params(params, get_param_requirements());
    if (!error.empty()) return CommandResult::Error(error);

    if (fs.switch_version(static_cast<int>(str_to_ull(params[0])))) {
        return CommandResult::Ok("Switched to version " + params[0]);
    } else {
        return CommandResult::Error(Logger::get_logger().get_information());
    }
}

std::vector<ParamType> SwitchVersionCommand::get_param_requirements() const {
    return {ParamType::ULL};
}

std::string SwitchVersionCommand::get_name() const {
    return "switch_version";
}

std::string SwitchVersionCommand::get_help() const {
    return "Switch to a specific version. Usage: switch_version <id>";
}

} // namespace ffvms
