#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

/*
 * Node definitions
 */
struct node_t
{
    int data;
    struct node_t *next;
};

struct node_t *node_create( int data )
{
    struct node_t *node = (struct node_t*)malloc( sizeof( struct node_t ) );

    if( NULL == node )
    {
        printf( "node creation failed\n" );
        return NULL;
    }

    /* Initialize node members */
    node->data = data;
    node->next = NULL;

    return node;
}

/*
* List definitions
*/
struct list_t
{
    struct node_t *head;
    int size;
};

struct list_t *list_create()
{
    struct list_t *l = (struct list_t*)malloc( sizeof( struct list_t ) );

    if( NULL == l )
    {
        printf( "list creation failed\n" );
        return NULL;
    }

    /* Initialize node members */
    l->head = NULL;
    l->size = 0;

    return l;
}

/* Add a new element to the list; list creates memory internally */
int list_append( struct list_t *list, struct node_t *node )
{
    struct node_t *cur;

    if( NULL == list || NULL == node )
    {
        return -1;
    }

    /* List initially empty; set head pointer */
    if( NULL == list->head )
    {
        list->head = node;
        list->size = 1;
        return 0;
    }

    /* If size > 0, traverse list to end, then add */
    cur = list->head;

    while( NULL != cur->next )
    {
        cur = cur->next;
    }

    cur->next = node;
    list->size++;

    return 0;
}

int list_remove( struct list_t *list, int index )
{
    int pos = 0;
    struct node_t *cur;
    struct node_t *rem;

    /* Input validation: null list, 0-sized list, index range */
    if( NULL == list 
        || 0 == list->size
        || index > (list->size-1) )
    {
        return -1;
    }

    cur = list->head;

    if( 1 == list->size )
    {
        free( cur );
        list->head = NULL;
        list->size = 0;
        return 0;
    }

    if( 0 == index )
    {
        list->head = cur->next;
        list->size--;
        free( cur );
        return 0;
    }

    while( pos != index-1 )
    {
        cur = cur->next;
        pos++;
    }

    rem = cur->next;
    cur->next = rem->next;
    free( rem );

    list->size--;

    return 0;
}

void list_print( const struct list_t *list )
{
    printf( "list contents (size %d): ", list->size );

    /* List initially empty; set head pointer */
    if( NULL == list->head )
    {
        printf( "NULL\n" );
        return;
    }

    struct node_t *cur = list->head;

    while( NULL != cur )
    {
        printf( "%d ", cur->data );
        cur = cur->next;
    }
    
    printf( "\n" );
}

void test()
{
    struct list_t *my_list = NULL;
    int i;
    const int size = 10;

    assert( list_append( NULL, NULL ) == -1 );
    
    my_list = list_create();

    assert( list_append( my_list, NULL ) == -1 );
    assert( list_remove( my_list, 0 ) == -1 );

    for( i = 0; i < size; ++i )
    {
        struct node_t *n = node_create( i );
        assert(list_append( my_list, n ) == 0);
        list_print( my_list );
    }

    assert( list_remove( my_list, 10 ) == -1 );

    assert( list_remove( my_list, 0 ) == 0 );
    list_print( my_list );
    assert( list_remove( my_list, 1 ) == 0 );
    list_print( my_list );
    assert( list_remove( my_list, 2 ) == 0 );
    list_print( my_list );
    assert( list_remove( my_list, my_list->size-1 ) == 0 );
    list_print( my_list );
}

char *getinput()
{
    static char line[100];

    fgets( line, sizeof( line ), stdin );

    /* Chomp */
    line[strlen( line ) - 1] = '\0';

    return line;
}

int parseinput( char *in, char *action, int *data )
{
    char *tok[2];

    if( NULL == in || 0 == strlen( in ) )
    {
        return -1;
    }

    tok[0] = strtok( in, " " );
    tok[1] = strtok( NULL, " " );

    /* Validate action token */
    if( NULL == tok[0] )
    {
        return -1;
    }

    *action = tok[0][0];

    /* Validate data token, if present */
    if( NULL != tok[1] )
    {
        int  i;

        for( i = 0; i < strlen( tok[1] ); ++i )
            if( !isdigit( tok[1][i] ) )
                return -1;

        *data = atoi( tok[1] );
    }

    return 0;
}

void displayprompt()
{
    printf( "CMD>> " );
}

void printusage()
{
    printf( "<action> [data]\n" );
    printf( "   a <data>    - add given integer data to the list\n" );
    printf( "   r <index>   - remove data at given list index\n" );
    printf( "   p           - prints current list\n" );
    printf( "   h           - display help\n" );
    printf( "   x           - exit program\n" );
}


int main()
{
#if 0
    test();
#endif

    struct list_t *my_list = list_create();
    struct node_t *new_node;
    while( 1 )
    {
        char action;
        int data;
        displayprompt();
        char *in = getinput();

        if( 0 != parseinput( in, &action, &data ) )
            continue;
        
        switch( action )
        {
        case 'a':
            new_node = node_create( data );
            list_append( my_list, new_node );
            break;

        case 'r':
            list_remove( my_list, data );
            break;

        case 'p':
            list_print( my_list );
            break;

        case 'x':
            return 0;

        case 'h':
        default: printusage();
        }
    }

    return 0;
}

