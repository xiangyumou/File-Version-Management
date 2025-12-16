#include "commands/rmd_command.h"
#include "file_system.h"
#include "logger.h"
#include "interfaces/i_session.h"
#include <sstream>

namespace ffvms {

CommandResult RmdCommand::execute(ISession& session, const std::vector<std::string>& params) {
    FileSystem& fs = session.get_file_system();
    std::string error = validate_params(params, get_param_requirements());
    if (!error.empty()) return CommandResult::Error(error);

    std::ostringstream output;
    
    for (const auto& fn : params) {
        if (!fs.remove_dir(fn)) {
            if (!output.str().empty()) output << "\n";
            output << Logger::get_logger().get_information();
        }
    }
    
    return CommandResult::Ok(output.str());
}

std::vector<ParamType> RmdCommand::get_param_requirements() const {
    return {ParamType::STRING};
}

std::string RmdCommand::get_name() const {
    return "rmd";
}

std::string RmdCommand::get_help() const {
    return "Remove directory(ies). Usage: rmd <dirname> [dirname2 ...]";
}

std::vector<std::string> RmdCommand::get_aliases() const {
    return {"remove_dir"};
}

} // namespace ffvms
