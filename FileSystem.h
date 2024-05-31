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

// �ļ���Ϣ�ṹ
struct File {
    std::string name;
    std::vector<int> blocks; // �ļ�ռ�õĴ��̿�
    int size; // �ļ���С
    int physicalAddress; // �ļ��������ַ����ţ�
    int protectionCode; // ������
    std::string owner; // �ļ�������
    bool readPermission; // ��Ȩ��
    bool writePermission; // дȨ��
    std::unordered_map<std::string, std::pair<bool, bool>> permissions; // �û�Ȩ�ޱ�<�û���, <��Ȩ��, дȨ��>> };

};

// �ļ�ϵͳ��
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
    std::unordered_map<int, std::string> directory; // ��Ŀ¼
    std::unordered_map<std::string, User> users; // �û���Ϣ
    User* currentUser; // ��ǰ��¼�û�
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
        return -1; // û�п��ÿ�
    }

    void freeBlock(int blockIndex) {
        freeBlocks.push_back(blockIndex);
    }
};

#endif // !FILESYSTEM_H
