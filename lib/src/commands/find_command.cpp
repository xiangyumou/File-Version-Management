#include "commands/find_command.h"
#include "file_system.h"
#include "logger.h"
#include "interfaces/i_session.h"
#include <sstream>

namespace ffvms {

CommandResult FindCommand::execute(ISession& session, const std::vector<std::string>& params) {
    FileSystem& fs = session.get_file_system();
    std::string error = validate_params(params, get_param_requirements());
    if (!error.empty()) return CommandResult::Error(error);

    std::vector<std::pair<std::string, std::vector<std::string>>> res;
    if (!fs.Find(params[0], res)) {
        return CommandResult::Error(Logger::get_logger().get_information());
    }
    
    std::ostringstream oss;
    oss << "name\t" << "path\n";
    for (auto& r : res) {
        oss << r.first << '\t' << '/';
        for (auto& s : r.second) {
            oss << s << '/';
        }
        oss << '\n';
    }
    return CommandResult::Ok(oss.str());
}

std::vector<ParamType> FindCommand::get_param_requirements() const {
    return {ParamType::STRING};
}

std::string FindCommand::get_name() const {
    return "find";
}

std::string FindCommand::get_help() const {
    return "Find files by name. Usage: find <name>";
}

} // namespace ffvms
