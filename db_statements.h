void execute_insert(Statement *statement, Database *db)
{
    //get the number of rows in the table
    unsigned int num_rows = db->num_rows;

    //calculate the byte offset
    unsigned int offset = num_rows * ROW_SIZE;

    //move file pointer to that location
    int offset_set = lseek(db->fd, offset, SEEK_SET);  
    if (offset_set == offset)
    {
        void *destination = malloc(sizeof(ROW_SIZE));
        insert_row_into_file(statement->row, destination);
        
        int res = write(db->fd, destination, ROW_SIZE);
        if (res <= 0)
        {
            printf("ERROR: could not write to the file");
            return;
        }     
        free(destination);
    }
    // printf("SUCCESS: written to file\n");
    db->num_rows++;
}

statement_result insert_statement(Statement *statement, input_buffer *ip_buffer, Database *db)
{
    switch (prepare_insert(statement, ip_buffer))
    {
    case SYNTAX_ERROR:
        return EXECUTE_FAILURE;
    case SYNTAX_OKAY:
        execute_insert(statement, db);
        return EXECUTE_SUCCESS;
    }
}

void execute_select(Statement *statement, Database *db, int *arguments)
{
    if (arguments[0] == 0 && arguments[1] == 1)
    {
        //read everything
        unsigned int num_rows = db->num_rows;
        void *destination = malloc(sizeof(ROW_SIZE));
        Row row;

        //position it to beginning of the file
        lseek(db->fd, 0, SEEK_SET);

        for (int i = 0; i < num_rows; i++)
        {
            read(db->fd, destination, ROW_SIZE);
            lseek(db->fd, (i + 1) * ROW_SIZE, SEEK_SET);
            select_row_from_file(destination, &row);
            print_row(&row);
        }

        free(destination);
    }
}

statement_result select_statement(Statement *statement, input_buffer *ip_buffer, Database *db)
{
    int arguments[2];
    switch (prepare_select(statement, ip_buffer, arguments))
    {
    case SYNTAX_ERROR:
        return EXECUTE_FAILURE;
    case SYNTAX_OKAY:
        print_header();
        execute_select(statement, db, arguments);
        return EXECUTE_SUCCESS;
    }
}