#ifndef DECODE_H
#define DECODE_H

#include "encode.h"
#include "types.h" // Contains user defined types



//OperationType check_operation_type(char opt);

//Status open_files(EncodeInfo *encInfo);

/* Decoding function prototype */

/* Check operation type */
/*
OperationType check_operation_type(char opt);

/* Read and validate Decode args from argv */
//Status validate_decode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the decoding */
//Status do_decoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
//Status open_files_decoding(EncodeInfo *encInfo);



/* Get image size */
//uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
//uint get_file_size(FILE *fptr);



/* Store Magic String from decoded data */
//Status decode_magic_string(char *magic_string_decode, EncodeInfo *encInfo);


//decode  secret file extension size
//Status decode_secret_file_extn_size(EncodeInfo *encInfo);


/* Decode secret file extenstion */
//Status decode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);

/* Decode secret file size */
//Status decode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Decode secret file data*/
//Status decode_secret_file_data(EncodeInfo *encInfo);

// Decode function, which does the real decoding */
//Status decode_size_from_lsb(long *size, char *Image_buff);

/* Decode a byte from LSB of image data array */
//Status decode_byte_from_lsb(char *image_buffer, char *data);


Status validate_decode_args(char *argv[], EncodeInfo *encInfo);

Status do_decoding(EncodeInfo *encInfo);

Status open_files_decoding(EncodeInfo *encInfo);

Status decode_magic_string(char *magic_string_decode,EncodeInfo *encInfo);

Status decode_byte_from_lsb(char *image_buffer,char *data);

Status decode_secret_file_extn_size(EncodeInfo *encInfo);

Status decode_secret_file_extn(EncodeInfo *encInfo);

Status decode_secret_file_size(EncodeInfo *encInfo);

Status decode_secret_file_data(EncodeInfo *encInfo);

Status decode_size_from_lsb(int *size, char *Image_buff);
#endif // DECODE_H