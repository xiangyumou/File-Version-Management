#include "commands/ls_command.h"
#include "file_system.h"
#include "logger.h"
#include "interfaces/i_session.h"
#include <sstream>
#include <algorithm>

namespace ffvms {

CommandResult LsCommand::execute(ISession& session, const std::vector<std::string>& params) {
    FileSystem& fs = session.get_file_system();
    std::vector<std::string> ls_content;
    if (!fs.list_directory_contents(ls_content)) {
        return CommandResult::Error(Logger::get_logger().get_information());
    }
    
    if (ls_content.empty()) {
        return CommandResult::Ok("The folder is empty.  QAQ");
    }

    std::ostringstream oss;
    bool detailed = false;
    if (!params.empty() && params[0] == "-a") {
        detailed = true;
    }

    if (!detailed) {
        std::sort(ls_content.begin(), ls_content.end());
        for (size_t i = 0; i < ls_content.size(); i++) {
            if (i != 0 && i % 8 == 0) oss << '\n';
            oss << ls_content[i] << "\t";
        }
    } else {
         oss << "type\t" << "create time\t\t" << "update time\t\t" << "name\n";
         
         for (const auto& item : ls_content) {
            treeNode::TYPE type;
            std::string create_time, update_time;
            
            if (!fs.get_type(item, type)) return CommandResult::Error(Logger::get_logger().get_information());
            if (!fs.get_create_time(item, create_time)) return CommandResult::Error(Logger::get_logger().get_information());
            if (!fs.get_update_time(item, update_time)) return CommandResult::Error(Logger::get_logger().get_information());
            
            oss << (type == treeNode::FILE ? "file" : "dir") << '\t' 
                << create_time << '\t' 
                << update_time << '\t' 
                << item << '\n';
         }
    }
    
    return CommandResult::Ok(oss.str());
}

std::vector<ParamType> LsCommand::get_param_requirements() const {
    return {}; // No mandatory parameters
}

std::string LsCommand::get_name() const {
    return "ls";
}

std::string LsCommand::get_help() const {
    return "List directory contents. Usage: ls [-a]";
}

std::vector<std::string> LsCommand::get_aliases() const {
    return {"list_directory_contents"};
}

} // namespace ffvms
