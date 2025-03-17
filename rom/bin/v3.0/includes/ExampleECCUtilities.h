/*----------------------------------------------------------------
FILE : ExampleECCUtilities.h

GOAL : Utilities Functions for the validation
------------------------------------------------------------------*/
#ifndef EXAMPLE_ECC_UTILS_H
#define EXAMPLE_ECC_UTILS_H

extern void ConvertStringToU8(char * src, uint8_t * dst, uint32_t size);
extern void ConvertByteToU32(uint8_t * src, uint32_t* dst,uint32_t size);
extern void ConvertStringToU32(char * src, uint32_t * dst,uint32_t size);

#endif
