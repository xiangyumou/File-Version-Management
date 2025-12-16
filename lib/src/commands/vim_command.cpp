#include "commands/vim_command.h"
#include "file_system.h"
#include "logger.h"
#include "interfaces/i_session.h"
#include "core/platform.h"
#include <fstream>
#include <iostream>

namespace ffvms {

CommandResult VimCommand::execute(ISession& session, const std::vector<std::string>& params) {
    FileSystem& fs = session.get_file_system();
    std::string error = validate_params(params, get_param_requirements());
    if (!error.empty()) return CommandResult::Error(error);

    std::string file_name = "temp_file_" + params[0];
    std::string content;
    
    // Try to get content, ignore error if file doesn't exist (new file)
    fs.get_content(params[0], content);
    
    {
        std::ofstream out(file_name);
        if (!out) {
            return CommandResult::Error("Failed to create temporary file for editing.");
        }
        out << content;
        out.close();
    }
    
    std::string cmd = platform::get_editor_command() + " " + file_name;
    std::system(cmd.c_str());
    
    std::ifstream in(file_name);
    if (!in) {
        return CommandResult::Error("Failed to open temporary file after editing.");
    }
    
    content.clear();
    std::string tmp;
    while (std::getline(in, tmp)) {
        content += tmp;
        content.push_back('\n');
    }
    in.close();
    
    std::remove(file_name.c_str());
    
    // Create file if it doesn't exist
    fs.make_file(params[0]);
    
    if (fs.update_content(params[0], content)) {
        return CommandResult::Ok();
    } else {
        return CommandResult::Error(Logger::get_logger().get_information());
    }
}

std::vector<ParamType> VimCommand::get_param_requirements() const {
    return {ParamType::STRING};
}

std::string VimCommand::get_name() const {
    return "vim";
}

std::string VimCommand::get_help() const {
    return "Edit file content using system editor.";
}

} // namespace ffvms
