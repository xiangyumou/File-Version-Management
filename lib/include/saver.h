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
#include "interfaces/i_storage.h"
#include "interfaces/i_logger.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

// Type alias for 2D string vector (same as ffvms::DataTable)
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
 * 
 * Implements IStorage interface for data persistence.
 * Uses FFT-based encryption for data security.
 */
class Saver : public ffvms::IStorage, private Encryptor {
private:
    std::string data_file = "data.chm";
    std::map<unsigned long long, dataNode> mp;
    
    // Logger can be injected or use global singleton
    ffvms::ILogger* logger_ = nullptr;
    
    // Helper to get logger
    ffvms::ILogger& get_logger_ref();

    template <class T>
    unsigned long long get_hash(T& s);

    bool load_file();
    void save_data(unsigned long long name_hash, unsigned long long data_hash, 
                   std::vector<std::pair<double, double>> data);
    int read(std::string& s);

public:
    /// Default constructor (uses global Logger singleton)
    Saver();
    
    /// Constructor with injected logger
    explicit Saver(ffvms::ILogger* logger);
    
    ~Saver() override;

    // IStorage interface implementation (also serves as legacy interface since vvs == DataTable)
    bool save(const std::string& name, const ffvms::DataTable& content) override;
    bool load(const std::string& name, ffvms::DataTable& content, 
              bool mandatory_access = false) override;
    
    // Static utility functions
    static bool is_all_digits(const std::string& s);
    static unsigned long long str_to_ull(const std::string& s);
    
    /// Get global singleton instance (legacy access pattern)
    static Saver& get_saver();
};

#endif // SAVER_H
