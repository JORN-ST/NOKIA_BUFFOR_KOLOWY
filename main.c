#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define BUFF_SIZE 8
#define ITEM_SIZE 2

#define MAX_WRITE_ITERATOR (8/2)

typedef struct item_t
{
    uint8_t payload[ITEM_SIZE];
} item;

typedef struct ring_t
{
    int32_t read_cursor;
    int32_t write_cursor;
    uint8_t ring_section[BUFF_SIZE];
} __attribute__((packed, aligned(4))) ring;

ring buff;
void init_ring()
{
    buff.read_cursor = 0;
    buff.write_cursor = 0;
}

void write_item(item* it)
{
    if(it == NULL)
        return;

    if(buff.write_cursor >= MAX_WRITE_ITERATOR)
        return;


    memcpy(&buff.ring_section[buff.write_cursor], &it->payload, ITEM_SIZE);
    buff.write_cursor += ITEM_SIZE - 1;
    memset(&buff.ring_section[buff.write_cursor], 0, ITEM_SIZE);

}

void read_item(item* it)
{
    if(it == NULL)
        return;

    buff.write_cursor += ITEM_SIZE - 1;
    memcpy(it->payload, buff.ring_section[buff.read_cursor], ITEM_SIZE);

}

int main()
{
    init_ring();
    struct item_t a, b, c;
    a.payload[0] = 0xA;
    a.payload[1] = 0xB;
    write_item(&a);

    b.payload[0] = 0x8;
    b.payload[1] = 0x9;
    write_item(&b);

    read_item(&c);

    printf("1 read %x %x" ,c.payload[0],c.payload[1]);

    return 0;
}
