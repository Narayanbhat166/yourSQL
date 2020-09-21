#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <time.h>

#include "definitions.h"
#include "house_keeping.h"
#include "utilities.h"
#include "db_statements.h"

int main(int argc, char *argv[])
{
    // if (argc < 2)
    // {
    //     printf("Usage: ./a.out fiilename\n");
    //     exit(EXIT_SUCCESS);
    // }

    char *filename = "dbfile";
    Database *db = database_init(filename);
    input_buffer *ip_buffer = construct_input_buffer();

    Statement statement;
    statement.row = malloc(sizeof(Row));

    clock_t start, end;
    double cpu_time_used;
    
    
    while (true)
    {
        print_prompt();
        read_input(ip_buffer);

        if (strcmp(ip_buffer->buffer, "exit") == 0)
        {
            database_close(db);
            close_input_buffer(ip_buffer);
            exit(EXIT_SUCCESS);
        }

        else
        {
            start = clock();
            switch (statement.type = identify_statement(ip_buffer))
            {
            case SELECT_STATEMENT:
                select_statement(&statement, ip_buffer, db);
                break;
            case INSERT_STATEMENT:
                insert_statement(&statement, ip_buffer, db);
                break;
            case UPDATE_STATEMENT:
                update_statement(&statement, ip_buffer, db);
                break;
            case DELETE_STATEMENT:
                delete_statement(&statement,ip_buffer,db);
                break;
            default:
                printf("ERROR: statement not recognized\n");
                continue;
            }
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            printf("QUERY OK, took %0.3fs\n\n",cpu_time_used);
        }
        free(ip_buffer->buffer);
        ip_buffer->buffer = NULL;
    }

    return 0;
}