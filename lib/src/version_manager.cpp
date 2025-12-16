/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#include "version_manager.h"

bool VersionManager::load() {
    vvs node_information;
    if (!saver.load(DATA_TREENODE_INFO, node_information)) return false;
    vvs version_information;
    if (!saver.load(DATA_VERSION_INFO, version_information)) return false;

    std::map<unsigned long long, treeNode*> label_to_ptr;
    std::string s_label, s_type, s_cnt, s_link, s_next_brother, s_first_son;
    for (auto& node : node_information) {
        if (node.size() != 6) {
            logger.log("VersionManager: File is corrupted and cannot be read.", Logger::WARNING, __LINE__);
            return false;
        }
        s_label = node[0];
        s_type = node[1];
        s_cnt = node[2];
        s_link = node[3];
        s_next_brother = node[4];
        s_first_son = node[5];
        if (!saver.is_all_digits(s_label) || !saver.is_all_digits(s_type) || 
            !saver.is_all_digits(s_cnt) || !saver.is_all_digits(s_link) || 
            !saver.is_all_digits(s_next_brother) || !saver.is_all_digits(s_first_son)) {
            logger.log("VersionManager: File is corrupted and cannot be read.", Logger::WARNING, __LINE__);
            return false;
        }

        unsigned long long label, type, cnt, link;
        label = saver.str_to_ull(s_label);
        type = saver.str_to_ull(s_type);
        cnt = saver.str_to_ull(s_cnt);
        link = saver.str_to_ull(s_link);
        
        if (type >= 3) {
            logger.log("VersionManager: File is corrupted and cannot be read.", Logger::WARNING, __LINE__);
            return false;
        }

        treeNode* t = new treeNode();
        if (type == 0) t->type = treeNode::FILE;
        else if (type == 1) t->type = treeNode::DIR;
        else t->type = treeNode::HEAD_NODE;
        t->cnt = static_cast<int>(cnt);
        t->link = link;

        label_to_ptr[label] = t;
    }

    for (auto& node : node_information) {
        s_label = node[0];
        s_next_brother = node[4];
        s_first_son = node[5];

        unsigned long long label, next_brother, first_son;
        label = saver.str_to_ull(s_label);
        next_brother = saver.str_to_ull(s_next_brother);
        first_son = saver.str_to_ull(s_first_son);
        
        if (next_brother != NULL_NODE && !label_to_ptr.count(next_brother)) {
            logger.log("VersionManager: File is corrupted and cannot be read.", Logger::WARNING, __LINE__);
            return false;
        }
        if (first_son != NULL_NODE && !label_to_ptr.count(first_son)) {
            logger.log("VersionManager: File is corrupted and cannot be read.", Logger::WARNING, __LINE__);
            return false;
        }

        treeNode* t = label_to_ptr[label];
        t->next_brother = next_brother == NULL_NODE ? nullptr : label_to_ptr[next_brother];
        t->first_son = first_son == NULL_NODE ? nullptr : label_to_ptr[first_son];
    }

    std::string s_version_id, version_info, s_version_head_label;
    for (auto& ver : version_information) {
        if (ver.size() != 3) {
            logger.log("VersionManager: File is corrupted and cannot be read.", Logger::WARNING, __LINE__);
            return false;
        }
        s_version_id = ver[0];
        version_info = ver[1];
        s_version_head_label = ver[2];
        if (!saver.is_all_digits(s_version_id) || !saver.is_all_digits(s_version_head_label)) {
            logger.log("VersionManager: File is corrupted and cannot be read.", Logger::WARNING, __LINE__);
            return false;
        }
        unsigned long long version_id, version_head_label;
        version_id = saver.str_to_ull(s_version_id);
        version_head_label = saver.str_to_ull(s_version_head_label);

        if (!label_to_ptr.count(version_head_label)) {
            version.clear();
            logger.log("VersionManager: File is corrupted and cannot be read.", Logger::WARNING, __LINE__);
            return false;
        }

        auto t = versionNode();
        t.info = version_info;
        t.p = label_to_ptr[version_head_label];
        
        version[version_id] = t;
    }

    return true;
}

void VersionManager::dfs(treeNode* cur, std::map<treeNode*, unsigned long long>& label) {
    if (cur == nullptr || label.count(cur)) return;
    dfs(cur->next_brother, label);
    dfs(cur->first_son, label);
    label[cur] = label.size();
}

bool VersionManager::save() {
    std::map<treeNode*, unsigned long long> label;
    for (auto& ver : version) {
        dfs(ver.second.p, label);
    }
    vvs node_information;
    for (auto& node : label) {
        node_information.push_back(std::vector<std::string>());
        std::vector<std::string>& noif = node_information.back();
        noif.push_back(std::to_string(node.second));
        treeNode* tn = node.first;
        if (tn->type == treeNode::FILE) {
            noif.push_back("0");
        } else if (tn->type == treeNode::DIR) {
            noif.push_back("1");
        } else if (tn->type == treeNode::HEAD_NODE) {
            noif.push_back("2");
        }
        noif.push_back(std::to_string(tn->cnt));
        noif.push_back(std::to_string(tn->link));
        if (tn->next_brother == nullptr) {
            noif.push_back(std::to_string(NULL_NODE));
        } else {
            noif.push_back(std::to_string(label[tn->next_brother]));
        }
        if (tn->first_son == nullptr) {
            noif.push_back(std::to_string(NULL_NODE));
        } else {
            noif.push_back(std::to_string(label[tn->first_son]));
        }
    }
    if (!saver.save(DATA_TREENODE_INFO, node_information)) {
        return false;
    }
    vvs version_information;
    for (auto& it : version) {
        version_information.push_back(std::vector<std::string>());
        std::vector<std::string>& veif = version_information.back();
        veif.push_back(std::to_string(it.first));
        veif.push_back(it.second.info);
        veif.push_back(std::to_string(label[it.second.p]));
    }
    if (!saver.save(DATA_VERSION_INFO, version_information)) {
        return false;
    }
    return true;
}

VersionManager::VersionManager() {
    if (!load()) return;
}

VersionManager::~VersionManager() {
    if (!save()) return;
}

bool VersionManager::recursive_increase_counter(treeNode* p, bool modify_brother) {
    if (p == nullptr) {
        logger.log("Get a null pointer in line " + std::to_string(__LINE__));
        return false;
    }
    recursive_increase_counter(p->first_son, true);
    if (modify_brother) recursive_increase_counter(p->next_brother, true);
    p->cnt++;
    node_manager.increase_counter(p->link);
    logger.log("The counter for node " + node_manager.get_name(p->link) + " has been incremented by one.");
    return true;
}

bool VersionManager::init_version(treeNode* p, treeNode* vp) {
    if (p == nullptr || vp == nullptr) {
        logger.log("Get a null pointer in line " + std::to_string(__LINE__), Logger::FATAL, __LINE__);
        return false;
    }
    p->first_son = vp->first_son;
    if (!recursive_increase_counter(p, true)) return false;
    return true;
}

bool VersionManager::create_version(unsigned long long model_version, std::string version_info) {
    if (model_version != NO_MODEL_VERSION && !version_exist(model_version)) {
        logger.log("The version number does not exist in the system.", Logger::WARNING, __LINE__);
        return false;
    }
    treeNode* new_version = new treeNode(treeNode::DIR);
    if (new_version == nullptr) {
        logger.log("The system did not allocate memory for this operation.", Logger::FATAL, __LINE__);
        return false;
    }
    new_version->cnt = 0;
    new_version->link = node_manager.get_new_node("root");
    if (model_version != NO_MODEL_VERSION && new_version->first_son != nullptr) {
        delete new_version->first_son;
        new_version->first_son = nullptr;
    }
    treeNode* model = model_version == NO_MODEL_VERSION ? new_version : version[model_version].p;
    if (!init_version(new_version, model)) return false;
    unsigned long long id = version.empty() ? 1001 : (*version.rbegin()).first + 1;
    version[id] = versionNode(version_info, new_version);
    return true;
}

bool VersionManager::version_exist(unsigned long long id) {
    return version.count(id);
}

bool VersionManager::get_version_pointer(unsigned long long id, treeNode*& p) {
    if (!version_exist(id)) {
        logger.log("Version " + std::to_string(id) + " does not exist.", Logger::WARNING, __LINE__);
        return false;
    }
    p = version[id].p;
    return true;
}

bool VersionManager::get_latest_version(unsigned long long& id) {
    if (version.empty()) {
        logger.log("No version exists in the system. Please create a new version to use.", Logger::WARNING, __LINE__);
        return false;
    }
    id = version.rbegin()->first;
    return true;
}

bool VersionManager::get_version_log(std::vector<std::pair<unsigned long long, versionNode>>& version_log) {
    for (auto& it : version) {
        version_log.push_back(it);
        version_log.back().second.p = nullptr;
    }
    return true;
}

bool VersionManager::empty() {
    return version.empty();
}
