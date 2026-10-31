#include <stdio.h>

#include "overscan.h"
#include "status.h"

static void print_error(const status_code err) {
    switch (err) {
        case STATUS_OK:
            break;
        case STATUS_INVALID_INPUT:
            printf("Ошибка: Некорректный ввод.\n");
            break;
        case STATUS_NULL_PTR:
            printf("Ошибка: Передан нулевой указатель.\n");
            break;
        case STATUS_MEMORY_ERROR:
            printf("Ошибка: Не удалось выделить память.\n");
            break;
        case STATUS_OVERFLOW:
            printf("Ошибка: Переполнение.\n");
            break;
        case STATUS_INVALID_RANGE:
            printf("Ошибка: Значение вне допустимого диапазона.\n");
            break;
        case STATUS_IO_ERROR:
            printf("Ошибка: Сбой ввода-вывода.\n");
            break;
        case STATUS_EOF:
            printf("Ошибка: Достигнут конец ввода.\n");
            break;
        case STATUS_MATCH_ERROR:
            printf("Ошибка: Данные не соответствуют формату.\n");
            break;
        case STATUS_UNSUPPORTED_FORMAT:
            printf("Ошибка: Неподдерживаемый формат.\n");
            break;
        default:
            printf("Неизвестная ошибка.\n");
            break;
    }
}

int main(void) {
    int assigned = 0;
    status_code st;
    int roman_val = 0;
    unsigned int zeck_val = 0U;
    int base_val = 0;
    int std_val = 0;
    FILE *fp;

    printf("=== oversscanf: демо ===\n");

    st = oversscanf("MMXXIV", &assigned, "%Ro", &roman_val);
    if (st != STATUS_OK) {
        print_error(st);
        return st;
    }
    printf("%%Ro MMXXIV -> %d (assigned=%d)\n", roman_val, assigned);

    st = oversscanf("001011", &assigned, "%Zr", &zeck_val);
    if (st != STATUS_OK) {
        print_error(st);
        return st;
    }
    printf("%%Zr 001011 -> %u (assigned=%d)\n", zeck_val, assigned);

    st = oversscanf("1", &assigned, "%Zr", &zeck_val);
    if (st != STATUS_OK) {
        print_error(st);
        return st;
    }
    printf("%%Zr 1 -> %u\n", zeck_val);

    st = oversscanf("ff", &assigned, "%Cv", &base_val, 16);
    if (st != STATUS_OK) {
        print_error(st);
        return st;
    }
    printf("%%Cv ff base16 -> %d\n", base_val);

    st = oversscanf("FF", &assigned, "%CV", &base_val, 16);
    if (st != STATUS_OK) {
        print_error(st);
        return st;
    }
    printf("%%CV FF base16 -> %d\n", base_val);

    /* неверный base -> 10 */
    st = oversscanf("42", &assigned, "%Cv", &base_val, 99);
    if (st != STATUS_OK) {
        print_error(st);
        return st;
    }
    printf("%%Cv 42 base99(->10) -> %d\n", base_val);

    st = oversscanf("XIV 0011 7", &assigned, "%Ro %Zr %d", &roman_val, &zeck_val, &std_val);
    if (st != STATUS_OK) {
        print_error(st);
        return st;
    }
    printf("mix: roman=%d zeck=%u dec=%d assigned=%d\n",
           roman_val, zeck_val, std_val, assigned);

    printf("\n=== overfscanf: файл ===\n");
    fp = fopen("task6_demo_in.txt", "w");
    if (fp == NULL) {
        print_error(STATUS_IO_ERROR);
        return STATUS_IO_ERROR;
    }
    if (fputs("IX 11 aa\n", fp) < 0) {
        fclose(fp);
        print_error(STATUS_IO_ERROR);
        return STATUS_IO_ERROR;
    }
    if (fclose(fp) != 0) {
        print_error(STATUS_IO_ERROR);
        return STATUS_IO_ERROR;
    }

    fp = fopen("task6_demo_in.txt", "r");
    if (fp == NULL) {
        print_error(STATUS_IO_ERROR);
        return STATUS_IO_ERROR;
    }
    st = overfscanf(fp, &assigned, "%Ro %Zr %Cv", &roman_val, &zeck_val, &base_val, 16);
    if (st != STATUS_OK) {
        print_error(st);
        fclose(fp);
        return st;
    }
    printf("file: roman=%d zeck=%u hex=%d assigned=%d\n",
           roman_val, zeck_val, base_val, assigned);
    if (fclose(fp) != 0) {
        print_error(STATUS_IO_ERROR);
        return STATUS_IO_ERROR;
    }

    return STATUS_OK;
}
