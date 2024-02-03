#include "os_mem.h"
#include "stdio.h"
#include "stdlib.h"


typedef struct physical_memory {

    int numvirtmem;
    char meter;
    int last_accessed_time;

} physical_memory;

physical_memory* maneger = NULL; 
int pagesize;
int numpage;
char memory = 0;

// Функция для создания и инициализации менеджера памяти
int my_create(int size, int num_pages) { //принимает два аргумента размер страницы и количество страниц

    if (memory == 0) { 
        maneger = (physical_memory*)malloc(sizeof(physical_memory) * num_pages);
        if (maneger != NULL) { 
            numpage = num_pages;
            pagesize = size;
            for (int i = 0; i < num_pages; i++) {

                maneger[i].meter = 0;
                maneger[i].numvirtmem = -1;
            }
            memory = 1;
            return 1;
        }
    }
    return 0;
}

// Функция для уничтожения выделенной памяти
int my_destroy() {

    if (memory == 1) {

        free(maneger);
        maneger = NULL;
        memory = 0;
        return 1;
    }
    return 0;
}

// Функция для получения номера страницы на основе адреса блока памяти
int my_page_num(mem_handle_t block) {

    if (memory == 0 || block.addr / pagesize != (block.addr + block.size - 1) / pagesize)
        return -1;
    else return block.addr / pagesize;
}

// Функция для выполнения операции чтения по указанному адресу памяти
int my_read(int addr, int size) {

    if (memory == 1 && addr >= 0 && size > 0 && addr / pagesize == (addr + size - 1) / pagesize) {

        int num = addr / pagesize;
        int i = 0;
        int min_access_time = maneger[0].last_accessed_time;
        int min_access_index = 0; 

        while (i < numpage && maneger[i].numvirtmem != -1) { 

            if (num == maneger[i].numvirtmem) {

                printf("read %d %d %d\n", addr, size, i);
                maneger[i].last_accessed_time = 0;
                for (int j = 0; j < numpage && maneger[j].numvirtmem != -1; j++)
                    if (j != i)
                        maneger[j].last_accessed_time++;
                return 1;
            }
            if (maneger[i].last_accessed_time > min_access_time) { 

                min_access_time = maneger[i].last_accessed_time; 
                min_access_index = i;
            }
            i++;
        }
        int t = i;
        if (i == numpage)
            t = min_access_index; 
        if (maneger[t].meter == 1) { 
            printf("save %d %d\n", t, maneger[t].numvirtmem);
        }
        printf("load %d %d\n", num, t);
        maneger[t].numvirtmem = num; 
        maneger[t].meter = 0; 
        maneger[t].last_accessed_time = 0; 
        printf("read %d %d %d\n", addr, size, t);
        for (int j = 0; j < numpage && maneger[j].numvirtmem != -1; j++)
            if (j != t)
                maneger[j].last_accessed_time++;
        return 1;
    }
    return -1;
}

// Функция для выполнения операции записи по указанному адресу памяти
int my_write(int addr, int size) {

    if (memory == 1 && addr >= 0 && size > 0 && addr / pagesize == (addr + size - 1) / pagesize) { 
        int num = addr / pagesize;
        int i = 0;
        int min_access_time = maneger[0].last_accessed_time;
        int min_access_index = 0;

        while (i < numpage && maneger[i].numvirtmem != -1) { 

            if (num == maneger[i].numvirtmem) {

                maneger[i].meter = 1;
                printf("write %d %d %d\n", addr, size, i);
                maneger[i].last_accessed_time = 0; 
                for (int j = 0; j < numpage && maneger[j].numvirtmem != -1; j++)
                    if (j != i)
                        maneger[j].last_accessed_time++;
                return 1;
            }
            if (maneger[i].last_accessed_time > min_access_time) {

                min_access_time = maneger[i].last_accessed_time;
                min_access_index = i;
            }
            i++;
        }
        int t = i;
        if (i == numpage)
            t = min_access_index;
        if (maneger[t].meter == 1) {
            printf("save %d %d\n", t, maneger[t].numvirtmem);
        }
        printf("load %d %d\n", num, t);
        maneger[t].numvirtmem = num;
        maneger[t].meter = 1;
        maneger[t].last_accessed_time = 0;
        printf("write %d %d %d\n", addr, size, t);
        for (int j = 0; j < numpage && maneger[j].numvirtmem != -1; j++)
            if (j != t)
                maneger[j].last_accessed_time++;

        return 1;
    }
    return -1;
}

// Функция для настройки менеджера памяти
void setup_memory_manager(memory_manager_t* mm) {

    mm->create = my_create;
    mm->destroy = my_destroy;
    mm->page_num = my_page_num;
    mm->read = my_read;
    mm->write = my_write;
}