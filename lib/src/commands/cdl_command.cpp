#include "commands/cdl_command.h"
#include "file_system.h"
#include "logger.h"

namespace ffvms {

CommandResult CdlCommand::execute(FileSystem& fs, const std::vector<std::string>& params) {
    if (fs.goto_last_dir()) {
        return CommandResult::Ok();
    } else {
        return CommandResult::Error(Logger::get_logger().get_information());
    }
}

std::vector<ParamType> CdlCommand::get_param_requirements() const {
    return {};
}

std::string CdlCommand::get_name() const {
    return "cdl";
}

std::string CdlCommand::get_help() const {
    return "Return to the previous directory.";
}

std::vector<std::string> CdlCommand::get_aliases() const {
    return {"goto_last_dir"};
}

} // namespace ffvms
