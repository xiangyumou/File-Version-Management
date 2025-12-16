#include "commands/create_version_command.h"
#include "file_system.h"
#include "logger.h"
#include "interfaces/i_session.h"

namespace ffvms {

CommandResult CreateVersionCommand::execute(ISession& session, const std::vector<std::string>& params) {
    FileSystem& fs = session.get_file_system();
    if (params.empty()) {
        if (fs.create_version("")) {
            return CommandResult::Ok();
        }
    } else if (params.size() == 1) {
        if (is_all_digits(params[0])) {
             if (fs.create_version(str_to_ull(params[0]))) return CommandResult::Ok();
        } else {
             if (fs.create_version(params[0])) return CommandResult::Ok();
        }
    } else {
        if (is_all_digits(params[0])) {
             if (fs.create_version(str_to_ull(params[0]), params[1])) return CommandResult::Ok();
        } else if (is_all_digits(params[1])) {
             if (fs.create_version(params[0], str_to_ull(params[1]))) return CommandResult::Ok();
        } else {
             return CommandResult::Error("create_version requires at least one numeric parameter (version id).");
        }
    }
    
    return CommandResult::Error(Logger::get_logger().get_information());
}

std::vector<ParamType> CreateVersionCommand::get_param_requirements() const {
    return {};
}

std::string CreateVersionCommand::get_name() const {
    return "create_version";
}

std::string CreateVersionCommand::get_help() const {
    return "Create a new version. Usage: create_version [id] [description]";
}

} // namespace ffvms
