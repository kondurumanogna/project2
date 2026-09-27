#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "decode.h" 
#include "types.h"
#include "common.h"
#include "encode.h"


Status validate_decode_args(char *argv[], EncodeInfo *encInfo)
{
    int len=strlen(argv[2]);
    if(len<4)
    {
        printf("Entered INVALID EXTENSION FILE\n");
        return e_failure;
    }
    if(argv[2][len-4]!='.')
    {
        printf("Entered INVALID EXTENSION FILE!!\nCheck file extension character!!Look for '.' at len-4\n");
        return e_failure;
    }
    if(argv[2][len-3]!='b')
    {
        printf("Entered INVALID EXTENSION FILE!!\nCheck file extension!! Write bmp extension\n");        
        return e_failure;
    }
    if(argv[2][len-2]!='m')
    {
        printf("Entered INVALID EXTENSION FILE!!\nCheck file extension!! Write one m in extension\n");            
        return e_failure;
    }
    if(argv[2][len-1]!='p')
    {
        printf("Entered INVALID EXTENSION FILE!!\nCheck file extension!! Write one p in extension\n");
        return e_failure;
    }

    encInfo->stego_image_fname=argv[2];

   if(argv[3] == NULL)
    {
        encInfo->secret_fname = malloc(100);
        if(encInfo->secret_fname==NULL)
        {
            printf("Error : Memory allocation failed\n");
            return e_failure;
        }
         strcpy(encInfo->secret_fname, "decoded");
    }
    else
    {
        encInfo->secret_fname = malloc(100);
        if(encInfo->secret_fname==NULL)
        {
            printf("Error : Memory allocation failed\n");
            return e_failure;
        }
        strcpy(encInfo->secret_fname, argv[3]);
    }
    return e_success;

}
Status do_decoding(EncodeInfo *encInfo)
{
    /*
        ->call open_files(encInfo)==e_failure
            print error msg,return e_failure

        -> call decode_magic_string(MAGIC_STRING,encInfo)==e_failure
            print error msg,return e_failure
            ->seek set should go to 54th byte of the stego image file
        
        ->compare the decoded magic string with MAGIC_STRING
            ->if not same,stop execution

        
        ->call decode_secret_file_extn_size(encInfo)==e_failure
            print error msg,return e_failure
        
        ->call decode_secret_file_extn(secret_file_extn,encInfo)==e_failure
            print error msg,return e_failure

        ->call decode_secret_file_size(secret_file_size,encInfo)==e_failure
            print error msg,return e_failure    


        ->call decode_secret_file_data(&encInfo)==e_failure of secret_file_size
            print error msg,return e_failure    

        ->store the decoded char to o/p file
        
        ->to open o/p file use filename from cla use extn from encoding
        
        return e_success

        */
    char magic_string_decode[3];
    if(open_files_decoding(encInfo)==e_failure)
    {
        printf("Error while opening files\n");
        return e_failure;
    }
    fseek(encInfo->fptr_stego_image, 54, SEEK_SET);
    if(decode_magic_string(magic_string_decode,encInfo)==e_failure)
    {
        printf("Error while decoding magic string\n");
        return e_failure;
    }
    if(strcmp(MAGIC_STRING,magic_string_decode)!=0)
    {
        printf("Magic String is not same\n");
        return e_failure;
    }
    if(decode_secret_file_extn_size(encInfo)==e_failure)
    {
        printf("Error while decoding secret file extension size\n");
        return e_failure;
    }
    if(decode_secret_file_extn(encInfo)==e_failure)
    {
        printf("Error while decoding secret file extension\n");
        return e_failure;
    }

    int len=strlen(encInfo->secret_fname);
    char *dot=strrchr(encInfo->secret_fname,'.');
    if(dot!=NULL)
    {
        len=dot-encInfo->secret_fname;
    }
    if(len+encInfo->extn_secret_file_size>=100)
    {
        printf("Error : Output filename is too long\n");
        return e_failure;
    }
    for(int i=0;i<encInfo->extn_secret_file_size;i++)
    {
        encInfo->secret_fname[len+i]=encInfo->extn_secret_file[i];
    }
    encInfo->secret_fname[len+encInfo->extn_secret_file_size]='\0';
    
    encInfo->fptr_secret=fopen(encInfo->secret_fname,"wb");
    if(encInfo->fptr_secret==NULL)
    {
        printf("Error while creating secret file\n");
        return e_failure;
    }

    if(decode_secret_file_size(encInfo)==e_failure)
    {      
        printf("Error while decoding secret file size\n");
        return e_failure;
    }
    if(decode_secret_file_data(encInfo)==e_failure)
    {
        printf("Error while decoding secret file data\n");
        return e_failure;
    }
    return e_success;
}
Status open_files_decoding(EncodeInfo *encInfo)
{
    /*
        -> open stego_image_fname in read mode
            ->if not able to open,print error msg,return e_failure

        -> open secret_fname in write mode
            ->if not able to open,print error msg,return e_failure

        return e_success;
    */
    encInfo->fptr_stego_image=fopen(encInfo->stego_image_fname,"rb");

    if(encInfo->fptr_stego_image==NULL)
    {
        printf("Error while opening stego image file\n");
        return e_failure;
    }
    
    /*
    encInfo->fptr_secret =fopen(encInfo->secret_fname, "wb");

    if(encInfo->fptr_secret == NULL)
    {
        printf("Error while opening secret file\n");
        return e_failure;
    }
    */
     return e_success;
}


Status decode_magic_string(char *magic_string_decode, EncodeInfo *encInfo)
{
    /*
    ->declare a buffer of 8 bytes
    ->loop for length of magic string
        ->read 8 bytes from the stego image file into buffer
        ->call decode_byte_from_lsb(buffer,magic_string_decode[0])
            ->if not e_success,print error msg,return e_failure

        return e_success
    */

    char *buffer=malloc(8*sizeof(char));
    if(buffer==NULL)
    {
        printf("Error : Memory allocation failed\n");
        return e_failure;
    }
    fseek(encInfo->fptr_stego_image, 54, SEEK_SET);
    for(int i=0;i<strlen(MAGIC_STRING);i++)
    {
        
        if(fread(buffer,1,8,encInfo->fptr_stego_image)!=8)
        {
            printf("Error while reading magic_string from stego image\n");
            free(buffer);
            return e_failure;
        }
        if(decode_byte_from_lsb(buffer,&magic_string_decode[i])==e_failure)
        {
            printf("Error while decoding magic string\n");
            free(buffer);
            return e_failure;
        }
    }
    magic_string_decode[strlen(MAGIC_STRING)] = '\0';

    free(buffer);
    return e_success;
}
Status decode_byte_from_lsb(char *image_buffer, char *data)
{
    /*
        ->loop for 8 times
            ->get the LSB of image_buffer[7-i]
            ->set the i th bit of data with the LSB of image_buffer[7-i]

        return e_success
    */
   *data = 0;
    for(int i=7;i>=0;i--)
    {
        if(image_buffer[7-i] & 1)
        {
            *data=*data|(1 << i);
        }
    }
   return e_success;
}
Status decode_secret_file_extn_size(EncodeInfo *encInfo)
{
    /*
        ->declare a buffer of 32 bytes
        ->read 32 bytes from the stego image file into buffer
        ->call decode_size_from_lsb(buffer,secret_file_extn_size)
            ->if not e_success,print error msg,return e_failure

        return e_success
    */
    char *buffer=malloc(32*sizeof(char));
    if(buffer==NULL)
    {
        printf("Error : Memory allocation failed\n");
        return e_failure;
    }
    if(fread(buffer,1,32,encInfo->fptr_stego_image)!=32)
    {
        printf("Error while reading secret file extension size\n");
        free(buffer);
        return e_failure;
    }
    if(decode_size_from_lsb(&encInfo->extn_secret_file_size,buffer)==e_failure)
    {
        printf("Error while decoding secret file extension size\n");
        free(buffer);
        return e_failure;
    }
    if(encInfo->extn_secret_file_size<0 ||encInfo->extn_secret_file_size>MAX_FILE_SUFFIX)
    {
        printf("Error : Invalid secret file extension size\n");
        free(buffer);
        return e_failure;
    }
    free(buffer);
    return e_success;
}
Status decode_secret_file_extn(EncodeInfo *encInfo)
{
    /*
        ->declare a buffer of 8 bytes
        ->loop for secret file extension length
            ->read 8 bytes from the stego image file into buffer 
            ->call decode_byte_from_lsb(buffer,file_extn[i])
                ->if not e_success,print error msg,return e_failure
                */
    char *buffer=malloc(8*sizeof(char));
    if(buffer==NULL)
    {
        printf("Error : Memory allocation failed\n");
        return e_failure;
    }
    for(int i=0;i<encInfo->extn_secret_file_size;i++)
    {
        if(fread(buffer,1,8,encInfo->fptr_stego_image)!=8)
        {
            printf("Error while reading secret file extension\n");
            free(buffer);
            return e_failure;
        }
        if(decode_byte_from_lsb(buffer,&encInfo->extn_secret_file[i])==e_failure)
        {
            printf("Error while decoding secret file extension\n");
            free(buffer);
            return e_failure;
        }
    }
    free(buffer);
    return e_success;
}
Status decode_secret_file_size( EncodeInfo *encInfo)
{   
    /*
        ->declare a buffer of 32 bytes
        ->read 32 bytes from the stego image file into buffer 
        ->call decode_size_from_lsb(buffer,secret_file_size)
            ->if not e_success,print error msg,return e_failure

        return e_success
    */
    char *buffer=malloc(32*sizeof(char));
    if(buffer==NULL)
    {
        printf("Error : Memory allocation failed\n");
        return e_failure;
    }
    if(fread(buffer,1,32,encInfo->fptr_stego_image)!=32)
    {
        printf("Error while reading secret file size\n");
        free(buffer);
        return e_failure;
    }
    if(decode_size_from_lsb(&encInfo->size_secret_file,buffer)==e_failure)
    {
        printf("Error while decoding secret file size\n");
        free(buffer);
        return e_failure;
    }
    free(buffer);
    return e_success;
}
Status decode_secret_file_data(EncodeInfo *encInfo)
{
    /*
        ->declare a buffer of 8 bytes
        ->loop for secret file size 
            ->read 8 bytes from the stego image file into buffer 
            ->call decode_byte_from_lsb(buffer,secret_file_data[i])
                ->if not e_success,print error msg,return e_failure
        ->write the decoded data to the secret file
        return e_success
    */
    char *buffer=malloc(8*sizeof(char));
    if(buffer==NULL)
    {
        printf("Error : Memory allocation failed\n");
        return e_failure;
    }
    char *data=malloc(encInfo->size_secret_file*sizeof(char));
    if(data==NULL)
    {
        printf("Error : Memory allocation failed\n");
        free(buffer);
        return e_failure;
    }
    for(int i=0;i<encInfo->size_secret_file;i++)
    {
        if(fread(buffer,1,8,encInfo->fptr_stego_image)!=8)
        {
            printf("Error while reading secret file data\n");
            free(buffer);
            free(data);
            return e_failure;
        }
        if(decode_byte_from_lsb(buffer,&data[i])==e_failure)
        {
            printf("Error while decoding secret file data\n");
            free(buffer);
            free(data);
            return e_failure;
        }
    }
    if(fwrite(data,1,encInfo->size_secret_file,encInfo->fptr_secret)!=encInfo->size_secret_file)
    {
        printf("Error while writing decoded secret file\n");
        free(buffer);
        free(data);
        return e_failure;
    }
    free(buffer);
    free(data);
    return e_success;
}   
Status decode_size_from_lsb(int *size,char *Image_buff)
{
    /*
        ->loop for 32 times
            ->get the LSB of Image_buff[31-i]
            ->set the i th bit of size with the LSB of Image_buff[31-i]

        return e_success
    */
   *size = 0;
    for(int i=31;i>=0;i--)
    {
        if(Image_buff[31-i] & 1)
        {
            *size=*size|(1<<i);
        }
        else
        {
            *size=*size& ~(1<<i);
        }
    }


    return e_success;
}
