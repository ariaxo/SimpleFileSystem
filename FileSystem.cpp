#include <FileSystem.h>
#include <fstream>

FileSystem() : disk(), fileCounter(0), currentUser(nullptr) {
    std::srand(std::time(0));
}

// 注册用户
void registerUser(const std::string& username, const std::string& password) {
    if (users.find(username) != users.end()) {
        std::cout << "User already exists!\n";
        return;
    }
    users[username] = { username, password };
    std::cout << "User registered successfully!\n";
}

// 用户登录
void loginUser(const std::string& username, const std::string& password) {
    if (users.find(username) == users.end() || users[username].password != password) {
        std::cout << "Invalid username or password!\n";
        return;
    }
    currentUser = &users[username];
    std::cout << "User logged in successfully!\n";
}

// 用户登出
void logoutUser() {
    if (currentUser) {
        std::cout << "User logged out: " << currentUser->username << "\n";
        currentUser = nullptr;
    }
    else {
        std::cout << "No user is currently logged in!\n";
    }
}

// 创建文件
void createFile(const std::string& name, const std::string& content, bool readPermission, bool writePermission) {
    if (!currentUser) {
        std::cout << "Please log in first!\n";
        return;
    }
    if (files.find(name) != files.end()) {
        std::cout << "File already exists!\n";
        return;
    }

    File file;
    file.name = name;
    file.size = content.size();
    file.physicalAddress = fileCounter++; // 分配文件编号作为物理地址
    file.protectionCode = std::rand(); // 随机生成保护码
    file.owner = currentUser->username; // 文件所有者
    file.readPermission = readPermission;
    file.writePermission = writePermission;
    file.permissions[currentUser->username] = { readPermission,writePermission };

    int numBlocks = (content.size() + BLOCK_SIZE - 1) / BLOCK_SIZE; // 计算需要的块数
    for (int i = 0; i < numBlocks; ++i) {
        int blockIndex = allocateBlock();
        if (blockIndex == -1) {
            std::cout << "No more space on disk!\n";
            return;
        }
        file.blocks.push_back(blockIndex);

        int offset = i * BLOCK_SIZE;
        int writeSize = std::min(BLOCK_SIZE, (int)content.size() - offset);
        disk.writeBlock(blockIndex, content.c_str() + offset, writeSize);
    }

    files[name] = file;
    directory[file.physicalAddress] = name; // 在目录中登记
}

// 读取文件内容
void readFile(const std::string& name) {
    if (!currentUser) {
        std::cout << "Please log in first!\n";
        return;
    }
    if (files.find(name) == files.end()) {
        std::cout << "File not found!\n";
        return;
    }

    const File& file = files[name];
    auto it = file.permissions.find(currentUser->username);
    if (it == file.permissions.end() || !it->second.first) {
        std::cout << "Permission denied!\n";
        return;
    }

    std::string content;
    content.reserve(file.size);

    for (int blockIndex : file.blocks) {
        char buffer[BLOCK_SIZE];
        disk.readBlock(blockIndex, buffer, BLOCK_SIZE);
        content.append(buffer, std::min(BLOCK_SIZE, file.size - (int)content.size()));
    }

    std::cout << "File content: " << content << "\n";
}

// 写入文件内容
void writeFile(const std::string& name, const std::string& content) {
    //cout << "writing" << endl;
    if (!currentUser) {
        std::cout << "Please log in first!\n";
        return;
    }
    if (files.find(name) == files.end()) {
        std::cout << "File not found!\n";
        return;
    }

    File& file = files[name];
    auto it = file.permissions.find(currentUser->username);
    if (it == file.permissions.end() || !it->second.second) {
        std::cout << "Permission denied!\n";
        return;
    }

    // 清空旧内容，写入新内容
    for (int blockIndex : file.blocks) {
        freeBlock(blockIndex);
    }
    file.blocks.clear();
    file.size = content.size();

    int numBlocks = (content.size() + BLOCK_SIZE - 1) / BLOCK_SIZE; // 计算需要的块数
    for (int i = 0; i < numBlocks; ++i) {
        int blockIndex = allocateBlock();
        if (blockIndex == -1) {
            std::cout << "No more space on disk!\n";
            return;
        }
        file.blocks.push_back(blockIndex);

        int offset = i * BLOCK_SIZE;
        int writeSize = std::min(BLOCK_SIZE, (int)content.size() - offset);
        disk.writeBlock(blockIndex, content.c_str() + offset, writeSize);
    }
}

// 删除文件
void deleteFile(const std::string& name) {
    if (!currentUser) {
        std::cout << "Please log in first!\n";
        return;
    }
    if (files.find(name) == files.end()) {
        std::cout << "File not found!\n";
        return;
    }

    File& file = files[name];
    auto it = file.permissions.find(currentUser->username);
    if (it == file.permissions.end() || !it->second.second) {
        std::cout << "Permission denied!\n";
        return;
    }

    for (int blockIndex : file.blocks) {
        freeBlock(blockIndex);
    }

    files.erase(name);
    directory.erase(file.physicalAddress); // 从目录中移除
}

// 显示目录
void showDirectory() {
    std::cout << std::left << std::setw(15) << "File Name"
        << std::setw(20) << "Physical Address"
        << std::setw(20) << "Protection Code"
        << std::setw(10) << "File Size"
        << std::setw(15) << "Owner"
        << std::setw(30) << "Permissions (username: [R, W])"
        << "\n";

    for (const auto& entry : directory) {
        const File& file = files[entry.second];
        std::cout << std::left << std::setw(15) << file.name
            << std::setw(20) << file.physicalAddress
            << std::setw(20) << file.protectionCode
            << std::setw(10) << file.size << " bytes"
            << std::setw(15) << file.owner;

        for (const auto& perm : file.permissions) {
            std::cout << perm.first << ": [" << (perm.second.first ? "R" : "-")
                << ", " << (perm.second.second ? "W" : "-") << "] ";
        }
        std::cout << "\n";
    }