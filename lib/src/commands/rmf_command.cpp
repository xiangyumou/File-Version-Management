#include "commands/rmf_command.h"
#include "file_system.h"
#include "logger.h"
#include "interfaces/i_session.h"
#include <sstream>

namespace ffvms {

CommandResult RmfCommand::execute(ISession& session, const std::vector<std::string>& params) {
    FileSystem& fs = session.get_file_system();
    std::string error = validate_params(params, get_param_requirements());
    if (!error.empty()) return CommandResult::Error(error);

    std::ostringstream output;
    
    for (const auto& fn : params) {
        if (!fs.remove_file(fn)) {
            if (!output.str().empty()) output << "\n";
            output << Logger::get_logger().get_information();
        }
    }
    
    // Even if errors occurred, we return Ok with the accumulated error messages as output
    // so they are displayed to the user, similar to original behavior.
    return CommandResult::Ok(output.str());
}

std::vector<ParamType> RmfCommand::get_param_requirements() const {
    return {ParamType::STRING};
}

std::string RmfCommand::get_name() const {
    return "rmf";
}

std::string RmfCommand::get_help() const {
    return "Remove file(s). Usage: rmf <filename> [filename2 ...]";
}

std::vector<std::string> RmfCommand::get_aliases() const {
    return {"remove_file"};
}

} // namespace ffvms
