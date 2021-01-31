#include <stdio.h>
#include <yaml.h>
#include <assert.h>
#include <wchar.h>
#include <stdbool.h>

#include <hape/cnt.h>

wchar_t *name_of_event( int t )
{
    switch ( t )
    {
        case YAML_NO_EVENT:
            return L"YAML_NO_EVENT";
            break;
        case YAML_STREAM_START_EVENT:
            return L"YAML_STREAM_START_EVENT";
            break;
        case YAML_STREAM_END_EVENT:
            return L"YAML_STREAM_END_EVENT";
            break;
        case YAML_DOCUMENT_START_EVENT:
            return L"YAML_DOCUMENT_START_EVENT";
            break;
        case YAML_DOCUMENT_END_EVENT:
            return L"YAML_DOCUMENT_END_EVENT";
            break;
        case YAML_ALIAS_EVENT:
            return L"YAML_ALIAS_EVENT";
            break;
        case YAML_SCALAR_EVENT:
            return L"YAML_SCALAR_EVENT";
            break;
        case YAML_SEQUENCE_START_EVENT:
            return L"YAML_SEQUENCE_START_EVENT";
            break;
        case YAML_SEQUENCE_END_EVENT:
            return L"YAML_SEQUENCE_END_EVENT";
            break;
        case YAML_MAPPING_START_EVENT:
            return L"YAML_MAPPING_START_EVENT";
            break;
        case YAML_MAPPING_END_EVENT:
            return L"YAML_MAPPING_END_EVENT";
            break;
        default:
            return L"<none>";
            break;
    }
}

wchar_t *name_of_token( int t )
{
    switch ( t )
    {
        case YAML_NO_TOKEN:
            return L"YAML_NO_TOKEN";
            break;
        case YAML_STREAM_START_TOKEN:
            return L"YAML_STREAM_START_TOKEN";
            break;
        case YAML_STREAM_END_TOKEN:
            return L"YAML_STREAM_END_TOKEN";
            break;
        case YAML_VERSION_DIRECTIVE_TOKEN:
            return L"YAML_VERSION_DIRECTIVE_TOKEN";
            break;
        case YAML_TAG_DIRECTIVE_TOKEN:
            return L"YAML_TAG_DIRECTIVE_TOKEN";
            break;
        case YAML_DOCUMENT_START_TOKEN:
            return L"YAML_DOCUMENT_START_TOKEN";
            break;
        case YAML_DOCUMENT_END_TOKEN:
            return L"YAML_DOCUMENT_END_TOKEN";
            break;
        case YAML_BLOCK_SEQUENCE_START_TOKEN:
            return L"YAML_BLOCK_SEQUENCE_START_TOKEN";
            break;
        case YAML_BLOCK_MAPPING_START_TOKEN:
            return L"YAML_BLOCK_MAPPING_START_TOKEN";
            break;
        case YAML_BLOCK_END_TOKEN:
            return L"YAML_BLOCK_END_TOKEN";
            break;
        case YAML_FLOW_SEQUENCE_START_TOKEN:
            return L"YAML_FLOW_SEQUENCE_START_TOKEN";
            break;
        case YAML_FLOW_SEQUENCE_END_TOKEN:
            return L"YAML_FLOW_SEQUENCE_END_TOKEN";
            break;
        case YAML_FLOW_MAPPING_START_TOKEN:
            return L"YAML_FLOW_MAPPING_START_TOKEN";
            break;
        case YAML_FLOW_MAPPING_END_TOKEN:
            return L"YAML_FLOW_MAPPING_END_TOKEN";
            break;
        case YAML_BLOCK_ENTRY_TOKEN:
            return L"YAML_BLOCK_ENTRY_TOKEN";
            break;
        case YAML_FLOW_ENTRY_TOKEN:
            return L"YAML_FLOW_ENTRY_TOKEN";
            break;
        case YAML_KEY_TOKEN:
            return L"YAML_KEY_TOKEN";
            break;
        case YAML_VALUE_TOKEN:
            return L"YAML_VALUE_TOKEN";
            break;
        case YAML_ALIAS_TOKEN:
            return L"YAML_ALIAS_TOKEN";
            break;
        case YAML_ANCHOR_TOKEN:
            return L"YAML_ANCHOR_TOKEN";
            break;
        case YAML_TAG_TOKEN:
            return L"YAML_TAG_TOKEN";
            break;
        case YAML_SCALAR_TOKEN:
            return L"YAML_SCALAR_TOKEN";
            break;
        default:
            return L"<none>";
            break;
    }
}

void tokens( yaml_parser_t * p )
{
    yaml_token_t token;
    MEM_ARENA arena = mem_arena_new(  );
    CNT cnt = cnt_create_a( arena );
    char key[100];

    unsigned row = 0;
    do
    {
        yaml_parser_scan( p, &token );
        switch ( token.type )
        {
            case YAML_STREAM_START_TOKEN:
                break;
            case YAML_STREAM_END_TOKEN:
                break;
            case YAML_KEY_TOKEN:
                
                yaml_token_delete( &token );
                yaml_parser_scan( p, &token );
                assert(token.type == YAML_SCALAR_TOKEN);
                strcpy(key, token.data.scalar.value );
                yaml_token_delete( &token );
                yaml_parser_scan( p, &token );
                assert(token.type == YAML_VALUE_TOKEN);
                yaml_token_delete( &token );
                yaml_parser_scan( p, &token );
                assert(token.type == YAML_SCALAR_TOKEN);
                cnt_set_idx_b( cnt, key, row, token.data.scalar.value);
                break;
            case YAML_VALUE_TOKEN:
                // wprintf( L"(Value token) " );
                break;
            case YAML_BLOCK_SEQUENCE_START_TOKEN:
                // wprintf( L"Sequence start\n" );
                break;
            case YAML_BLOCK_ENTRY_TOKEN:
                // wprintf( L"Entry start\n" );
                row++;
                break;
            case YAML_BLOCK_END_TOKEN:
                // wprintf( L"End block\n" );
                break;
            case YAML_BLOCK_MAPPING_START_TOKEN:
                // wprintf( L"[Block mapping]\n" );
                break;
            case YAML_SCALAR_TOKEN:
                // wprintf( L"scalar %s \n", token.data.scalar.value );
                break;
            // default:
                // wprintf( L"Got token of type %d\n", token.type );
        }
        if( token.type != YAML_STREAM_END_TOKEN )
            yaml_token_delete( &token );
    }
    while( token.type != YAML_STREAM_END_TOKEN );
    yaml_token_delete( &token );

    cnt_dump( cnt );
}

void scanner( yaml_parser_t * p )
{
    FILE *f = fopen( "test.yaml", "rb" );
    yaml_event_t e;
    assert( NULL != f );
    yaml_parser_set_input_file( p, f );

    MEM_ARENA arena = mem_arena_new(  );
    CNT cnt = cnt_create_a( arena );

    bool running = true;
    while( running )
    {
        assert( 1 == yaml_parser_parse( p, &e ) );
        wprintf( L"Event: %d %ls\n", e.type, name_of_event( e.type ) );

        switch ( e.type )
        {
            case YAML_STREAM_END_EVENT:
                running = false;
                break;
            case YAML_ALIAS_EVENT:
                wprintf( L"Got alias (anchor %s)\n", e.data.alias.anchor );
                break;
            case YAML_SCALAR_EVENT:
                wprintf( L"Got scalar (value %s)\n", e.data.scalar.value );
                break;
        }

        yaml_event_delete( &e );
    }
}

void parse( yaml_parser_t * p )
{
    FILE *f = fopen( "test.yaml", "rb" );
    yaml_event_t e;
    yaml_document_t d;
    assert( NULL != f );
    yaml_parser_set_input_file( p, f );

    yaml_parser_load( p, &d );

    yaml_node_t *n;

    n = yaml_document_get_root_node( &d );
    assert( n );
    switch ( n->type )
    {
        case YAML_SEQUENCE_NODE:
            wprintf( L"seq\n" );
            break;
    }


    yaml_document_delete( &d );
}


int main(  )
{
    yaml_parser_t p;
    assert( 1 == yaml_parser_initialize( &p ) );
    FILE *f = fopen( "test.yaml", "rb" );
    assert( NULL != f );
    yaml_parser_set_input_file( &p, f );
    tokens( &p );
    yaml_parser_delete( &p );
    wprintf( L"READY." );
}
