#include <stdio.h>

#define AXI_DATA_WIDTH 32

typedef struct {
    unsigned int addr;
    unsigned int data[AXI_DATA_WIDTH];
} AXI_Transaction;

void axi_write(AXI_Transaction *transaction) {
    printf("Writing data to address %x\n", transaction->addr);
    unsigned int data[AXI_DATA_WIDTH];
    for (int i =0; i < sizeof(transaction->data); i++){
            scanf("%u", data[i]);
    }
    // Здесь можно добавить логику для записи данных в указанный адрес
    transaction->data == data;
}

void axi_read(AXI_Transaction *transaction) {
    printf("Reading data from address %x\n", transaction->addr);
    // Здесь можно добавить логику для чтения данных из указанного адреса
    printf("Written data\n"); 
    for (int i =0; i < sizeof(transaction->data); i++){
        printf("%u\n", transaction->data[i]);
    }
}

int main() {
    unsigned int data[AXI_DATA_WIDTH];

    AXI_Transaction tx_write = {0x1000, {0xAA, 0xBB, 0xCC, 0xDD}};
    //AXI_Transaction tx_read = {0x2000, {0x00}};

    axi_write(&tx_write);
    axi_read(&tx_write);

    return 0;
}
