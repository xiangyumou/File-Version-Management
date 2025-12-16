#include "commands/cat_command.h"
#include "file_system.h"
#include "logger.h"
#include "interfaces/i_session.h"

namespace ffvms {

CommandResult CatCommand::execute(ISession& session, const std::vector<std::string>& params) {
    FileSystem& fs = session.get_file_system();
    std::string error = validate_params(params, get_param_requirements());
    if (!error.empty()) return CommandResult::Error(error);

    std::string content;
    if (fs.get_content(params[0], content)) {
        return CommandResult::Ok(content);
    } else {
        return CommandResult::Error(Logger::get_logger().get_information());
    }
}

std::vector<ParamType> CatCommand::get_param_requirements() const {
    return {ParamType::STRING};
}

std::string CatCommand::get_name() const {
    return "cat";
}

std::string CatCommand::get_help() const {
    return "Display file content. Usage: cat <filename>";
}

} // namespace ffvms
