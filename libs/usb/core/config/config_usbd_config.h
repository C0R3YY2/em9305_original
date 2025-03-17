/****************************************************************************
*
*            Copyright (c) 2008-2016 by HCC Embedded
*
* This software is copyrighted by and is the sole property of
* HCC.  All rights, title, ownership, or other interests
* in the software remain the property of HCC.  This
* software may only be used in accordance with the corresponding
* license agreement.  Any unauthorized use, duplication, transmission,
* distribution, or disclosure of this software is expressly forbidden.
*
* This Copyright notice may not be removed or modified without prior
* written consent of HCC.
*
* HCC reserves the right to modify this software without notice.
*
* HCC Embedded
* Budapest 1133
* Vaci ut 76.
* Hungary
*
* Tel:  +36 (1) 450 1302
* Fax:  +36 (1) 450 1303
* http: www.hcc-embedded.com
* email: info@hcc-embedded.com
*
***************************************************************************/

#ifndef _CONFIG_USBD_CONFIG_H_
#define _CONFIG_USBD_CONFIG_H_


/* The USB device class (USBD_DEVICE_CLASS) shall be defined by the CMake
   build system to build the USB library. */
#define  USBD_DEVICE_CLASS_CDCACM      1  /* CDC ACM device configuration */
#define  USBD_DEVICE_CLASS_KEYBOARD    2  /* Keyboard device configuration */
#define  USBD_DEVICE_CLASS_MOUSE       3  /* Mouse device configuration */
#define  USBD_DEVICE_CLASS_HIDGENERIC  4  /* HID generic device configuration */
#define  USBD_DEVICE_CLASS_CDCACM_HID  5  /* CDC ACM and HID keyboard & mouse composite device configuration */


/* HCC Embedded generated source */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/// USB-IF assigned  Vendor ID: 'EM MICROELECTRONIC - Marin SA', on Jan 15, 2021
//     Decimal: 13237
//     Hex:     0x33B5
#define EM_VENDOR_ID_LO           0xB5
#define EM_VENDOR_ID_HI           0x33

/// EM defined Product ID
//     Decimal: 1
//     Hex:     0x0001
#define EM_PRODUCT_ID_LO          0x01
#define EM_PRODUCT_ID_HI          0x00


/// Default maximum power [2mA units] -> 0x0A * 2mA/unit = 20mA
#define DEFAULT_MAX_POWER_UNITS   0x0A    //todo Estimated, need HW team to verify.


/* Type definitions */
typedef struct
{
  const uint8_t                        * device_descriptor;
  const uint8_t                        * string_descriptor;
  int                                    number_of_languages;
  int                                    number_of_strings;
  const uint8_t * const * const * const  strings;
  int                                    number_of_configurations;
  const uint8_t * const * const          configurations_fsls;
  const uint8_t * const * const          configurations_hs;
  const uint8_t * const                  dev_qualify_fsls;
  const uint8_t * const                  dev_qualify_hs;
} usbd_config_t;


/* Global definitions */
extern const uint8_t                  usbd_device_descriptor[];
extern const uint8_t                  usbd_fsls_configuration_descriptor_1[];
extern const uint8_t * const          usbd_fsls_configuration_descriptors[];
extern const uint8_t                  usbd_string_descriptor[];
extern const uint8_t                  usbd_string_manufacturer_en[];
extern const uint8_t                  usbd_string_product_en[];
extern const uint8_t                  usbd_string_serial_number_en[];
extern const uint8_t * const          usbd_strings_en[];
extern const uint8_t * const * const  usbd_strings[];
extern const usbd_config_t            usbd_configuration;


#ifdef __cplusplus
}
#endif

#endif /* _CONFIG_USBD_CONFIG_H_ */

/****************************** END OF FILE **********************************/
