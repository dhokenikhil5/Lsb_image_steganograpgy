/*
NAME       : NIKHIL R DHOKE
DATE       : 08/02/2023
DESCRIPTION: LSB IMAGE STEGANOGRAPHY(TEST ENCODE_DECODE.)
 */
#include <stdio.h>
#include <string.h>
#include "r_encode.h"
#include "r_decode.h"
#include "r_types.h"

int main(int argc, char *argv[])
{
    DecodeInfo decodeinfo;
    EncodeInfo encodeinfo;

  if(argc > 2 && argc <6)
  {
    if(check_operation_type(argv) == e_encode)
    {
	if(read_and_validate_encode_args(argv, &encodeinfo) == e_success)
	{
	    if(do_encoding(&encodeinfo) == e_success)
	    {
		printf("INFO: _____Encoding Done Successfully____\n");
	    }
	    else
		printf("ERROR: Encoding failure!!\n");
	}
	else
	    printf("ERROR: Arguments should be given in sequence ./a.out -e <.bmp_file> <.text_file> [output file]\n");
    }
    else if(check_operation_type(argv) == e_decode)
    {
	if(read_and_validate_decode_args(argv, &decodeinfo) == e_success)
	{
	    if(do_decoding(&decodeinfo) == e_success)
	    {
		printf("INFO: ## Decoding Done Successfully ##\n");
	    }
	    else
		printf("ERROR: Decoding failure!!\n");
	}
	else
	   printf("ERROR: Arguments should be given in sequence ./a.out -d <.bmp_file> <.text_file>\n");
    }
    else
    	printf("ERROR: Invalid option\n");
 

    return 0;
  }
  else
  printf("Please Pass at least two Argument or less than 5\n");

}

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e") == 0)
	return e_encode;
 else if(strcmp(argv[1],"-d") == 0)
	return e_decode;
    else
	return e_unsupported;
}
