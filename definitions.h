#define true 1
#define false 0
#define COLUMN_NAME_SIZE 30
#define COLUMN_ADDRESS_SIZE 50
#define COLUMN_PHONE_SIZE 11
#define COLUMN_DOB_SIZE 11
#define COLUMN_MAIL_SIZE 30
#define COLUMN_COURSE_SIZE 6
#define COLUMN_BRANCH_SIZE 6

typedef struct
{
    unsigned int id;
    char name[COLUMN_NAME_SIZE];
    char address[COLUMN_ADDRESS_SIZE];
    char dob[COLUMN_DOB_SIZE];
    char phone[COLUMN_PHONE_SIZE];
    char mail[COLUMN_MAIL_SIZE];
    char course[COLUMN_COURSE_SIZE];
    char branch[COLUMN_BRANCH_SIZE];
} Row;

#define size_of_attribute(Struct, Attribute) sizeof(((Struct *)0)->Attribute)
const unsigned int ID_SIZE = size_of_attribute(Row, id);
const unsigned int NAME_SIZE = size_of_attribute(Row, name);
const unsigned int ADDRESS_SIZE = size_of_attribute(Row, address);
const unsigned int DOB_SIZE = size_of_attribute(Row, dob);
const unsigned int PHONE_SIZE = size_of_attribute(Row, phone);
const unsigned int MAIL_SIZE = size_of_attribute(Row, mail);
const unsigned int COURSE_SIZE = size_of_attribute(Row, course);
const unsigned int BRANCH_SIZE = size_of_attribute(Row, branch);

const unsigned int ROW_SIZE = ID_SIZE + NAME_SIZE + ADDRESS_SIZE + DOB_SIZE + PHONE_SIZE + MAIL_SIZE + COURSE_SIZE + BRANCH_SIZE;

const unsigned int ID_OFFSET = 0;
const unsigned int NAME_OFFSET = ID_OFFSET + ID_SIZE;
const unsigned int ADDRESS_OFFSET = NAME_OFFSET + NAME_SIZE;
const unsigned int DOB_OFFSET = ADDRESS_OFFSET + ADDRESS_SIZE;
const unsigned int PHONE_OFFSET = DOB_OFFSET + DOB_SIZE;
const unsigned int MAIL_OFFSET = PHONE_OFFSET + PHONE_SIZE;
const unsigned int COURSE_OFFSET = MAIL_OFFSET + MAIL_SIZE;
const unsigned int BRANCH_OFFSET = COURSE_OFFSET + COURSE_SIZE;

#define TOTAL_ATTRIBUTES 8

extern char** attributes_of_student;
extern unsigned int attribute_size_of_student[];
extern unsigned int attribute_offset_of_student[];

char** attributes_of_student = (char *[]){"id","name","address","dob","phone","email","course","branch"};
unsigned int attribute_size_of_student[] = {ID_SIZE,NAME_SIZE,ADDRESS_SIZE,DOB_SIZE,PHONE_SIZE,MAIL_SIZE,COURSE_SIZE,BRANCH_SIZE};
unsigned int attribute_offset_of_student[] = {ID_OFFSET,NAME_OFFSET,ADDRESS_OFFSET,DOB_OFFSET,PHONE_OFFSET,MAIL_OFFSET,COURSE_OFFSET,BRANCH_OFFSET};

typedef struct
{
    int fd;
    unsigned int num_rows;
} Database;

typedef struct
{
    unsigned int type;
    Row *row;
} Statement;

typedef struct
{
    char *buffer;
    size_t input_length;
} input_buffer;

typedef enum
{
    SELECT_STATEMENT,
    INSERT_STATEMENT,
    UPDATE_STATEMENT,
    DELETE_STATEMENT,
    NOT_RECOGNIZED,
    SQL_SYNTAX_ERROR
} statement_type;

typedef enum
{
    SYNTAX_OKAY,
    SYNTAX_ERROR,
} statement_result;

typedef enum
{
    EXECUTE_SUCCESS,
    EXECUTE_FAILURE
} execution_result;