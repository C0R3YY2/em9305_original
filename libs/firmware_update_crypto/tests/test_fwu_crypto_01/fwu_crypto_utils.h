#include "types.h"

/**
 * @brief Data to write at NVM_INFO_P2_USER_PUBLIC_KEYS_BLOCK_ADDR to test
 *        public keys.
 * @note  This variable is aligned on 32-bit.
 */
extern const uint8_t gFWUCrypto_patchPublicKeyBlock[72u];

/*Utility function for testing*/
void ConvertStringToU8(char * src, uint8_t * dst, uint32_t size);
void ConvertByteToU32(uint8_t * src, uint32_t* dst,uint32_t size);
