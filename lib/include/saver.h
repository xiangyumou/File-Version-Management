/**
   ___ _                 _                      
  / __| |__   __ _ _ __ | |_    /\/\   ___  ___ 
 / /  | '_ \ / _` | '_ \| __|  /    \ / _ \/ _ \
/ /___| | | | (_| | | | | |_  / /\/\ |  __|  __/
\____/|_| |_|\__,_|_| |_|\__| \/    \/\___|\___|

@ Author: Mu Xiangyu, Chant Mee 
*/

#ifndef SAVER_H
#define SAVER_H

#include "encryptor.h"
#include "logger.h"
#include <string>
#include <vector>
#include <map>

// Type alias for 2D string vector
typedef std::vector<std::vector<std::string>> vvs;

/**
 * @brief Data node structure for encrypted storage
 */
struct dataNode {
    unsigned long long name_hash, data_hash;
    int len;
    std::vector<std::pair<double, double>> data;

    dataNode();
    dataNode(unsigned long long name_hash, unsigned long long data_hash, 
             std::vector<std::pair<double, double>>& data, int n);
};

/**
 * @brief Saver class for persistent data storage with encryption
 */
class Saver : private Encryptor {
private:
    std::string data_file = "data.chm";
    std::map<unsigned long long, dataNode> mp;
    Logger& logger = Logger::get_logger();

    template <class T>
    unsigned long long get_hash(T& s);

    bool load_file();
    void save_data(unsigned long long name_hash, unsigned long long data_hash, 
                   std::vector<std::pair<double, double>> data);
    int read(std::string& s);

public:
    Saver();
    ~Saver();

    bool save(std::string name, std::vector<std::vector<std::string>>& content);
    bool load(std::string name, std::vector<std::vector<std::string>>& content, 
              bool mandatory_access = false);
    
    static bool is_all_digits(const std::string& s);
    static unsigned long long str_to_ull(const std::string& s);
    static Saver& get_saver();
};

#endif // SAVER_H
