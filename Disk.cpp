#include <Disk.h>
#include <cstring>

Disk::Disk():blocks(TOTAL_BLOCKS){}

void writeBlock(int blockIndex, const char* data, int size) {
    if (blockIndex >= 0 && blockIndex < TOTAL_BLOCKS) {
        std::memcpy(blocks[blockIndex].data, data, size);
    }
}

// ��ȡָ���������
void readBlock(int blockIndex, char* buffer, int size) {
    if (blockIndex >= 0 && blockIndex < TOTAL_BLOCKS) {
        std::memcpy(buffer, blocks[blockIndex].data, size);
    }
}
