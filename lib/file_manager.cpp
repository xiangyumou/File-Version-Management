#ifndef FILE_MANAGER_CPP
#define FILE_MANAGER_CPP

#include "logger.cpp"
#include <string>
#include <map>

class FileManager {
private:
    struct fileNode {
        std::string content;
        int cnt;

        fileNode() = default;
        fileNode(std::string content) : content(content), cnt(1) {}
    };

    std::map<unsigned long long, fileNode> mp;
    Logger &logger = Logger::get_logger();

    unsigned long long get_new_id() {
        unsigned long long id;
        do {
            id = 1ULL * rand() * rand() * rand();
        } while (mp.count(id));
        return id;
    }

    bool file_exist(unsigned long long fid) {
        if (!mp.count(fid)) {
            logger.log("File id " + std::to_string(fid) + " does not exists. This is not normal. Please check if the procedure is correct.", Logger::FATAL, __LINE__);
            return false;
        }
        return true;
    }

    bool check_file(unsigned long long fid) {
        if (!file_exist(fid)) return false;
        if (mp[fid].cnt <= 0 ) {
            logger.log("File ID is " + std::to_string(fid) + " corresponding to the file, its counter is less than or equal to 0, this is abnormal, please check whether the program is correct.", Logger::FATAL, __LINE__);
            return false;
        }
        return true;
    }

public:
    // std::map<unsigned long long, fileNode> &MP = mp;

    static FileManager& get_file_manager() {
        static FileManager file_manager;
        return file_manager;
    }

    unsigned long long create_file(std::string content="") {
        unsigned long long id = get_new_id();
        mp[id] = fileNode(content);
        return id;
    }

    bool increase_counter(unsigned long long fid) {
        if (!mp.count(fid)) {
            logger.log("File id does not exists. Please check if the procedure is correct.", Logger::FATAL, __LINE__);
            return false;
        }
        if (!check_file(fid)) return false;
        mp[fid].cnt ++;
        return true;
    }

    bool decrease_counter(unsigned long long fid) {
        if (!mp.count(fid)) {
            logger.log("File id does not exists. Please check if the procedure is correct.", Logger::FATAL, __LINE__);
            return false;
        }
        if (!check_file(fid)) return false;
        if (--mp[fid].cnt <= 0) {
            mp.erase(mp.find(fid));
        }
        return true;
    }

    bool update_content(unsigned long long fid, unsigned long long &new_id, std::string content) {
        if (!file_exist(fid)) return false;
        if (!decrease_counter(fid)) return false;
        new_id = get_new_id();
        mp[new_id].content = content;
        mp[new_id].cnt = 1;
        return true;
    }

    bool get_content(unsigned long long fid, std::string &content) {
        if (!file_exist(fid)) return false;
        content = mp[fid].content;
        return true;
    }
};

int test_file_manager() {
// int main() {
    Logger &logger = Logger::get_logger();
    FileManager fm;
    unsigned long long id = fm.create_file("123123123");
    if (!fm.increase_counter(id)) std::cout << *logger.information << '\n';
    unsigned long long id1;
    fm.update_content(id, id1, "hahaha");
    fm.update_content(id, id1, "hahaha");
    std::string content;
    if (!fm.get_content(id, content)) std::cout << *logger.information << '\n';
    std::cout << content << '\n';
    if (!fm.get_content(id1, content)) std::cout << *logger.information << '\n';
    std::cout << content << '\n';
    return 0;
}

#endif