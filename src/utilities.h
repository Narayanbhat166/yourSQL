void print_prompt()
{
    printf("your_sql> ");
}

void read_input(input_buffer *ip_buffer)
{
    unsigned int bytes_read = getline(&(ip_buffer->buffer), &(ip_buffer->input_length), stdin);
    // strcpy(ip_buffer->buffer,"insert 1 Narayan greenstreet,karwar 16-06-2000 9448729432 bhatj1964@gmail.com CS530 CSE");
    // ssize_t bytes_read = strlen(ip_buffer->buffer + 1);

    if (bytes_read <= 0)
    {
        printf("There was an error reading the input\n");
        exit(EXIT_FAILURE);
    }

    //remove trailing newline
    ip_buffer->input_length = bytes_read - 1;
    ip_buffer->buffer[bytes_read - 1] = '\0';

    // printf("Line read %s\n", ip_buffer->buffer);
}

void print_welcome_message(){
    printf("Supported statements:\n");
    printf("SELECT usage- select *\n");
    printf("INSERT usage- insert id,name, ..another 6 attributes separated by comma\n");
    printf("UPDATE usage- update name attribute(except name,id) new_value\n");
    printf("DELETE usage- delete name\n");
    printf("All changes are persisted in the file, no need to make commits after changes\n");
    printf("To exit type exit\n");
}

statement_type identify_statement(input_buffer *ip_buffer)
{

    char *buffer = ip_buffer->buffer;
    char supported_statements[4][10];
    strcpy(supported_statements[0], "select");
    strcpy(supported_statements[1], "insert");
    strcpy(supported_statements[2], "update");
    strcpy(supported_statements[3], "delete");

    for (unsigned int i = 0; i < 4; i++)
    {
        if (strncmp(buffer, supported_statements[i], 6) == 0)
        {
            return i;
        }
    }

    return NOT_RECOGNIZED;
}

statement_result prepare_insert(Statement *statement, input_buffer *ip_buffer)
{
    //tokenise the statement insert 1 Narayan greenstreet,karwar 16-06-2000 9448729432 bhatj1964@gmail.com CS530 CSE
    char *keyword = strtok(ip_buffer->buffer, " ");
    char *id_string = strtok(NULL, ",");
    char *name = strtok(NULL, ",");
    char *address = strtok(NULL, ",");
    char *dob = strtok(NULL, ",");
    char *phone = strtok(NULL, ",");
    char *mail = strtok(NULL, ",");
    char *course = strtok(NULL, ",");
    char *branch = strtok(NULL, ",");
    

    if (id_string == NULL || name == NULL || address == NULL || dob == NULL || phone == NULL || mail == NULL || course == NULL || branch == NULL)
    {
        printf("ERROR: Required 8 arguments\n");
        return SYNTAX_ERROR;
    }

    int id = atoi(id_string);
    if (id < 0)
    {
        printf("ERROR: id cannot be negative\n");
        return SYNTAX_ERROR;
    }

    if (strlen(name) > 30)
    {
        printf("ERROR: name cannot be more than 30 characters\n");
        return SYNTAX_ERROR;
    }

    statement->row->id = id;
    strcpy(statement->row->name, name);
    strcpy(statement->row->address,address);
    strcpy(statement->row->dob,dob);
    strcpy(statement->row->phone,phone);
    strcpy(statement->row->mail,mail);
    strcpy(statement->row->course,course);
    strcpy(statement->row->branch,branch);

    return SYNTAX_OKAY;
}

statement_result prepare_select(Statement *statement, input_buffer *ip_buffer, int *arguments)
{
    //tokenise the statement
    char *keyword = strtok(ip_buffer->buffer, " ");
    char *arg1 = strtok(NULL, " ");
    char *arg2 = strtok(NULL, " ");

    if ( arg1 != NULL && strcmp(arg1, "*") == 0)
    {
        arguments[0] = 0;
        arguments[1] = 1;
        return SYNTAX_OKAY;
    }

    if (arg1 == NULL || arg2 == NULL)
    {
        printf("ERROR: Required 2 arguments\n");
        return SYNTAX_ERROR;
    }

    char attributes[2][10] = {"id", "name"};

    int k = 0;
    for (unsigned int i = 0; i < 2; i++)
    {
        if (strcmp(arg1, attributes[i]) == 0)
        {
            arguments[k++] = i;
        }
    }

    for (unsigned int i = 0; i < 2; i++)
    {
        if (strcmp(arg2, attributes[i]) == 0)
        {
            arguments[k++] = i;
        }
    }

    if (k != 2)
        return SYNTAX_ERROR;
    return SYNTAX_OKAY;
}

statement_result prepare_delete(input_buffer* ip_buffer,char** name){
    //tokenise the statement
    char *keyword = strtok(ip_buffer->buffer, " ");
    char *arg1 = strtok(NULL, " ");

    if(arg1 == NULL  ){
        printf("ERROR: Required 1 argument(Name)\n");
        return SYNTAX_ERROR;
    }

    *name = (char* )malloc((strlen(arg1) + 1) * sizeof(char));
    strcpy(*name,arg1);

    return SYNTAX_OKAY;   
}

statement_result prepare_update(input_buffer* ip_buffer,int *attribute_id,char** name,char** attribute_value){
    //update narayan email hola@bhola.com -> updates email attribute of narayan to hola@bhola.com
    //tokenise the statement

    char* keyword = strtok(ip_buffer->buffer, " ");
    char* name_arg = strtok(NULL," ");
    char* attribute_arg = strtok(NULL," ");
    char* value_arg = strtok(NULL," ");

    if(name_arg == NULL || attribute_arg == NULL || value_arg == NULL){
        printf("ERROR: Required 3 arguments\n");
        return SYNTAX_ERROR;
    }

    for(int i = 2;i<TOTAL_ATTRIBUTES;i++){
        if(strcmp(attribute_arg,attributes_of_student[i]) == 0){
            *attribute_id = i;
            break;
        }
    }

    *name = (char*)malloc((strlen(name_arg) + 1) * sizeof(char));
    strcpy(*name,name_arg);

    *attribute_value = (char*)malloc((strlen(value_arg) + 1) * sizeof(char));
    strcpy(*attribute_value,value_arg);

    return SYNTAX_OKAY;
        
}

void insert_row_into_file(Row *source, void *destination)
{
    memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
    strncpy(destination + NAME_OFFSET, (source->name), NAME_SIZE);
    strncpy(destination + ADDRESS_OFFSET, (source->address), ADDRESS_SIZE);
    strncpy(destination + DOB_OFFSET, (source->dob), DOB_SIZE);
    strncpy(destination + PHONE_OFFSET, (source->phone), PHONE_SIZE);
    strncpy(destination + MAIL_OFFSET, (source->mail), MAIL_SIZE);
    strncpy(destination + COURSE_OFFSET, (source->course), COURSE_SIZE);
    strncpy(destination + BRANCH_OFFSET, (source->branch), BRANCH_SIZE);
}

// function to retrieve a row from file
void select_row_from_file(void *source, Row *destination)
{
    memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
    memcpy(&(destination->name), source + NAME_OFFSET, NAME_SIZE);
    memcpy(&(destination->address), source + ADDRESS_OFFSET, ADDRESS_SIZE);
    memcpy(&(destination->dob), source + DOB_OFFSET, DOB_SIZE);
    memcpy(&(destination->phone), source + PHONE_OFFSET, PHONE_SIZE);
    memcpy(&(destination->mail), source + MAIL_OFFSET, MAIL_SIZE);
    memcpy(&(destination->course), source + COURSE_OFFSET, COURSE_SIZE);
    memcpy(&(destination->branch), source + BRANCH_OFFSET, BRANCH_SIZE);   
}

void print_row(Row *row)
{
    printf("%-3d | %-15s | %-30s | %-10s | %-10s | %-20s | %-6s | %-6s\n", 
    row->id, row->name,row->address,row->dob,row->phone,row->mail,row->course,row->branch);
}

void print_header()
{
    printf("\n");
    printf("%-3s | %-15s | %-30s | %-10s | %-10s | %-20s | %-6s | %-6s\n", 
    "ID","NAME","ADDRESS","DOB","PHONE","EMAIL","COURSE","BRANCH");
    for(int i=0;i<122;i++)
        printf("-");
    printf("\n");
}