#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#define OPTION_CREATE_BOOK 0
#define OPTION_READ_BOOK 1
#define OPTION_LIST_BOOKS 2

#define BOOK_FLAG_SOLD 0b00000001

typedef unsigned char BOOK_FLAGS;

struct book
{
    char title[20];
    char description[200];
    BOOK_FLAGS flags;
};

FILE* f = NULL;
struct  book*ptr = NULL;

int handle_create()
{
    struct book b;

    memset(&b, 0, sizeof(b));

    printf("Enter the books name:\n");
    fgets(b.title, sizeof(b.title),stdin);
    printf("Book name: %s\n",b.title);

    printf("Enter the books description:\n");
    fgets(b.description,sizeof(b.description),stdin);
    printf("Book description: %s\n",b.description);

    printf("Has the book sold?: (Y/N)%s\n");
    char c = 0;
    scanf("%c",&c);

    if (c == 'Y' || c == 'y')
    {
        b.flags |= BOOK_FLAG_SOLD;
    }
    

    fseek(f, 0, SEEK_END);

    fwrite(&b, sizeof(b),1,f);
}

int how_many_books()
{
    fseek(f, 0, SEEK_END);
    unsigned long total_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    return total_size/sizeof(struct book);
}

void view_book(struct book* book)
{
    printf("Title: %s\n", book->title);
    printf("Description: %s\n", book->description);

    if (book->flags & BOOK_FLAG_SOLD)
    {
        printf("This book was sold:\n");
    }
}

int handle_list()
{
    struct book b;
    fseek(f, 0, SEEK_SET);

    int index = 1;

    int total_books = how_many_books();

    ptr = (struct book*) malloc(sizeof(struct book) * total_books);

    while(fread(&b,sizeof(b),1,f)==1)
    {
        memcpy(&ptr[index], &b, sizeof(b));
        printf("%d - %s\n",index, b.title);
        printf("%s\n", b.description);
        index++;
    }

    int option = 0;

    printf("Choose a book: ");
    scanf("%d", &option);

    if (option < 0 || option > total_books)
    {
        printf("Invalid book.");
        return -1;
    }

    view_book(&ptr[option]);

    free(ptr);
    return 0;
}

int setup_file()
{

    f = fopen("./data.bin", "rb+");

    if (!f)
    {
        printf("Failed to open file\n");

        return -1;
    }
    return 0;
}

int choose_option(){

    int option = 0;
    printf("Choose an option: ");
    scanf("%i", &option);
    getchar();

    int res = 0;

    switch (option)
    {
        case OPTION_CREATE_BOOK:
            handle_create();

            if (res < 0)
            {
                return res;
            }        
            break;
        case OPTION_READ_BOOK:
            printf("You selected to read a book.");
            break;
    
        case OPTION_LIST_BOOKS:
            handle_list();
            break;
        default:
        printf("Invalid option choosen");
            return -1;
            break;
    }

    return 0;
}

int main(int argc, char** argv)
{
    if(setup_file() < 0){
        return -1;
    }


    if (choose_option() < 0)
    {
        return -1;
    }

    fclose(f);   
    return 0;
}
