/*
NAME       : NIKHIL R DHOKE
DATE       : 08/02/2023
DESCRIPTION: DECODING SECRET TEXT FROM STEGNOGRAPHY IMAGE
 */
#include<stdio.h>
#include<string.h>
#include "r_decode.h"
#include "r_types.h"
#include "r_common.h"
/****************************************************/

/*          FUNCTION DEFINITION FOR DECODING.      */

/****************************************************/

/* Fn-Def forRead and validate command line arguments*/
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(argv[2])
    {
        if(strcmp(strstr(argv[2],"."),".bmp") == 0)                      // CHECK! ARGUMENT 2 IS ".BMP" FILE OR NOT.
            decInfo -> stego_src_name = argv[2];
        else
           return e_failure;                                             // RETURN FAILURE 
    }
    if(argv[3])                                                         
	   decInfo -> output_fname = argv[3];
    else
	   decInfo -> output_fname = NULL;
    return e_success;
}
Status do_decoding(DecodeInfo *decInfo)                                  // DO DECODING
{
    printf("INFO: ## Decoding Procedure Started ##\n");                  // PRINT USER INFO MESSAGE
    if(open_files_decode(decInfo) == e_success); 
      else
	 return e_failure;                                                   // ELSE RETURN FAILURE
    if(decode_magic_string(decInfo) == e_success)
    {
	  printf("INFO: Done\n");
	  printf("INFO: MAGIC_STRING is matching\n");
    }
    else
    {
	  printf("ERROR: MAGIC_STRING is not matching\n");
	  return e_failure;
    }
    if(decode_secret_file_extn_size(decInfo) == e_success);
    else
	  return e_failure;
    if(decode_secret_file_extn(decInfo -> extn_size, decInfo -> fptr_stego_image, decInfo) == e_success)
    {
	   int flag = 0;
        if(decInfo -> output_fname == NULL)
            flag = 1;

            if(strcmp(decInfo -> extn_output_file, ".txt") == 0)
            {
                decInfo -> fptr_output = fopen("output.txt","w");
                decInfo -> output_fname = "output.txt";
            }
            else if(strcmp(decInfo -> extn_output_file, ".c") == 0)
            {
                decInfo -> fptr_output = fopen("output.c","w");
                decInfo -> output_fname = "output.c";
            }
	         printf("INFO: Done\n");
                if(flag)
                    printf("INFO: Output file is not mentioned. Creating  %s as default\n",decInfo -> output_fname);
                else 
                    printf("INFO: secret output file %s created\n",decInfo -> output_fname);
                decInfo -> fptr_output = fopen(decInfo -> output_fname, "w");

    }
    if(decode_secret_file_size(decInfo -> fptr_stego_image, decInfo) == e_success)
	    printf("INFO: Done\n");
    if(decode_secret_file_data(decInfo -> fptr_output, decInfo -> fptr_stego_image, decInfo) == e_success);
        printf("INFO: Done\n");
    return e_success;
}                                        
Status open_files_decode(DecodeInfo *decInfo)                                // OPENING REQUIRED FILES
{
    printf("INFO: Opening required files\n");

    decInfo -> fptr_stego_image = fopen(decInfo -> stego_src_name, "r");     // STEGO IMAGE FILE

    if(decInfo -> fptr_stego_image == NULL)                                  // DO ERROR HANDLING
    {
	  perror("fopen");
	  fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo -> stego_src_name);
	  return e_failure;
    }
    printf("INFO: Opened %s\n", decInfo -> stego_src_name);
    return e_success;
}                                                                             // DECODING MAGIC STRING
Status decode_magic_string(DecodeInfo *decInfo)                              
{
    char magic[3];
    magic[2] = '\0';
    char image_buffer[8];
    printf("INFO: Decoding Magic String Signature\n");
    fseek(decInfo -> fptr_stego_image, 54, SEEK_SET);
    for(int i = 0; i < 2; i++)
    {
	fread(image_buffer, 8, 1, decInfo -> fptr_stego_image);
	decode_byte_from_lsb(&magic[i] ,image_buffer);
    }
    if(strcmp(magic, "#*") == 0)
	  return e_success;
    else
	  return e_failure;
}
Status decode_data_from_image(int size, FILE* fptr_stego_image, FILE* fptr_output)     
{
    char ch;
    char image_buffer[8];
    for(int i = 0; i < size; i++)
    {
	fread(image_buffer, 8, 1, fptr_stego_image);
	decode_byte_from_lsb(&ch ,image_buffer);
	fwrite(&ch, 1, 1, fptr_output);
    }
    return e_success;
}
Status decode_byte_from_lsb(char* ch , char *image_data)                              // DECODING BYTE FROM LSB
{
    *ch = 0;
    for(int i = 0; i < 8; i++)
     *ch = *ch | ((image_data[i] & 1) << (7 - i));
}
Status decode_secret_file_extn_size(DecodeInfo *decInfo)                             // DECODING SECRET FILE EXTENSION SIZE
{
    int ext_size;
    decode_int_from_lsb(&ext_size, decInfo -> fptr_stego_image, decInfo);
    decInfo -> extn_size = ext_size;
    return e_success;
}
Status decode_int_from_lsb(int* size, FILE *fptr_stego_image, DecodeInfo *decInfo)     // DECODING INT FROM LSB
{
    *size = 0;
    char buffer[32];
    fread(buffer, 32, 1, fptr_stego_image);
    for(int i = 0; i < 32; i++)
    {
	*size = *size | (((int)buffer[i] & 1) << (31 - i));
    }
}
Status decode_secret_file_extn(int size, FILE *fptr_stego_image, DecodeInfo *decInfo) // DECODING SECRET FILE EXTENSION
{
    printf("INFO: Decoding output file extension\n");
    int i;
    char ch;
    char buffer[8];
    for(i = 0; i < size; i++)
    {
	fread(buffer, 8, 1, fptr_stego_image);
	decode_byte_from_lsb(&ch ,buffer);
	decInfo -> extn_output_file[i] = ch;
    }
    decInfo -> extn_output_file[i] = '\0';
    return e_success;
}
Status decode_secret_file_size(FILE *fptr_stego_image, DecodeInfo *decInfo)        // DECODING SECRET FILE SIZE
{
    printf("INFO: Decoding secret file size\n");
    int file_size;
    decode_int_from_lsb(&file_size, decInfo -> fptr_stego_image, decInfo);
    decInfo -> secret_file_size = file_size;
    return e_success;
}
Status decode_secret_file_data(FILE *fptr_output, FILE *fptr_stego_image, DecodeInfo *decInfo) // DECODING SECRET FILE DATA
{
    printf("INFO: Decoding secret file data\n");
    decode_data_from_image(decInfo -> secret_file_size, decInfo -> fptr_stego_image, decInfo -> fptr_output);
    return e_success;
}
                      // PROGRAM ENDS HERE
