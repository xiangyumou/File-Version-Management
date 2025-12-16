#include "commands/version_command.h"
#include "file_system.h"
#include "logger.h"
#include "interfaces/i_session.h"
#include <sstream>

namespace ffvms {

CommandResult VersionCommand::execute(ISession& session, const std::vector<std::string>& params) {
    FileSystem& fs = session.get_file_system();
    std::vector<std::pair<unsigned long long, versionNode>> version_content;
    if (!fs.version(version_content)) {
        return CommandResult::Error(Logger::get_logger().get_information());
    }

    std::ostringstream oss;
    oss << "version id\t" << "information\n";
    for (const auto& it : version_content) {
        oss << it.first << "\t\t" << (it.second.info.empty() ? "NULL" : it.second.info) << '\n';
    }
    
    return CommandResult::Ok(oss.str());
}

std::vector<ParamType> VersionCommand::get_param_requirements() const {
    return {};
}

std::string VersionCommand::get_name() const {
    return "version";
}

std::string VersionCommand::get_help() const {
    return "Display version log.";
}

std::vector<std::string> VersionCommand::get_aliases() const {
    return {"get_version_log"};
}

} // namespace ffvms
