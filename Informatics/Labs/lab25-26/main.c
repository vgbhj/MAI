#include <stdio.h>
#include "udt.h"

void print_menu(){
    printf("Выберите действие: \n");
    printf("1. Добавить элемент в начало\n");
    printf("2. Добавить элемент в конец\n");
    printf("3. Удалить первый элемент\n");
    printf("4. Удалить последний элемент\n");
    printf("5. Вывести список\n");
    printf("6. Размер списка\n");
    printf("7. Вставить элемент на позицию(элемент, позиция)\n");
    printf("8. Сортировка списка\n");
    printf("0. Выход\n");
}

int main(){
    Udt list;
    udt_create(&list);
    int value;
    int pos;
    char c;
    print_menu();
    while ((c = getchar()) != EOF)
    {
        if (c == '\n') continue;
        switch (c){
            case '1':
                scanf("%d", &value);
                udt_push_front(&list, value);
                break;
            case '2':
                scanf("%d", &value);
                udt_push_back(&list, value);
                break;
            case '3':
                udt_pop_front(&list);
                break;
            case '4':
                udt_pop_back(&list);
                break;
            case '5':
                udt_print(&list);
                break;
            case '6':
                printf("%zu ", udt_size(&list));
                break;
            case '7':
                scanf("%d", &value);
                scanf("%d", &pos);
                udt_insert(&list, pos, value);
                break;
            case '8':
                udt_sort(&list);
                break;
            default:
                printf("Error!\n");
                return 0;
        }
        printf("\n");
        printf("Выберите действие: \n");
    }
}
