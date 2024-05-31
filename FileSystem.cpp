#include <FileSystem.h>
#include <fstream>

FileSystem() : disk(), fileCounter(0), currentUser(nullptr) {
    std::srand(std::time(0));
}

// ע���û�
void registerUser(const std::string& username, const std::string& password) {
    if (users.find(username) != users.end()) {
        std::cout << "User already exists!\n";
        return;
    }
    users[username] = { username, password };
    std::cout << "User registered successfully!\n";
}

// �û���¼
void loginUser(const std::string& username, const std::string& password) {
    if (users.find(username) == users.end() || users[username].password != password) {
        std::cout << "Invalid username or password!\n";
        return;
    }
    currentUser = &users[username];
    std::cout << "User logged in successfully!\n";
}

// �û��ǳ�
void logoutUser() {
    if (currentUser) {
        std::cout << "User logged out: " << currentUser->username << "\n";
        currentUser = nullptr;
    }
    else {
        std::cout << "No user is currently logged in!\n";
    }
}

// �����ļ�
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
    file.physicalAddress = fileCounter++; // �����ļ������Ϊ�����ַ
    file.protectionCode = std::rand(); // ������ɱ�����
    file.owner = currentUser->username; // �ļ�������
    file.readPermission = readPermission;
    file.writePermission = writePermission;
    file.permissions[currentUser->username] = { readPermission,writePermission };

    int numBlocks = (content.size() + BLOCK_SIZE - 1) / BLOCK_SIZE; // ������Ҫ�Ŀ���
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
    directory[file.physicalAddress] = name; // ��Ŀ¼�еǼ�
}

// ��ȡ�ļ�����
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

// д���ļ�����
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

    // ��վ����ݣ�д��������
    for (int blockIndex : file.blocks) {
        freeBlock(blockIndex);
    }
    file.blocks.clear();
    file.size = content.size();

    int numBlocks = (content.size() + BLOCK_SIZE - 1) / BLOCK_SIZE; // ������Ҫ�Ŀ���
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

// ɾ���ļ�
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
    directory.erase(file.physicalAddress); // ��Ŀ¼���Ƴ�
}

// ��ʾĿ¼
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