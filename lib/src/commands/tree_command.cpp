#include "commands/tree_command.h"
#include "file_system.h"
#include "logger.h"

namespace ffvms {

CommandResult TreeCommand::execute(FileSystem& fs, const std::vector<std::string>& params) {
    std::string tree_content;
    if (fs.tree(tree_content)) {
        return CommandResult::Ok(tree_content);
    } else {
        return CommandResult::Error(Logger::get_logger().get_information());
    }
}

std::vector<ParamType> TreeCommand::get_param_requirements() const {
    return {};
}

std::string TreeCommand::get_name() const {
    return "tree";
}

std::string TreeCommand::get_help() const {
    return "Display the directory tree structure.";
}

} // namespace ffvms
