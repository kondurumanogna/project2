#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    printf("IMAGE CAPACITY: %d\n", width * height * 3);
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
/*
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
*/

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    /* Source file
        -> check last 4 charcters of argv[2] has ".bmp" 
            ->if not,print error msg & return status e_failure 
        encInfo->char *src_image_fname=argv[2]
        
        encInfo->char *secret_fname=argv[3]

        -> check_argv[4]==NULL
           encInfo->char *stego_image_fname="output.bmp"
           ->else
                 -> check last 4 charcters of argv[4] has ".bmp" 
                        ->if not,print error msg & return status e_failure
                 -> encInfo->char *stego_image_fname=argv[4];

        ->  call open_files(encInfo)==e_failure
            -> return e_failure

        return e_success
    */


    //validating source file extension!! 
    int len=strlen(argv[2]);
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

    //source file name
    encInfo->src_image_fname=argv[2];
    //secret file name
    encInfo->secret_fname=argv[3];

    //validating output file
    if(argv[4]==NULL)
    {
        //default output name if it is not given!!
        encInfo->stego_image_fname="output.bmp";

    }
    else
    {
        int len=strlen(argv[4]);
        if(argv[4][len-4]!='.')
        {
            printf("Entered INVALID EXTENSION FILE!!\nCheck file extension character!!Look for '.'\n");
            return e_failure;
        }
        if(argv[4][len-3]!='b')
        {
                printf("Entered INVALID EXTENSION FILE!!\nCheck file extension!! Write bmp extension\n");        
                return e_failure;
        }
        if(argv[4][len-2]!='m')
        {
            printf("Entered INVALID EXTENSION FILE!!\nCheck file extension!! Write one m in extension\n");            
            return e_failure;
        }
        if(argv[4][len-1]!='p')
        {
            printf("Entered INVALID EXTENSION FILE!!\nCheck file extension!! Write one p in extension\n");
            return e_failure;
        }

        //output file name
        encInfo->stego_image_fname=argv[4];
        
    }

    if(open_files(encInfo)==e_failure)
    {
        printf("Error while opening files\n");
        return e_failure;
    }

    return e_success;
    
}
 Status open_files(EncodeInfo *encInfo)
{
    /*
        ->open 'encInfo->src_image_fname' file in read mode fread
            -> if return value is NULL address,print error msg & return e_failure
            fptr_src_image=fopen()

        ->open 'encInfo->secret_file' file in read mode fread
            -> if return value is NULL address,print error msg & return e_failure
            fptr_secret=fopen()

        ->open 'encInfo->stego_image_fname' file in write mode 
            fptr_stego_image=fopen()

        -> return e_success;
    
    */

    // Open source image
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");

    if (encInfo->fptr_src_image == NULL)
    {
        printf("Error: Unable to open source image\n");
        return e_failure;
    }

    // Open secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");

    if (encInfo->fptr_secret == NULL)
    {
        printf("Error: Unable to open secret file\n");
        return e_failure;
    }

    // Open stego image
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");

    if (encInfo->fptr_stego_image == NULL)
    {
        printf("Error: Unable to create stego image\n");
        return e_failure;
    }

    return e_success;
}
Status do_encoding(EncodeInfo *encInfo)
{
    /*

        -> call check_capacity(encInfo)==e_failure
            print error msg,return e_failure

        -> call copy_bmp_header(fptr_src_image,fptr_dest_image)== e_failure
            print error msg,return e_failure

        -> call encode_magic_string(MAGIC_STRING,encInfo)==e_failure
            print error msg,return e_failure

        -> encode_secret_file_extn_size(encInfo)==e_failure
            print error msg,return e_failure
        
        ->encode_secret_file_extn(extn_secret_file,encInfo)==e_failure
            print error msg,return e_failure
        
        -> call encode_secret_file_size(size_secret_file,encInfo)==e_failure
            print error msg,return e_failure;

        -> call encode_secret_file_data(&encInfo)==e_failure
             print error msg,return e_failure
        
        ->call copy_remaining_img_data(fptr_src_file,fptr_dest_file)==e_failure
            print error msg,return e_failure

        return e_success
    */
   if(check_capacity(encInfo)==e_failure)
   {
        printf("Recheck Size of the file!!\n");
        return e_failure;
   }
   if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_failure)
   {
        printf("Error While copying!!\n");
        return e_failure;
   }
   if(encode_magic_string(MAGIC_STRING,encInfo)==e_failure)
   {
        printf("Error while encoding signature\n");
        return e_failure;
   }
   if(encode_secret_file_extn_size(encInfo)==e_failure)
   {
        return e_failure;
   }
   if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_failure)
   {
        return e_failure;
   }
   if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_failure)
   {
        return e_failure;
   }
   if( encode_secret_file_data(encInfo)==e_failure)
   {
        return e_failure;
   }
   if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_failure)
   {
        return e_failure;
   }
   return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    /*

        ->call get_image_size_for_bmp(encode->fptr_src_image)
              image_capcity=get_image_size()

        ->call get_file_size(encode->fptr_secret)
                size_secret_file=get_file_size()
        
        -> check ((14+size_secret_file)*8)>image_capacity
            ->return e_failure
        ->return e_successs

    */
    uint image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
    uint size_secret_file=get_file_size(encInfo->fptr_secret);

    if(((14+size_secret_file)*8)>image_capacity)
    {
        return e_failure;
    }

    return e_success;
}
uint get_file_size(FILE *fptr)
{
    /*
        
       ->move the offset to  last position
       ->return ftell()
    */
    fseek(fptr,0, SEEK_END);
    return ftell(fptr);
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    /*
        ->move the file pointers to SEEK_SET
        ->declare the buff[54]
        ->Read 54 bytes from src file
        ->write 54 bytes to dest file
        ->validate the contents


        ->return e_success
    */

    fseek(fptr_src_image,0,SEEK_SET);
    fseek(fptr_dest_image,0,SEEK_SET);

    char *buffer=malloc(54*sizeof(char));

    fread(buffer,1,54,fptr_src_image);
    fwrite(buffer,1,54,fptr_dest_image);



    free(buffer);
    return e_success;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    /*
        declare a buff of 8 bytes
        ->loop for (length of magic_string) 2 times
        read 8 bytes from the src file into buff
        encode_byte_to_lsb(magic_string[1],buff)
        write the encoded_buff to output_file


        return e_successs
    */
     
    char *buffer=malloc(8*sizeof(char));
    for(int i=0;i<strlen(MAGIC_STRING);i++)
    {
        //read 8 bytes
        fread(buffer,1,8,encInfo->fptr_src_image);
    
        encode_byte_to_lsb(MAGIC_STRING[i],buffer);
        fwrite(buffer,1,8,encInfo->fptr_stego_image);
    }
    free(buffer);
    return e_success;
}
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    /*
    for(int i=7;i>=0;i--)
    {
        ->get the i th bit is or not
            ->if set,set the LSB of image_buffer[7-i]
            -> if clear ,clear the LSB of image_buffer[7-i]
    }
    
    */
   for(int i=7;i>=0;i--)
   {
        if(data&(1<<i))
        {
            image_buffer[7-i]=image_buffer[7-i] | 1;
        }
        else
        {
           image_buffer[7-i]=image_buffer[7-i] & ~(1<<0) ;
        }
   }
   return e_success;
}

Status encode_secret_file_extn_size(EncodeInfo *encInfo)
{
    /*
        ->char *dot=strchr(secrect_file_name,'.')
        ->strcpy(extn_secret_file,dot)

        ->declare a buff[32]

        ->read 32 bytes from src_file into buff

        -> call encode_size_to_lsb(strlen(extn_secret_file),buff)
        ->write 32 bytes of bufffer to output_size
    
    */
    char *dot=strchr(encInfo->secret_fname,'.');
    strcpy(encInfo->extn_secret_file,dot);

    char *buffer=malloc(32*sizeof(char));
    
    fread(buffer,1,32,encInfo->fptr_src_image);
    if(encode_size_to_lsb(strlen(encInfo->extn_secret_file),buffer)==e_failure)
    {
        free(buffer);
        return e_failure;
    }
    fwrite(buffer,1,32,encInfo->fptr_stego_image);

    free(buffer);
    return e_success;
}
Status encode_size_to_lsb(int size,char *Image_buff)
{
    /*
        for(int i=31;i>=0;i--)
        {
         ->get the i th bit is or not
            ->if set,set the LSB of image_buffer[7-i]
            -> if clear ,clear the LSB of image_buffer[7-i]
        }

        return e_success;

    */
   for(int i=31;i>=0;i--)
   {
        if(size & (1<<i))
        {
            Image_buff[31-i]=Image_buff[31-i] | 1;
        }
        else
        {
            Image_buff[31-i]=Image_buff[31-i] & ~(1<<0);
        }
   }

   return e_success;


}
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    /*
        -> declare buff[8]
    ->loop for extension length
        ->read 8 bytes from src_image
        
        ->encode_byte_to_lsb(file_extn[],buff)
        -> write 8 bytes of buff to output_file
    
        return e_success
    */
    char *buffer=malloc(8*sizeof(char));
    for(int i=0;i<strlen(file_extn);i++)
    {
        fread(buffer,1,8,encInfo->fptr_src_image);   
        encode_byte_to_lsb(file_extn[i],buffer) ;
        fwrite(buffer,1,8,encInfo->fptr_stego_image); 
    }

    free(buffer);
    return e_success;

}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    /*
        ->Declare the buff[32]

        ->read 32 bytes from src_image

        -> call encode_size_to_lsb(file_size,buff)
        ->write 32 bytes of buff to output_file

        return e_success;
    
    */

    char *buffer=malloc(32*sizeof(char));
    fread(buffer,1,32,encInfo->fptr_src_image);  
    encode_size_to_lsb(file_size,buffer);
    fwrite(buffer,1,32,encInfo->fptr_stego_image);

    free(buffer);
    return e_success;
    


}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    /*
        ->declare buff[8],data
    ->loop till eof of secret_file
        ->read 8 bytes form src image
        ->read 1 byte from secret_file
        ->encode_byte_to_lsb(data,buff)
    */
   char *buffer=malloc(8*sizeof(char));
   char data;

   fseek(encInfo->fptr_secret,0,SEEK_SET);
   while(fread(&data,1,1,encInfo->fptr_secret)>0)
   {
        fread(buffer,1,8,encInfo->fptr_src_image);
        encode_byte_to_lsb(data,buffer);
        fwrite(buffer,1,8,encInfo->fptr_stego_image);
   }
   free(buffer);
   return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    /*
        ->declare char as data

    ->loop till eof of src_file
        ->read a char from src_file
        ->write data to dest_file
    
    return e_success
    */

    char data;

    while(fread(&data,1,1,fptr_src)>0)
    {
        fwrite(&data,1,1,fptr_dest);
    }
    return e_success;
}