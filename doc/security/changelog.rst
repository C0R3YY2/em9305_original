Change Log
------------


Release note V1.0
------------------
* (validation)Add SHA1, SHA224,SHA256, SHA384 and SHA512 NIST tests. Tests in line with test plan.
* (validation)Add ECDSA NIST tests + wrong parameters + invalid signature.Tests in line with test plan.
* (validation)Add ECC\ECDH NIST tests+ wrong parameters +  specif infinity point tests.Tests in line with test plan.
* (lib)Improve RAM management of ECDSA
* (lib)Add a sanity check on public key ECDSA
* (lib)Enable RNG init in ECC for the first time the context is initlaized
* (lib)slighly Improve PRNG Lib Timing (health tests)
* (lib) Compilation in release configuration:
  * Optimisation for fastest libs
  * remove debug info
* (lib) Fix a bug when Health tests are not successful
* (validation) Move test vectors in NVM (NVM_CONST) to avoid exploding the data segment


Release note V1.1
------------------
* (lib) Add AES ECB lib using both the AES with implicitly key or AES with key containers
* (lib) Add a function AES_GenerateAuthenticateChallenge in PRNG lib dedicated to the External authentication with AESSetPRNGCtx
* (lib) Add a function SetPRNGCtx that set a PRNG context without picking the HW RNG
* (lib) Modify the ECCGenerateRandom internal function , to use SetPRNGCtx for ECC functions that need RNG (GeneratePrivateKey, Generate the schnoor challenge)
* (Validation) : Modify Validation_ECC, Validation Schnoor accordingly to set up the PRNG context
* (Validation) : Modify Validation_PRNG accordingly to set up the PRNG context
* (Validation): Add Validation_AuthAES.c to validation the new AES_GenerateAuthenticateChallenge function
* (Validation): Add Validation_AES_ECB_128.c,Validation_AES_ECB_192.c,Validation_AES_ECB_256.c,alidation_AES_ECB_Error.c to test AES_ECB lib


Release note V2.0
------------------
V1.1 are the ROM libs in Di01.
V2.x are the NVM libs (Di01)
* (lib) Add ECC Verify Pub Key lib


Release note V3.0
------------------
V3.0 are ROM libs for di03 (second ROM mask)
* (lib) : Switch to 2019.09 tool set
* (lib) : include ECC Verify PubKey API in ECC lib
* (lib) : select core 2 as default RNG core
* (lib) : Add AES_ECB
* (lib) : Add AES_CBC
* (lib) : Add AES_CTR
* (lib) : Add AES_CMAC
* (lib) : Add AES_GCM

* (validation): Add Validation_AES_ECB_128
* (validation): Add Validation_AES_ECB_192
* (validation): Add Validation_AES_ECB_256
* (validation): Add Validation_AES_ECB_Error
* (validation): Add Validation_AES_CBC_128
* (validation): Add Validation_AES_CBC_192
* (validation): Add Validation_AES_CBC_256
* (validation): Add Validation_AES_CBC_Error
* (validation): Add Validation_AES_CTR_128
* (validation): Add Validation_AES_CTR_192
* (validation): Add Validation_AES_CTR_256
* (validation): Add Validation_AES_CTR_Error
* (validation): Add Validation_AES_CMAC_128
* (validation): Add Validation_AES_CMAC_192
* (validation): Add Validation_AES_CMAC_256
* (validation): Add Validation_AES_CMAC_Error
* (validation): Add Validation_AES_GCM_128
* (validation): Add Validation_AES_GCM_192
* (validation): Add Validation_AES_GCM_256
* (validation): Add Validation_AES_GCM_Error


Release note V3.1
------------------
* (lib) : Add AES_CCM library
* (lib) : Add null pointer test for AES GCM
* (lib) : Add null pointer test for ECDH
* (lib) : PRNG.h Inverse order of headers
* (lib) : Improve doxygen documentation of .h file
* (Validation): Add Validation_AES_CCM_128.c,Validation_AES_CCM_192.c,Validation_AES_CCM_256.c,alidation_AES_CCM_Error.c to test AES_CCM lib
* (Validation): Clean headers


Release note V3.2
-----------------
The main purpose of this release is mainly cleaning to generate clean cryptolib document. It adds examples , refered in the documentation.

PRNG.h was also modified to correct blatant spelling mistake
-(lib) Modify PRNG.h- Error status SW_PRNG_NOT_INITIALZED was corrected into SW_PRNG_NOT_INITIALIZED (missing I before the Z)
NB: this status is used in PRNG lib, ECC lib but not out of the libs. 

* (lib) clean up headers

* (Doc) generared html and PDF is provided. (later might be generated in the project)
