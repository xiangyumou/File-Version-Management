#include "commands/clear_command.h"
#include "core/platform.h"
#include "file_system.h" // Not used but required for signature

namespace ffvms {

CommandResult ClearCommand::execute(FileSystem& fs, const std::vector<std::string>& params) {
    ffvms::platform::clear_screen();
    return CommandResult::Ok();
}

std::vector<ParamType> ClearCommand::get_param_requirements() const {
    return {};
}

std::string ClearCommand::get_name() const {
    return "clear";
}

std::string ClearCommand::get_help() const {
    return "Clear the terminal screen.";
}

} // namespace ffvms
