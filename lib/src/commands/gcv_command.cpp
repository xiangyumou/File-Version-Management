#include "commands/gcv_command.h"
#include "file_system.h"
#include "logger.h"
#include "interfaces/i_session.h"

namespace ffvms {

CommandResult GcvCommand::execute(ISession& session, const std::vector<std::string>& params) {
    FileSystem& fs = session.get_file_system();
    return CommandResult::Ok("The current version of the file system is " + 
                             std::to_string(fs.get_current_version()));
}

std::vector<ParamType> GcvCommand::get_param_requirements() const {
    return {};
}

std::string GcvCommand::get_name() const {
    return "gcv";
}

std::string GcvCommand::get_help() const {
    return "Get current version ID.";
}

std::vector<std::string> GcvCommand::get_aliases() const {
    return {"get_current_version"};
}

} // namespace ffvms
