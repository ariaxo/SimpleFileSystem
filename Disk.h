#ifndef DISK_H
#define DISK_H
#include<vector>

using namespace std;
// 模拟磁盘块大小
const int BLOCK_SIZE = 512;
// 模拟磁盘总块数
const int TOTAL_BLOCKS = 1024;

// 磁盘块结构
struct DiskBlock {
    char data[BLOCK_SIZE];
};

// 模拟磁盘类
class Disk {
public:
    Disk() : blocks(TOTAL_BLOCKS) {}

    // 写入数据到指定块
    void writeBlock(int blockIndex, const char* data, int size)

    // 读取指定块的数据
    void readBlock(int blockIndex, char* buffer, int size)

private:
    std::vector<DiskBlock> blocks;
};

#endif // !DISK_H

