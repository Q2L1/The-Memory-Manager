#include "stdio.h"
#include "stdlib.h"
#include "os_mem.h"

int main() {

    memory_manager_t mm;
    setup_memory_manager(&mm);

    mm.create(11, 3) != 0;
    mm.read(6, 2);
    mm.read(13, 2);
    mm.read(24, 2);
    mm.read(36, 2);
    mm.read(7, 2);
    mm.read(17, 2);
    mm.read(44, 2);
    mm.read(8, 2);
    mm.read(12, 2);
    mm.read(25, 2);
    mm.read(37, 2);
    mm.read(46, 2);
    mm.destroy();
    mm.create(11, 4) != 0;
    mm.write(6, 2);
    mm.write(13, 2);
    mm.write(24, 2);
    mm.write(36, 2);
    mm.write(7, 2);
    mm.write(17, 2);
    mm.read(44, 2);
    mm.write(8, 2);
    mm.write(12, 2);
    mm.write(25, 2);
    mm.read(21, 2);
    mm.write(37, 2);
    mm.write(46, 2);
    mm.destroy();
    return 0;
}