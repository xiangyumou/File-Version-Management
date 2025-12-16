/**
   ___ _                 _
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee
*/

#include "terminal.h"
#include "commands/cat_command.h"
#include "commands/cd_command.h"
#include "commands/cdl_command.h"
#include "commands/clear_command.h"
#include "commands/create_version_command.h"
#include "commands/find_command.h"
#include "commands/gcv_command.h"
#include "commands/help_command.h"
#include "commands/ls_command.h"
#include "commands/mkdir_command.h"
#include "commands/pwd_command.h"
#include "commands/rmd_command.h"
#include "commands/rmf_command.h"
#include "commands/switch_version_command.h"
#include "commands/touch_command.h"
#include "commands/tree_command.h"
#include "commands/update_content_command.h"
#include "commands/update_name_command.h"
#include "commands/version_command.h"
#include "commands/vim_command.h"
#include "core/types.h" // For LogLevel
#include "logger.h"
#include <iostream>
#include <memory>

using namespace ffvms;

Terminal::Terminal() : session_(file_system), logger_(nullptr) {
  register_commands();
}

ffvms::ILogger &Terminal::get_logger_ref() {
  if (logger_)
    return *logger_;
  return Logger::get_logger();
}

void Terminal::register_commands() {
  // Basic Commands
  registry_.register_command(std::make_unique<TouchCommand>());
  registry_.register_command(std::make_unique<MkdirCommand>());
  registry_.register_command(std::make_unique<CdCommand>());
  registry_.register_command(std::make_unique<LsCommand>());
  registry_.register_command(std::make_unique<CatCommand>());

  // File Management
  registry_.register_command(std::make_unique<RmfCommand>());
  registry_.register_command(std::make_unique<RmdCommand>());
  registry_.register_command(std::make_unique<UpdateNameCommand>());
  registry_.register_command(std::make_unique<UpdateContentCommand>());
  registry_.register_command(std::make_unique<TreeCommand>());
  registry_.register_command(std::make_unique<CdlCommand>());
  registry_.register_command(std::make_unique<PwdCommand>());
  registry_.register_command(std::make_unique<FindCommand>());

  // Version Management
  registry_.register_command(std::make_unique<CreateVersionCommand>());
  registry_.register_command(std::make_unique<SwitchVersionCommand>());
  registry_.register_command(std::make_unique<VersionCommand>());
  registry_.register_command(std::make_unique<GcvCommand>());

  // System Utilities
  registry_.register_command(std::make_unique<ClearCommand>());
  registry_.register_command(std::make_unique<VimCommand>());
  registry_.register_command(std::make_unique<HelpCommand>(registry_));
}

int Terminal::run() {
  while (true) {
    std::cout << "# ";
    std::vector<std::string> args = interpreter_.parse_input();

    if (args.empty())
      continue;

    std::string name = args[0];
    if (name == "exit")
      return 0;

    // Separate parameters from command name
    std::vector<std::string> params;
    if (args.size() > 1) {
      params.assign(args.begin() + 1, args.end());
    }

    auto cmd = registry_.get_command(name);
    if (cmd) {
      CommandResult result = cmd->execute(session_, params);
      if (result.success) {
        if (!result.output.empty()) {
          std::cout << result.output << "\n";
        }
      } else {
        std::cout << result.message << "\n";
      }
    } else {
      std::cout << "Error: Command not found: " << name << "\n";
      std::cout << "Type 'help' to see available commands.\n";
    }
  }
  return 0;
}
