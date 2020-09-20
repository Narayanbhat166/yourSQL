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

statement_result execute_delete(Database* db,char** name){
    //find the name
    unsigned int num_rows = db->num_rows;
    void *destination = malloc(sizeof(ROW_SIZE));
    Row row;
    char temp_name[100];

    //position it to beginning of the file
    lseek(db->fd, 0, SEEK_SET);
    int i;

    for (i = 0; i < num_rows; i++)
    {
        read(db->fd, destination, ROW_SIZE);
        lseek(db->fd, (i + 1) * ROW_SIZE, SEEK_SET);
        // select_row_from_file(destination, &row);
        
        memcpy(temp_name, destination + NAME_OFFSET, NAME_SIZE);
        if(strcmp(*name,temp_name) == 0){
            //delete
            //copy all next recores to one step behind
            break;
        }
        printf("Name read %s",temp_name);
        // print_row(&row);
    }

    //see if end of file is reached
    // unsigned int end_offset = lseek(db->fd,0,SEEK_END);
    // unsigned int current_offset = lseek(db->fd,0,SEEK_CUR);
    if(i != db->num_rows){
        for (; i < num_rows; i++)
            {
                //read next row
                lseek(db->fd, (i + 1) * ROW_SIZE, SEEK_SET);
                read(db->fd, destination, ROW_SIZE);
                
                select_row_from_file(destination, &row);

                lseek(db->fd,i * ROW_SIZE,SEEK_SET);
                int res = write(db->fd, destination, ROW_SIZE);
                if (res <= 0)
                {
                    printf("ERROR: could not write to the file");
                    return EXIT_FAILURE;
                }   
                

                printf("Name read %s",temp_name);
                // print_row(&row);
            }
            db->num_rows--;
            if(!ftruncate(db->fd,db->num_rows * ROW_SIZE)){
                printf("TRUNCATED\n");
            }
    }

    free(destination);
    return EXECUTE_SUCCESS;

}

statement_result delete_statement(Statement *statement, input_buffer *ip_buffer, Database *db){
    //supports only delete by name
    char* name;
    switch(prepare_delete(ip_buffer,&name)){
        case SYNTAX_ERROR:
            return EXECUTE_FAILURE;
        case SYNTAX_OKAY:
            printf("Delete %s",name);
            execute_delete(db,&name);
            free(name);
            return EXECUTE_SUCCESS;
    }
}