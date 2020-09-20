//function to open the file
Database *database_init(char *filename)
{
    //Initialize and allocate a new db object
    Database *db = malloc(sizeof(Database));

    //open the file in append mode(supports reading and writing)
    db->fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
    if (db->fd == -1)
    {
        printf("FATAL: cannot open file\n");
        exit(EXIT_FAILURE);
    }

    long file_length = lseek(db->fd, 0, SEEK_END);
    unsigned int num_rows = file_length / ROW_SIZE;

    db->num_rows = num_rows;
    printf("Read %d rows from file\n", num_rows);

    return db;
}

//function to close database files
void database_close(Database *db)
{
    free(db);
    printf("Bo bye\n");
}

input_buffer *construct_input_buffer()
{
    input_buffer *ip_buffer = malloc(sizeof(input_buffer));

    ip_buffer->buffer = NULL;
    ip_buffer->input_length = 0;

    return ip_buffer;
}

void close_input_buffer(input_buffer *ip)
{
    free(ip->buffer);
    free(ip);
}