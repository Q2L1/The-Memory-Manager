#ifndef _OS_MEM_H
#define _OS_MEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

struct mem_handle_t {
    // ����� ���������� ����� ������������ ������ ���� ������ (� ������)
    int addr;
    // ������ ����� � ������
    int size;

    mem_handle_t(int addr, int size) :
        addr(addr),
        size(size)
    {}

    bool operator==(const mem_handle_t& other) { return other.addr == addr && other.size == size; }
    bool operator!=(const mem_handle_t& other) { return !operator==(other); }
}; // ���������� ����� ������

// ��������� ��������� ������
// ������� �������� � ������������ � �������� ��������, ��� ������� �������� ��������������� ����������, ���, ������ �� ����� load <page_num> ��������,
// ��� ����� ������ ����� load � ����� �������� page_num, �������� load 5 (��� ������� ��������).
typedef struct
{
    // �������� ��������� ������,
    // size: ��� ���������� ������ - ������ ���� ������ � ������, ��� garbage collection - ������������ ���������� ��������, ��� ���������� ���������� - ������ ����� �������� ������
    // num_pages: ��� ���������� ���������� - ���������� ���������� �������
    // ������������ ��������: 1 - �����, 0 - �������
    int (*create)(int size, int num_pages);

    // �������� ��������� ������
    // ������������ ��������: 1 - �����, 0 - ������� (��������, �������� �� ��� ������)
    int (*destroy)();

    // ��������� ����� ������ �������� block_size ����
    // ��� ���������� ������ � ������������� �������� ����� ������ �������� ������� ������������.
    // ������������ ��������: ���������� ����������� ����� - �����, ������� ���������� {0,0} - �������
    mem_handle_t(*alloc)(int block_size);

    // ������������ ����� ������ � ������������ h
    // ������������ ��������: 1 - �����, 0 - ������� (��������, �������� ����������)
    // �������� �������� ������ ������������ �������� h � ������ ����������� ��������� ���������� ����
    int (*free)(mem_handle_t h);

    // ���������� ���������� ����������� ����� ������, ������� ����������� ��������� ������
    // � ������, ���� ������ ����� �� ����������, ������������ ������� ���������� {0,0}
    mem_handle_t(*get_block)(int addr, int size);

    // ������������ ��������: ������������ ������ ����� ������ (� ������), ������� ����� ���� ������ �������
    int (*get_max_block_size)();

    // ������������ ��������: ��������� ����� ��������� ������ � ������
    int (*get_free_space)();

    // ������������� ��� ���������� ����� ������ � ������� <�ddr> <size>
    // ������� ������ - �� ����������� �������.
    void (*print_blocks)();

    //////////////////////////////////////////////////////////
    // Garbage collection
    //////////////////////////////////////////////////////////

    // �������� �������
    // ����� ����� ������� ���������� 32 ���������
    // ������������ ��������: 1 - �����, 0 - ������� (��������, ���� ������ � ����� ������ ��� ����������)
    int (*create_object)(const char* name);

    // ����������� �������
    // ��� ���� ��������� ��� ��������� �� ������� ������
    // ������ �������� ����� ������
    // ������������ ��������: 1 - �����, 0 - ������� (��������, ���� ������ ������� �� ����������)
    int (*destroy_object)(const char* name);

    // �������� ����� ���� ��������� � �� ������������ �������� � ���������� �������
    void (*print_objects)();

    // ������ ������ � �������� ������ ��������
    // ����������� ������������� ��������� �������� ��������
    // ������������ ��������: 1 - �����, 0 - ������� (��������, ���� ������ ������� �� ����������)
    int (*set_root)(const char* name);

    // ������� ������ �� ������� object1 �� ������ object2
    // ���������� ������ �� ������� ������� ���������� ������ 16
    // ������������ ��������: 1 - �����, 0 - ������� (��������, ���� ������ �� �������� �� ����������)
    int (*link)(const char* object1_name, const char* object2_name);

    // ��������� � �������� ��������� ����� ��������.
    // ������ ��������� ������� ���� root (��������), � ����� ��� �������, ��������� �� ������
    // ����� �������� �� �������
    // ����� �������� �������������� � ���������� �������
    void (*collect_live_objects)(void);

    // ��������� � �������� ��������� ������� ������ � ��������, �.�. ������, ����������� �� 
    // ��������� �������. ��� ����� ������� ���� ���� ��������, ��������� � ���������
    // � ������� create_object � destroy_object
    // ������ ������ ������: <object1_name>,<ref_num>
    // ����� �������������� ����� �������������� �������� � ���������� �������, � ����� �� ������ ������ �� �����������
    void (*collect_dead_links)(void);

    // ������������� ��� ������� ������� ���������� ������ �� ����
    // ��������� ������ ������, ������� �� �� ��������� ��������
    // ������ ������: <object_name> <link_count>
    // ����� �������� �������������� � ���������� �������
    void (*print_link_counts)();

    //////////////////////////////////////////////////////////
    // Paging
    //////////////////////////////////////////////////////////

    // ���������� �������� ���������� ������ real_page � ���� � ������ page_<virtual_page>
    // ������ ������������ � �������� �������
    // � ������ ������ ������� �� ����� ���������: save_page <real_page> <virtual_page>
    // ������������ ��������: 1 - �����, 0 - ������� (��������, ���� �� �����������)
    int (*save_page)(int real_page, int virtual_page);

    // ��������� �� ����� � ������ page_<virtual_page> �������� ������ � ��������� �� � ���������� �������� real_page.
    // ������ ����������� � �������� �������
    // ���� ���� � ����� ������ �����������, ���������� �������� ����������� ������.
    // � ������ ������ ������� �� ����� ���������: load_page <virtual_page> <real_page>
    // ������������ ��������: 1 - ����� (������������� ���� ����� �������� �������), 0 - ������� (�������� ����� �������� ������ ���� ��� ������ ������ ����� �������)
    int (*load_page)(int virtual_page, int real_page);

    // ���������� �� ���������� ������������ ������ addr ��������� �������� (����) val
    // � ������, ���� ��������������� ����������� �������� ����������� � ������, ������ �� ������������
    // ������������ ��������: 1 - �����, 2 - ����������� �������� �����������, 0 - ������� (��������, ����� ������ 0)
    int (*set_byte)(int addr, char val);

    // ��������� �������� (����) �� ������ ����������� ������, ������������� �� ���������� ������ addr, � ������ � ����� dst
    // ������������ ��������: 1 - �����, 2 - ����������� �������� �����������, 0 - ������� (��������, ����� ������ 0)
    int (*get_byte)(int addr, char* dst);

    // ���������� ����� ����������� �������� ������, ���������� ��������� ����
    // ������������ ��������: ����� �������� � ������ ������, -1 � ������ ������� (���� ��������� ���� ������� �� ���������� �� � ����� ��������)
    int (*page_num)(mem_handle_t block);

    // ���������� ��������� � ������ ���� ������
    // � ������ ������ ����� ������� �� ����� ��������� � �������: read <addr> <size> <ppa>
    // ��� ppa - ����� ���������� ��������, � ������� ���������� ���������
    // ���� ������ ���������� �� ���������
    // ������������ ��������: 1 - �����, -1 - ������� (���� ��������� ������ �� ����� ������ ����� ��������)
    int (*read)(int addr, int size);

    // ���������� ��������� � ������ ���� ������
    // � ������ ������ ����� ������� �� ����� ��������� � �������: write <addr> <size> <ppa>
    // ��� ppa - ����� ���������� ��������, � ������� ���������� ���������
    // ���� ������ ���������� �� ���������
    // ������������ ��������: 1 - �����, -1 - ������� (���� ��������� ������ �� ����� ������ ����� ��������)
    int (*write)(int addr, int size);

} memory_manager_t;

void setup_memory_manager(memory_manager_t* mm);

#endif
