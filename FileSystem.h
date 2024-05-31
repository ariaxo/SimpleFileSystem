#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include<Disk.h>
#include<User.h>

// 文件信息结构
struct File {
    std::string name;
    std::vector<int> blocks; // 文件占用的磁盘块
    int size; // 文件大小
    int physicalAddress; // 文件的物理地址（编号）
    int protectionCode; // 保护码
    std::string owner; // 文件所有者
    bool readPermission; // 读权限
    bool writePermission; // 写权限
    std::unordered_map<std::string, std::pair<bool, bool>> permissions; // 用户权限表<用户名, <读权限, 写权限>> };

};

// 文件系统类
class FileSystem {
public:
    FileSystem();

    void registerUser(const std::string& username, const std::string& password);
    void loginUser(const std::string& username, const std::string& password);
    void logoutUser();
    void createFile(const std::string& name, const std::string& content, bool readPermission, bool writePermission);
    void readFile(const std::string& name);
    void writeFile(const std::string& name, const std::string& content);
    void deleteFile(const std::string& name);
    void showDirectory();
    }

private:
    Disk disk;
    std::unordered_map<std::string, File> files;
    std::unordered_map<int, std::string> directory; // 主目录
    std::unordered_map<std::string, User> users; // 用户信息
    User* currentUser; // 当前登录用户
    std::vector<int> freeBlocks;
    int fileCounter;

    int allocateBlock() {
        if (freeBlocks.empty()) {
            for (int i = 0; i < TOTAL_BLOCKS; ++i) {
                freeBlocks.push_back(i);
            }
        }
        if (!freeBlocks.empty()) {
            int blockIndex = freeBlocks.back();
            freeBlocks.pop_back();
            return blockIndex;
        }
        return -1; // 没有可用块
    }

    void freeBlock(int blockIndex) {
        freeBlocks.push_back(blockIndex);
    }
};

#endif // !FILESYSTEM_H
