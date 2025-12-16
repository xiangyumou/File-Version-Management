#include "commands/pwd_command.h"
#include "file_system.h"
#include "logger.h"
#include "interfaces/i_session.h"
#include <sstream>

namespace ffvms {

CommandResult PwdCommand::execute(ISession& session, const std::vector<std::string>& params) {
    FileSystem& fs = session.get_file_system();
    std::vector<std::string> path;
    if (!fs.get_current_path(path)) {
        return CommandResult::Error(Logger::get_logger().get_information());
    }
    
    std::ostringstream oss;
    oss << '/';
    for (auto& p : path) {
        oss << p << '/';
    }
    return CommandResult::Ok(oss.str());
}

std::vector<ParamType> PwdCommand::get_param_requirements() const {
    return {};
}

std::string PwdCommand::get_name() const {
    return "pwd";
}

std::string PwdCommand::get_help() const {
    return "Print current working directory.";
}

} // namespace ffvms
