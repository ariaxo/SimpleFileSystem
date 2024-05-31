#ifndef DISK_H
#define DISK_H
#include<vector>

using namespace std;
// ģ����̿��С
const int BLOCK_SIZE = 512;
// ģ������ܿ���
const int TOTAL_BLOCKS = 1024;

// ���̿�ṹ
struct DiskBlock {
    char data[BLOCK_SIZE];
};

// ģ�������
class Disk {
public:
    Disk() : blocks(TOTAL_BLOCKS) {}

    // д�����ݵ�ָ����
    void writeBlock(int blockIndex, const char* data, int size)

    // ��ȡָ���������
    void readBlock(int blockIndex, char* buffer, int size)

private:
    std::vector<DiskBlock> blocks;
};

#endif // !DISK_H

