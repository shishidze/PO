#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xil_io.h"
#include "xparameters.h"

// Define base addresses for each AXI Lite block
#define AXI_BASEADDR_BLOCK1 XPAR_AXI_BUS_INTERFACE_0_BASEADDR

// Define register offsets for the control signals
#define START_REG 0x00
#define DEPTH_REG 0x04
#define WRITE_ENABLE_REG 0x08
#define TCP_IP_FRAG_REG 0x0C
#define SEND_ENABLE_REG 0x10

// Function to write to an AXI Lite register
void write_axi_reg(uint32_t base_addr, uint32_t reg_offset, uint32_t value) {
    Xil_Out32(base_addr + reg_offset, value);
}

uint32_t read_from_axi(uint32_t base_addr, uint32_t offset){
    return Xil_In32(base_addr + offset);
}

// Function to read configuration file and write to AXI Lite registers
void read_config_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open config file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *key = strtok(line, "=");
        char *value_str = strtok(NULL, "\n");

        if (key && value_str) {
            uint32_t value = atoi(value_str);
            if (strcmp(key, "start") == 0) {
                write_axi_reg(START_REG, value);
            } else if (strcmp(key, "depth") == 0) {
                write_axi_reg(DEPTH_REG, value);
            } else if (strcmp(key, "write_enable") == 0) {
                write_axi_reg(WRITE_ENABLE_REG, value);
            } else if (strcmp(key, "tcp_ip_fragment") == 0) {
                write_axi_reg(TCP_IP_FRAG_REG, value);
            } else if (strcmp(key, "send_enable") == 0) {
                write_axi_reg(SEND_ENABLE_REG, value);
            }
        }
    }
    fclose(file);
}

int main() {

    init_platform();

    read_config_file("config.txt");

    return 0;
}