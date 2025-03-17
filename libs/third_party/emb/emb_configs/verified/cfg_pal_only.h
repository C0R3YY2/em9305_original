/*! \brief REF_ONLY Enable inline encryption mode. */
#define BB_ENABLE_INLINE_ENC_TX         1

/*! \brief REF_ONLY Enable inline decryption mode. */
#define BB_ENABLE_INLINE_DEC_RX         1

/*! \brief REF_ONLY Utilize the UART Transport for the Stack. NOTE: The CHCI "UART Transport" used for this stack is connected to the EM Proprietary Transport Layer, which can be selected as SPI or UART elsewhere. This value is not changed if utilizing SPI for the EM Transport Layer */
#define CHCI_TR_UART                    1
