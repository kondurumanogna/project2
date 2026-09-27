#ifndef DECODE_H
#define DECODE_H

#include "encode.h"
#include "types.h" // Contains user defined types



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