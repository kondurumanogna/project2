#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc,char *argv[])
{
    EncodeInfo encInfo;
    
    /*
       -> call check_operation_type(argv[1][1])==e_encode
    */
        /*
            -> call read &validate_encode_argv(argv,&encInfo)==e_success
                -> call do_encoding(&encInfo)==e_success
                    print "Encoding is Success"

        */

    /*
        call check_operation_type(argv[1][1])==e_decode
            -> call validate_decode_args(argv,&encInfo)==e_success
                -> call do_decoding(&encInfo)==e_success
                    print "Decoding is Success"
    
    */
    
    if(check_operation_type(argv[1][1])==e_encode)
    {
        if(argc<4)
        {
            printf("Invalid Input\n");
            return e_failure;
        }
        if(read_and_validate_encode_args(argv,&encInfo)==e_success)
        {
            if(do_encoding(&encInfo)==e_success)
            {
                printf("Encoding is success\n");

            }
            else
            {
                printf("Encoding is failure\n");
            }
        }

    }
    else if(check_operation_type(argv[1][1])==e_decode)
    {
        if(argc<3)
        {
            printf("Invalid Input\n");
            return e_failure;
        }
        if(validate_decode_args(argv,&encInfo)==e_success)
        {
            if(do_decoding(&encInfo)==e_success)
            {
                printf("Decoding is success\n");
            }
            else
            {
                printf("Decoding is failure\n");
            }
        }
    }
    else
    {
        printf("Invalid operation type\n");
    }

    return 0;
}
OperationType check_operation_type(char opt)
{
    /*
        check opt is 'e'
        return e_encode
        if it is 'd'
        return e_decode
        else
        return e_unsupported!!
    */   
    if(opt=='e') return e_encode;
    else if(opt=='d') return e_decode;
    else return e_unsupported;

}


