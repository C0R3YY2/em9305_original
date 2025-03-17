/***************************************************************************
 *
 *            Copyright (c) 2020-2022 by HCC Embedded
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
 * Vaci ut 76
 * Hungary
 *
 * Tel:  +36 (1) 450 1302
 * Fax:  +36 (1) 450 1303
 * http: www.hcc-embedded.com
 * email: info@hcc-embedded.com
 *
 ***************************************************************************/
#ifndef _USBD_SYNOPSYS_OTG_REGS_H_
#define _USBD_SYNOPSYS_OTG_REGS_H_

#include "../../psp/include/psp_types.h"

#include "../../version/ver_usbd_synopsys_otg.h"
#if ( ( VER_USBD_EMM_SYNOPSYS_SHAREDFIFO_OTG_MAJOR != 1 ) || ( VER_USBD_EMM_SYNOPSYS_SHAREDFIFO_OTG_MINOR != 5 ) )
 #error "Invalid USBD_SYNOPSYS_SHAREDFIFO_OTG version."
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------*/
/*----------------------------------*/
/* ------- OTG_FS ----------------- */
/*----------------------------------*/
/*----------------------------------*/
#define HCC_OTG_GET_VAL( reg_val, pos, mask ) ( ( ( reg_val ) >> ( pos ) ) & ( mask ) )

/*----------------------------------*/
/* - Register Address Definitions - */
/*----------------------------------*/
#define HCC_OTG_GOTGCTL( base ) *(volatile uint32_t *)( ( base ) + 0x0000u )
#define HCC_SRQSCS          ( 1u << 0 )
#define HCC_SRQ             ( 1u << 1 )
#define HCC_BVALOEN         ( 1u << 6 )
#define HCC_BVALOVAL        ( 1u << 7 )
#define HCC_HNGSCS          ( 1u << 8 )
#define HCC_HNPRQ           ( 1u << 9 )
#define HCC_HSHNPEN         ( 1u << 10 )
#define HCC_DHNPEN          ( 1u << 11 )
#define HCC_CIDSTS          ( 1u << 16 )
#define HCC_DBCT            ( 1u << 17 )
#define HCC_ASVLD           ( 1u << 18 )
#define HCC_BSVLD           ( 1u << 19 )

#define HCC_OTG_GOTGINT( base ) *(volatile uint32_t *)( ( base ) + 0x0004u )
#define HCC_SEDET           ( 1u << 2 )
#define HCC_SRSSCHG         ( 1u << 8 )
#define HCC_HNSSCHG         ( 1u << 9 )
#define HCC_HNGDET          ( 1u << 17 )
#define HCC_ADTOCHG         ( 1u << 18 )
#define HCC_DBCDNE          ( 1u << 19 )

#define HCC_OTG_GAHBCFG( base ) *(volatile uint32_t *)( ( base ) + 0x0008u )
#define HCC_GINT            ( 1u << 0 )
#define HCC_TXFELVL         ( 1u << 7 )
#define HCC_PTXFELVL        ( 1u << 8 )

#define HCC_OTG_GUSBCFG( base ) *(volatile uint32_t *)( ( base ) + 0x000Cu )
#define HCC_TOCAL_SET( base, val ) \
  HCC_OTG_GUSBCFG( base ) &= ~( 0x7u ); \
  HCC_OTG_GUSBCFG( base ) |= ( ( val ) & 0x7u );
#define HCC_ULPI_SEL        ( 1u << 4 )
#define HCC_FS3PIN_IF       ( 1u << 5 )
#define HCC_PHYUSB11_SEL    ( 1u << 6 )
#define HCC_SRPCAP          ( 1u << 8 )
#define HCC_HNPCAP          ( 1u << 9 )
#define HCC_TRDT_SET( base, val ) \
  HCC_OTG_GUSBCFG( base ) &= ~( 0xFu << 10 ); \
  HCC_OTG_GUSBCFG( base ) |= ( ( ( val ) & 0xFu ) << 10 );
#define HCC_NPTXRWEN        ( 1u << 14 )
#define HCC_FHMOD           ( 1u << 29 )
#define HCC_FDMOD           ( 1u << 30 )
#define HCC_CTXPKT          ( 1u << 31 )

#define HCC_OTG_GRSTCTL( base ) *(volatile uint32_t *)( ( base ) + 0x0010u )
#define HCC_CSRST           ( 1u << 0 )
#define HCC_HSRST           ( 1u << 1 )
#define HCC_FCRST           ( 1u << 2 )
#define HCC_INTKNQFLSH      ( 1u << 3 )
#define HCC_RXFFLSH         ( 1u << 4 )
#define HCC_TXFFLSH         ( 1u << 5 )
#define HCC_TXFNUM_SET( base, val ) \
  HCC_OTG_GRSTCTL( base ) &= ~( 0x1Fu << 6 ); \
  HCC_OTG_GRSTCTL( base ) |= ( ( ( val ) & 0x1Fu ) << 6 );
#define HCC_TXFNUM_ALL_FIFO 0x10u
#define HCC_AHBIDL          ( 1u << 31 )

#define HCC_OTG_GINTSTS( base ) *(volatile uint32_t *)( ( base ) + 0x0014u )
#define HCC_CMOD            ( 1u << 0 )
#define HCC_MMIS            ( 1u << 1 )
#define HCC_OTGINT          ( 1u << 2 )
#define HCC_SOF             ( 1u << 3 )
#define HCC_RXFLVL          ( 1u << 4 )
#define HCC_NPTXFEMP        ( 1u << 5 )
#define HCC_GINAKEFF        ( 1u << 6 )
#define HCC_GONAKEFF        ( 1u << 7 )
#define HCC_ESUSP           ( 1u << 10 )
#define HCC_USBSUSP         ( 1u << 11 )
#define HCC_USBRST          ( 1u << 12 )
#define HCC_ENUMDNE         ( 1u << 13 )
#define HCC_ISOODRP         ( 1u << 14 )
#define HCC_EOPF            ( 1u << 15 )
#define HCC_IEPINT          ( 1u << 18 )
#define HCC_OEPINT          ( 1u << 19 )
#define HCC_IISOIXFR        ( 1u << 20 )
#define HCC_IPXFR           ( 1u << 21 )
#define HCC_HPRTINT         ( 1u << 24 )
#define HCC_HCINT           ( 1u << 25 )
#define HCC_PTXFE           ( 1u << 26 )
#define HCC_CIDSCHG         ( 1u << 28 )
#define HCC_DISCINT         ( 1u << 29 )
#define HCC_SRQINT          ( 1u << 30 )
#define HCC_WKUINT          ( 1u << 31 )

#define HCC_OTG_GINTMSK( base ) *(volatile uint32_t *)( ( base ) + 0x0018u )
#define HCC_CMODM           ( 1u << 0 )
#define HCC_MMISM           ( 1u << 1 )
#define HCC_OTGINTM         ( 1u << 2 )
#define HCC_SOFM            ( 1u << 3 )
#define HCC_RXFLVLM         ( 1u << 4 )
#define HCC_NPTXFEMPM       ( 1u << 5 )
#define HCC_GINAKEFFM       ( 1u << 6 )
#define HCC_GONAKEFFM       ( 1u << 7 )
#define HCC_ESUSPM          ( 1u << 10 )
#define HCC_USBSUSPM        ( 1u << 11 )
#define HCC_USBRSTM         ( 1u << 12 )
#define HCC_ENUMDNEM        ( 1u << 13 )
#define HCC_ISOODRPM        ( 1u << 14 )
#define HCC_EOPFM           ( 1u << 15 )
#define HCC_IEPINTM         ( 1u << 18 )
#define HCC_OEPINTM         ( 1u << 19 )
#define HCC_IISOIXFRM       ( 1u << 20 )
#define HCC_IPXFRM          ( 1u << 21 )
#define HCC_HPRTINTM        ( 1u << 24 )
#define HCC_HCINTM          ( 1u << 25 )
#define HCC_PTXFEM          ( 1u << 26 )
#define HCC_CIDSCHGM        ( 1u << 28 )
#define HCC_DISCINTM        ( 1u << 29 )
#define HCC_SRQINTM         ( 1u << 30 )
#define HCC_WKUINTM         ( 1u << 31 )

#define HCC_OTG_GINTSTS_MASKED( base ) ( HCC_OTG_GINTSTS( base ) & HCC_OTG_GINTMSK( base ) )

#define HCC_OTG_GRXSTSP( base )        *(volatile uint32_t *)( ( base ) + 0x0020u )
#define HCC_OTG_GRXSTSP_CHNUM_POS   0u
#define HCC_OTG_GRXSTSP_CHNUM_MASK  0xFu
#define HCC_OTG_GRXSTSP_BCNT_POS    4u
#define HCC_OTG_GRXSTSP_BCNT_MASK   0x7FFu
#define HCC_OTG_GRXSTSP_DPID_POS    15u
#define HCC_OTG_GRXSTSP_DPID_MASK   0x3u
#define HCC_OTG_GRXSTSP_PKTSTS_POS  17u
#define HCC_OTG_GRXSTSP_PKTSTS_MASK 0xFu
#define HCC_OTG_GRXSTSP_FRMNUM_POS  21u
#define HCC_OTG_GRXSTSP_FRMNUM_MASK 0xFu
#define HCC_OTG_GRXSTSP_GET_CHNUM( reg_val )  HCC_OTG_GET_VAL( ( reg_val ), HCC_OTG_GRXSTSP_CHNUM_POS, HCC_OTG_GRXSTSP_CHNUM_MASK )
#define HCC_OTG_GRXSTSP_GET_BCNT( reg_val )   HCC_OTG_GET_VAL( ( reg_val ), HCC_OTG_GRXSTSP_BCNT_POS, HCC_OTG_GRXSTSP_BCNT_MASK )
#define HCC_OTG_GRXSTSP_GET_DPID( reg_val )   HCC_OTG_GET_VAL( ( reg_val ), HCC_OTG_GRXSTSP_DPID_POS, HCC_OTG_GRXSTSP_DPID_MASK )
#define HCC_OTG_GRXSTSP_GET_PKSTS( reg_val )  HCC_OTG_GET_VAL( ( reg_val ), HCC_OTG_GRXSTSP_PKTSTS_POS, HCC_OTG_GRXSTSP_PKTSTS_MASK )
#define HCC_OTG_GRXSTSP_GET_FRMNUM( reg_val ) HCC_OTG_GET_VAL( ( reg_val ), HCC_OTG_GRXSTSP_FRMNUM_POS, HCC_OTG_GRXSTSP_FRMNUM_MASK )

/*--- Received packet status codes ---*/
/* 0001: Global OUT NAK (triggers an interrupt) */
#define HCC_OTG_GRXSTSP_PKTSTS_GLOBAL_OUT_NAK    1u

/* 0010: OUT data packet received */
#define HCC_OTG_GRXSTSP_PKTSTS_OUT_DATA_RECEIVED 2u

/* 0011: OUT transfer completed (triggers an interrupt) */
#define HCC_OTG_GRXSTSP_PKTSTS_OUT_TFR_COMPLETED 3u

/* 0100: SETUP transaction completed (triggers an interrupt) */
#define HCC_OTG_GRXSTSP_PKTSTS_SETUP_COMPLETED   4u

/* 0110: SETUP data packet received */
#define HCC_OTG_GRXSTSP_PKTSTS_SETUP_RECEIVED    6u


#define HCC_OTG_GRXFSIZ( base )                      *(volatile uint32_t *)( ( base ) + 0x0024u )
#define HCC_SET_GRXFSIZ_RXFD( base, val )            HCC_OTG_GRXFSIZ( base ) = ( ( val ) & 0xFFFFu );
#define HCC_GET_GRXFSIZ_RXFD( base )                 ( HCC_OTG_GRXSTSR( base ) & 0xFFFFu )

#define HCC_OTG_GNPTXFSIZ( base )                    *(volatile uint32_t *)( ( base ) + 0x0028u )
#define HCC_SET_DIEPTXF( base_dieptxf, depth, addr ) ( base_dieptxf ) = ( ( ( depth ) << 16 ) | ( addr ) )

#define HCC_SET_NPTXFSA( base, val ) \
  HCC_OTG_GNPTXFSIZ( base ) &= ~0xFFFFu; \
  HCC_OTG_GNPTXFSIZ( base ) |= ( ( val ) & 0xFFFFu );
#define HCC_GET_NPTXFSA( base )                      ( HCC_OTG_GNPTXFSIZ( base ) & 0xFFFFu )
#define HCC_SET_NPTXFD( base, val ) \
  HCC_OTG_GNPTXFSIZ( base ) &= ~( 0xFFFFu << 16 ); \
  HCC_OTG_GNPTXFSIZ( base ) |= ( ( ( val ) & 0xFFFFu ) << 16 );
#define HCC_GET_NPTXFD( base )                       ( ( HCC_OTG_GNPTXFSIZ( base ) & ( 0xFFFFu << 16 ) ) >> 16 )

#define HCC_OTG_GNPTXSTS( base )                     *(volatile uint32_t *)( ( base ) + 0x002Cu )
#define HCC_GET_NPTXFSAV( base )                     ( HCC_OTG_GNPTXSTS( base ) & 0xFFFFu )
#define HCC_GET_NPTQXSAV( base )                     ( ( HCC_OTG_GNPTXSTS( base ) & ( 0xFFu << 16 ) ) >> 16 )
#define HCC_GET_NPTXQTOP( base )                     ( ( HCC_OTG_GNPTXSTS( base ) & ( 0x7Fu << 24 ) ) >> 24 )

#define HCC_OTG_GCCFG( base )                        *(volatile uint32_t *)( ( base ) + 0x0038u )
#define HCC_PWRDWN    ( 1u << 16 )
#define HCC_VBUSASEN  ( 1u << 18 )
#define HCC_VBUSBSEN  ( 1u << 19 )
#define HCC_SOFOUTEN  ( 1u << 20 )
#define HCC_NOVBUSSEN ( 1u << 21 )
#define HCC_VBDEN     ( 1u << 21 ) /* newer synopsys cores contain VBDEN bit in OTG_GCCFG */
#define HCC_PHYHSEN   ( 1u << 23 )

#define HCC_OTG_CID( base )            *(volatile uint32_t *)( ( base ) + 0x003Cu )
#define HCC_OTG_CID_HW_VERSION( base ) ( HCC_OTG_CID( base ) >> 12 )

#define HCC_OTG_GHWCFG2( base )        *(volatile uint32_t *)( ( base ) + 0x0048u )

#define HCC_OTG_DIEPTXF_N( base, ep )  *(volatile uint32_t *)( ( base ) + 0x0104u + ( ( ( ep ) - 1 ) * 0x4 ) )
#define HCC_SET_INEPTXSA( base, ep, val ) \
  HCC_OTG_DIEPTXF_N( base, ep ) &= ~0xFFFFu; \
  HCC_OTG_DIEPTXF_N( base, ep ) |= ( ( val ) & 0xFFFFu );
#define HCC_GET_INEPTXSA( base, ep )   ( HCC_OTG_DIEPTXF_N( ( base ), ( ep ) ) & 0xFFFFu )
#define HCC_SET_INEPTXFD( base, ep, val ) \
  HCC_OTG_DIEPTXF_N( base, ep ) &= ~( (uint32_t)( 0xFFFFu << 16 ) ); \
  HCC_OTG_DIEPTXF_N( base, ep ) |= ( ( (uint32_t)( val ) & 0xFFFFu ) << 16 );
#define HCC_GET_INEPTXFD( base, ep )   ( ( HCC_OTG_DIEPTXF_N( ( base ), ( ep ) ) & ( 0xFFFFu << 16 ) ) >> 16 )

#define HCC_OTG_DCFG( base )           *(volatile uint32_t *)( ( base ) + 0x0800u )
#define HCC_OTG_DCFG_DSPD_SET( base, val ) \
  HCC_OTG_DCFG( base ) &= ~0x3u; \
  HCC_OTG_DCFG( base ) |= ( ( val ) & 0x3u );
#define HCC_OTG_DCFG_DSPD_GET( base )  ( HCC_OTG_DCFG( base ) & 0x3u )
#define HCC_OTG_DCFG_DSPD_FULLSPEED      ( 3u )
#define HCC_OTG_DCFG_DSPD_FULLSPEED_ULPI ( 1u )
#define HCC_OTG_DCFG_DSPD_XCVRDLY        ( 1u << 14 )
#define HCC_OTG_DCFG_DSPD_NZLSOHSK       ( 1u << 2 )
#define HCC_OTG_DCFG_DAD_SET( base, val ) \
  HCC_OTG_DCFG( base ) &= ~( 0x7Fu << 4 ); \
  HCC_OTG_DCFG( base ) |= ( ( ( val ) & 0x7Fu ) << 4 );
#define HCC_OTG_DCFG_DAD_GET( base )   ( ( HCC_OTG_DCFG( base ) & ( 0x7Fu << 4 ) ) >> 4 )
#define HCC_OTG_DCFG_PFIVL_SET( base, val ) \
  HCC_OTG_DCFG( base ) &= ~( 0x3u << 11 ); \
  HCC_OTG_DCFG( base ) |= ( ( ( val ) & 0x3u ) << 11 );
#define HCC_OTG_DCFG_PFIVL_GET( base ) ( ( HCC_OTG_DCFG( base ) & ( 0x3u << 11 ) ) >> 11 )
#define HCC_OTG_DCFG_DESC_DMA           ( 1u << 23 )

#define HCC_OTG_DCTL( base )           *(volatile uint32_t *)( ( base ) + 0x0804u )
#define HCC_RWUSIG                      ( 1u << 0 )
#define HCC_SDIS                        ( 1u << 1 )
#define HCC_GINSTS                      ( 1u << 2 )
#define HCC_GONSTS                      ( 1u << 3 )
#define HCC_SET_TCTL( base, val ) \
  HCC_OTG_DCTL( base ) &= ~( 0x7u << 4 ); \
  HCC_OTG_DCTL( base ) |= ( ( ( val ) & 0x7u ) << 4 );
#define HCC_GET_TCTL( base ) ( ( HCC_OTG_DCTL( base ) & ( 0x7u << 4 ) ) >> 4 )
#define HCC_SGINAK                      ( 1u << 7 )
#define HCC_CGINAK                      ( 1u << 8 )
#define HCC_SGONAK                      ( 1u << 9 )
#define HCC_CGONAK                      ( 1u << 10 )
#define HCC_POPRGDNE                    ( 1u << 11 )

#define HCC_OTG_DSTS( base )             *(volatile uint32_t *)( ( base ) + 0x0808u )
#define HCC_OTG_DSTS_SUSPSTS            ( 1u << 0 )
#define HCC_OTG_DSTS_ENUMSPD_GET( base ) HCC_OTG_GET_VAL( HCC_OTG_DSTS( base ), 1u, 0x3u )
#define HCC_OTG_DSTS_ENUMPSD_HIGH_30_60 ( 0u << 1 )
#define HCC_OTG_DSTS_ENUMPSD_FULL_30_60 ( 1u << 1 )
#define HCC_OTG_DSTS_ENUMPSD_LOW_6      ( 2u << 1 )
#define HCC_OTG_DSTS_ENUMPSD_FULL_48    ( 3u << 1 )
#define HCC_OTG_DSTS_EERR               ( 1u << 3 )
#define HCC_OTG_DSTS_FNSOF_GET( base ) HCC_OTG_GET_VAL( HCC_OTG_DSTS( base ), 8u, 0x3fffu )

#define HCC_OTG_DIEPMSK( base )        *(volatile uint32_t *)( ( base ) + 0x0810u )
#define HCC_OTG_DIEPMSK_XFRCM           ( 1u << 0 )
#define HCC_OTG_DIEPMSK_EPDM            ( 1u << 1 )
#define HCC_OTG_DIEPMSK_TOM             ( 1u << 3 )
#define HCC_OTG_DIEPMSK_ITTXFEMSK       ( 1u << 4 )
#define HCC_OTG_DIEPMSK_INEPNMM         ( 1u << 5 )
#define HCC_OTG_DIEPMSK_INEPNEM         ( 1u << 6 )
#define HCC_OTG_DIEPMSK_TXFURM          ( 1u << 8 )
#define HCC_OTG_DIEPMSK_BIM             ( 1u << 9 )

#define HCC_OTG_DOEPMSK( base ) *(volatile uint32_t *)( ( base ) + 0x0814u )
#define HCC_OTG_DOEPMSK_XFRCM           ( 1u << 0 )
#define HCC_OTG_DOEPMSK_EPDM            ( 1u << 1 )
#define HCC_OTG_DOEPMSK_STUPM           ( 1u << 3 )
#define HCC_OTG_DOEPMSK_OTEPDM          ( 1u << 4 )
#define HCC_OTG_DOEPMSK_STSPHSRXM       ( 1u << 5 )
#define HCC_OTG_DOEPMSK_B2BSTUP         ( 1u << 6 )
#define HCC_OTG_DOEPMSK_OPEM            ( 1u << 8 )
#define HCC_OTG_DOEPMSK_BOIM            ( 1u << 9 )

#define HCC_OTG_DAINT( base )                        *(volatile uint32_t *)( ( base ) + 0x0818u )
#define HCC_OTG_DAINT_IEPINT_MASK       0xFFu
#define HCC_OTG_DAINT_IEPINT_POS        0u
#define HCC_OTG_DAINT_IEPINT_GET( base )             HCC_OTG_GET_VAL( HCC_OTG_DAINT( base ),  HCC_OTG_DAINT_IEPINT_POS, HCC_OTG_DAINT_IEPINT_MASK )
#define HCC_OTG_DAINT_OEPINT_MASK       0xFFu
#define HCC_OTG_DAINT_OEPINT_POS        16u
#define HCC_OTG_DAINT_OEPINT_GET( base )             HCC_OTG_GET_VAL( HCC_OTG_DAINT( base ),  HCC_OTG_DAINT_OEPINT_POS, HCC_OTG_DAINT_OEPINT_MASK )

#define HCC_OTG_DAINTMSK( base )                     *(volatile uint32_t *)( ( base ) + 0x081Cu )
#define HCC_OTG_DAINTMSK_IEPM_MASK      0xFFu
#define HCC_OTG_DAINTMSK_IEPM_POS       0u
#define HCC_OTG_DAINTMSK_IEPM_GET( base )            HCC_OTG_GET_VAL( HCC_OTG_DAINTMSK( base ),  HCC_OTG_DAINTMSK_IEPM_POS, HCC_OTG_DAINTMSK_IEPM_MASK )
#define HCC_OTG_DAINTMSK_OEPM_MASK      0xFFu
#define HCC_OTG_DAINTMSK_OEPM_POS       16u
#define HCC_OTG_DAINTMSK_OEPM_GET( base )            HCC_OTG_GET_VAL( HCC_OTG_DAINTMSK( base ),  HCC_OTG_DAINTMSK_OEPM_POS, HCC_OTG_DAINTMSK_OEPM_MASK )
#define HCC_OTG_DAINTMSK_IEPM_SET( base, ep )        HCC_OTG_DAINTMSK( base ) |= ( 1u << ( ep ) )
#define HCC_OTG_DAINTMSK_IEPM_CLR( base, ep )        HCC_OTG_DAINTMSK( base ) &= ~( 1u << ( ep ) )
#define HCC_OTG_DAINTMSK_OEPM_SET( base, ep )        HCC_OTG_DAINTMSK( base ) |= ( 1u << ( 16u + ( ep ) ) )
#define HCC_OTG_DAINTMSK_OEPM_CLR( base, ep )        HCC_OTG_DAINTMSK( base ) &= ~( 1u << ( 16u + ( ep ) ) )

#define HCC_OTG_DTKNQR1( base )                      *(volatile uint32_t *)( ( base ) + 0x0820u )
#define HCC_OTG_DTKNQR1_WRAPBIT ( 1u << 7 )
#define HCC_OTG_DTKNQR2( base )                      *(volatile uint32_t *)( ( base ) + 0x0824u )

#define HCC_OTG_DVBUSDIS( base )                     *(volatile uint32_t *)( ( base ) + 0x0828u )

#define HCC_OTG_DVBUSPULSE( base )                   *(volatile uint32_t *)( ( base ) + 0x082Cu )

#define HCC_OTG_DIEPEMPMSK( base )                   *(volatile uint32_t *)( ( base ) + 0x0834u )
#define HCC_OTG_DIEPEMPMSK_INEPTXFEM_SET( base, ep ) HCC_OTG_DIEPEMPMSK( base ) |= ( 1u << ( ep ) )
#define HCC_OTG_DIEPEMPMSK_INEPTXFEM_CLR( base, ep ) HCC_OTG_DIEPEMPMSK( base ) &= ~( 1u << ( ep ) )
#define HCC_OTG_DIEPEMPMSK_INEPTXFEM_GET( base, ep ) ( ( HCC_OTG_DIEPEMPMSK( base ) >> ( ep ) ) & 1u )

#define HCC_OTG_DIEPCTL0( base )                     *(volatile uint32_t *)( ( base ) + 0x0900u )
#define HCC_OTG_DIEPCTL0_MPSIZ_SET( base, val ) \
  HCC_OTG_DIEPCTL0( base ) &= ~0x3u; \
  HCC_OTG_DIEPCTL0( base ) |= ( ( val ) & 0x3u );
#define HCC_OTG_DIEPCTL0_MPSIZ_GET( base )           ( HCC_OTG_DIEPCTL0( base ) & 0x3u )
#define HCC_OTG_DIEPCTL0_USBAEP ( 1u << 15 )
#define HCC_OTG_DIEPCTL0_NAKSTS ( 1u << 17 )
#define HCC_OTG_DIEPCTL0_EPTYP_SET( base, val ) \
  HCC_OTG_DIEPCTL0( base ) &= ~( 0x3u << 18 ); \
  HCC_OTG_DIEPCTL0( base ) |= ( ( ( val ) & 0x3u ) << 18 );
#define HCC_OTG_DIEPCTL0_EPTYP_GET( base )           ( ( HCC_OTG_DIEPCTL0( base ) & ( 0x3u << 18 ) ) >> 18 )
#define HCC_OTG_DIEPCTL0_STALL  ( 1u << 21 )
#define HCC_OTG_DIEPCTL0_TXFNUM_SET( base, val ) \
  HCC_OTG_DIEPCTL0( base ) &= ~( 0xfu << 22 ); \
  HCC_OTG_DIEPCTL0( base ) |= ( ( ( val ) & 0xfu ) << 22 );
#define HCC_OTG_DIEPCTL0_TXFNUM_GET( base )          ( ( HCC_OTG_DIEPCTL0( base ) & ( 0xfu << 22 ) ) >> 22 )
#define HCC_OTG_DIEPCTL0_CNAK   ( 1u << 26 )
#define HCC_OTG_DIEPCTL0_SNAK   ( 1u << 27 )
#define HCC_OTG_DIEPCTL0_EPDIS  ( 1u << 30 )
#define HCC_OTG_DIEPCTL0_EPENA  ( 1u << 31 )

#define HCC_OTG_DIEPCTL_N( base, N )           *(volatile uint32_t *)( ( base ) + 0x0900u + ( ( N ) * 0x20u ) )
#define HCC_OTG_DIEPCTL_N_MPSIZ_SET( base, N, val ) \
  HCC_OTG_DIEPCTL_N( base, N ) &= ~0x7ffu; \
  HCC_OTG_DIEPCTL_N( base, N ) |= ( ( val ) & 0x7ffu );
#define HCC_OTG_DIEPCTL_N_MPSIZ_GET( base, N ) ( HCC_OTG_DIEPCTL_N( base, N ) & 0x7ffu )
#define HCC_DIEPCTL_N_USBAEP         ( 1u << 15 )
#define HCC_DIEPCTL_N_EONUM_DPID     ( 1u << 16 )
#define HCC_DIEPCTL_N_NAKSTS         ( 1u << 17 )
#define HCC_OTG_DIEPCTL_N_EPTYP_SET( base, N, val ) \
  HCC_OTG_DIEPCTL_N( base, N ) &= ~( 0x3u << 18 ); \
  HCC_OTG_DIEPCTL_N( base, N ) |= ( ( ( val ) & 0x3u ) << 18 );
#define HCC_OTG_DIEPCTL_N_EPTYP_GET( base, N )  ( ( HCC_OTG_DIEPCTL_N( base, N ) & ( 0x3u << 18 ) ) >> 18 )
#define HCC_DIEPCTL_N_STALL          ( 1u << 21 )
#define HCC_OTG_DIEPCTL_N_TXFNUM_SET( base, N, val ) \
  HCC_OTG_DIEPCTL_N( base, N ) &= ~( 0xfu << 22 ); \
  HCC_OTG_DIEPCTL_N( base, N ) |= ( ( ( val ) & 0xfu ) << 22 );
#define HCC_OTG_DIEPCTL_N_TXFNUM_GET( base, N ) ( ( HCC_OTG_DIEPCTL_N( base, N ) & ( 0xfu << 22 ) ) >> 22 )
#define HCC_DIEPCTL_N_EONUM          ( 1u << 16 )
#define HCC_DIEPCTL_N_CNAK           ( 1u << 26 )
#define HCC_DIEPCTL_N_SNAK           ( 1u << 27 )
#define HCC_DIEPCTL_N_SD0PID_SEVNFRM ( 1u << 28 )
#define HCC_DIEPCTL_N_SD1PID         ( 1u << 29 )
#define HCC_DIEPCTL_N_EPDIS          ( 1u << 30 )
#define HCC_DIEPCTL_N_EPENA          ( 1u << 31 )

#define HCC_OTG_DIEPINT_N( base, N ) *(volatile uint32_t *)( ( base ) + 0x0908u + ( ( N ) * 0x20u ) )
#define HCC_DIEPINT_N_XFRC           ( 1u << 0 )
#define HCC_DIEPINT_N_EPDISD         ( 1u << 1 )
#define HCC_DIEPINT_N_AHBERR         ( 1u << 2 )
#define HCC_DIEPINT_N_TOC            ( 1u << 3 )
#define HCC_DIEPINT_N_ITTXFE         ( 1u << 4 )
#define HCC_DIEPINT_N_INEPNM         ( 1u << 5 )
#define HCC_DIEPINT_N_INEPNE         ( 1u << 6 )
#define HCC_DIEPINT_N_TXFE           ( 1u << 7 )
#define HCC_DIEPINT_N_TXFIFOUDRN     ( 1u << 8 )
#define HCC_DIEPINT_N_PKTDRPSTS      ( 1u << 11 )
#define HCC_DIEPINT_N_NAK            ( 1u << 13 )
#define HCC_DIEPINT_N_ALL_MASK \
  ( HCC_DIEPINT_N_XFRC | HCC_DIEPINT_N_EPDISD | HCC_DIEPINT_N_AHBERR  \
   | HCC_DIEPINT_N_TOC | HCC_DIEPINT_N_ITTXFE | HCC_DIEPINT_N_INEPNM   \
   | HCC_DIEPINT_N_TXFIFOUDRN | HCC_DIEPINT_N_PKTDRPSTS | HCC_DIEPINT_N_NAK )

#define HCC_OTG_DIEPTSIZ0( base )              *(volatile uint32_t *)( ( base ) + 0x0910u )
#define HCC_SET_DIEPTSIZ0_XFRSIZ( base, val ) \
  HCC_OTG_DIEPTSIZ0( base ) &= ~0x7fu; \
  HCC_OTG_DIEPTSIZ0( base ) |= ( ( val ) & 0x7fu );
#define HCC_GET_DIEPTSIZ0_XFRSIZ( base )       ( HCC_OTG_DIEPTSIZ0( base ) & 0x7fu )

#define HCC_SET_DIEPTSIZ0_PKTCNT( base, val ) \
  HCC_OTG_DIEPTSIZ0( base ) &= ~( 0x3u << 19 ); \
  HCC_OTG_DIEPTSIZ0( base ) |= ( ( (uint32_t)val & 0x3u ) << 19 );
#define HCC_GET_DIEPTSIZ0_PKTCNT( base )       ( ( HCC_OTG_DIEPTSIZ0( base ) & ( 0x3u << 19 ) ) >> 19 )

#define HCC_OTG_DIEPTSIZ_N( base, N )          *(volatile uint32_t *)( ( base ) + 0x0910u + ( ( N ) * 0x20u ) )
#define HCC_SET_DIEPTSIZ_N_XFRSIZ( base, N, val ) \
  HCC_OTG_DIEPTSIZ_N( base, N ) &= ~0x7ffu; \
  HCC_OTG_DIEPTSIZ_N( base, N ) |= ( ( val ) & 0x7ffu );
#define HCC_GET_DIEPTSIZ_N_XFRSIZ( base, N )   ( HCC_OTG_DIEPTSIZ_N( base, N ) & 0x7ffu )
#define HCC_SET_DIEPTSIZ_N_PKTCNT( base, N, val ) \
  HCC_OTG_DIEPTSIZ_N( base, N ) &= ~( 0xfu << 19 ); \
  HCC_OTG_DIEPTSIZ_N( base, N ) |= ( ( ( val ) & 0xfu ) << 19 );
#define HCC_GET_DIEPTSIZ_N_PKTCNT( base, N )   ( ( HCC_OTG_DIEPTSIZ_N( base, N ) & ( 0xfu << 19 ) ) >> 19 )
#define HCC_SET_DIEPTSIZ_N_MCNT( base, N, val ) \
  HCC_OTG_DIEPTSIZ_N( base, N ) &= ~( 0x3u << 29 ); \
  HCC_OTG_DIEPTSIZ_N( base, N ) |= ( ( ( val ) & 0x3u ) << 29 );
#define HCC_GET_DIEPTSIZ_N_MCNT( base, N )     ( ( HCC_OTG_DIEPTSIZ_N( base, N ) & ( 0x3u << 19 ) ) >> 29 )

#define HCC_OTG_DIEPDMA0( base )               *(volatile uint32_t *)( ( base ) + 0x0914u )

#define HCC_OTG_DIEPDMA_N( base, N )           *(volatile uint32_t *)( ( base ) + 0x0914u + ( ( N ) * 0x20u ) )

#define HCC_OTG_DTXFSTS_N( base, N )           *(volatile uint32_t *)( ( base ) + 0x0918u + ( ( N ) * 0x20u ) )
#define HCC_GET_DTXFSTS_N_INEPTFSAV( base, N ) ( HCC_OTG_DTXFSTS_N( base, N ) & 0xffff )

#define HCC_OTG_DIEPDMAB0( base )              *(volatile uint32_t *)( ( base ) + 0x091Cu )

#define HCC_OTG_DIEPDMAB_N( base, N )          *(volatile uint32_t *)( ( base ) + 0x091Cu + ( ( N ) * 0x20u ) )

#define HCC_OTG_DOEPCTL0( base )               *(volatile uint32_t *)( ( base ) + 0x0B00u )
#define HCC_SET_DOEPCTL0_MPSIZ( base, val ) \
  HCC_OTG_DOEPCTL0( base ) &= ~0x3u; \
  HCC_OTG_DOEPCTL0( base ) |= ( ( val ) & 0x3u );
#define HCC_GET_DOEPCTL0_MPSIZ( base )         (uint8_t)( HCC_OTG_DOEPCTL0( base ) & 0x3u )
#define HCC_OTG_DOEPCTL0_MPSIZ_64 0u
#define HCC_OTG_DOEPCTL0_MPSIZ_32 1u
#define HCC_OTG_DOEPCTL0_MPSIZ_16 2u
#define HCC_OTG_DOEPCTL0_MPSIZ_8  3u

#define HCC_DOEPCTL0_USBAEP       ( 1u << 15 )
#define HCC_DOEPCTL0_NAKSTS       ( 1u << 17 )
#define HCC_SET_DOEPCTL0_EPTYP( base, val ) \
  HCC_OTG_DOEPCTL0( base ) &= ~( 0x3u << 18 ); \
  HCC_OTG_DOEPCTL0( base ) |= ( ( ( val ) & 0x3u ) << 18 );
#define HCC_GET_DOEPCTL0_EPTYP( base ) ( ( HCC_OTG_DOEPCTL0( base ) & ( 0x3u << 18 ) ) >> 18 )
#define HCC_DOEPCTL0_USBAEP       ( 1u << 15 )
#define HCC_DOEPCTL0_SNPM         ( 1u << 20 )
#define HCC_DOEPCTL0_STALL        ( 1u << 21 )
#define HCC_DOEPCTL0_CNAK         ( 1u << 26 )
#define HCC_DOEPCTL0_SNAK         ( 1u << 27 )
#define HCC_DOEPCTL0_EPDIS        ( 1u << 30 )
#define HCC_DOEPCTL0_EPENA        ( 1u << 31 )

#define HCC_OTG_DOEPCTL_N( base, N )       *(volatile uint32_t *)( ( base ) + 0x0B00u + ( ( N ) * 0x20u ) )
#define HCC_OTG_DOEPCTL_N_MPSIZ_SET( base, N, val ) \
  HCC_OTG_DOEPCTL_N( base, N ) &= ~0x7ffu; \
  HCC_OTG_DOEPCTL_N( base, N ) |= ( ( val ) & 0x7ffu );
#define HCC_GET_DOEPCTL_N_MPSIZ( base, N ) ( HCC_OTG_DOEPCTL_N( base, N ) & 0x7ffu )
#define HCC_DOEPCTL_N_USBAEP         ( 1u << 15 )
#define HCC_DOEPCTL_N_EONUM_DPID     ( 1u << 16 )
#define HCC_DOEPCTL_N_NAKSTS         ( 1u << 17 )
#define HCC_OTG_DOEPCTL_N_EPTYP_SET( base, N, val ) \
  HCC_OTG_DOEPCTL_N( base, N ) &= ~( 0x3u << 18 ); \
  HCC_OTG_DOEPCTL_N( base, N ) |= ( ( ( val ) & 0x3u ) << 18 );
#define HCC_GET_DOEPCTL_N_EPTYP( base, N ) ( ( HCC_OTG_DOEPCTL_N( base, N ) & ( 0x3u << 18 ) ) >> 18 )
#define HCC_DOEPCTL_N_SNPM           ( 1u << 20 )
#define HCC_DOEPCTL_N_STALL          ( 1u << 21 )
#define HCC_DOEPCTL_N_CNAK           ( 1u << 26 )
#define HCC_DOEPCTL_N_SNAK           ( 1u << 27 )
#define HCC_DOEPCTL_N_SD0PID_SEVNFRM ( 1u << 28 )
#define HCC_DOEPCTL_N_SD0PID_SODDFRM ( 1u << 29 )
#define HCC_DOEPCTL_N_EPDIS          ( 1u << 30 )
#define HCC_DOEPCTL_N_EPENA          ( 1u << 31 )

#define HCC_OTG_DOEPINT_N( base, N ) *(volatile uint32_t *)( ( base ) + 0x0B08u + ( ( N ) * 0x20u ) )
#define HCC_DOEPINT_N_XFRC           ( 1u << 0 )
#define HCC_DOEPINT_N_EPDISD         ( 1u << 1 )
#define HCC_DOEPINT_N_STUP           ( 1u << 3 )
#define HCC_DOEPINT_N_OTEPDIS        ( 1u << 4 )
#define HCC_DOEPINT_N_STSPHSRX       ( 1u << 5 )
#define HCC_DOEPINT_N_B2BSTUP        ( 1u << 6 )
#define HCC_DOEPINT_N_OUTPKTERR      ( 1u << 8 )
#define HCC_DOEPINT_N_BERR           ( 1u << 12 )
#define HCC_DOEPINT_N_NAK            ( 1u << 13 )
#define HCC_DOEPINT_N_NYET           ( 1u << 14 )
#define HCC_DOEPINT_N_STUPPKTRCVD    ( 1u << 15 )

#define HCC_DOEPINT_N_ALL_MASK \
  ( HCC_DOEPINT_N_XFRC | HCC_DOEPINT_N_EPDISD | HCC_DOEPINT_N_STUP   \
   | HCC_DOEPINT_N_OTEPDIS | HCC_DOEPINT_N_STSPHSRX | HCC_DOEPINT_N_OUTPKTERR   \
   | HCC_DOEPINT_N_B2BSTUP | HCC_DOEPINT_N_BERR | HCC_DOEPINT_N_NAK   \
   | HCC_DOEPINT_N_NYET | HCC_DOEPINT_N_STUPPKTRCVD )

#define HCC_OTG_DOEPTSIZ0( base )                        *(volatile uint32_t *)( ( base ) + 0x0B10u )
#define HCC_OTG_DOEPTSIZ0_XFRSIZ_SET( base, val ) \
  HCC_OTG_DOEPTSIZ0( base ) &= ~0x7fu; \
  HCC_OTG_DOEPTSIZ0( base ) |= ( ( val ) & 0x7fu );
#define HCC_OTG_DOEPTSIZ0_XFRSIZ_GET( base )             ( HCC_OTG_DOEPTSIZ0( base ) & 0x7fu )
#define HCC_OTG_DOEPTSIZ0_XFRSIZ( val )                  ( ( val ) << 0u )
#define HCC_OTG_DOEPTSIZ0_PKTCNT ( 1u << 19 )
#define HCC_OTG_DOEPTSIZ0_STUPCNT_SET( base, val ) \
  HCC_OTG_DOEPTSIZ0( base ) &= ~( 0x3u << 29 ); \
  HCC_OTG_DOEPTSIZ0( base ) |= ( ( ( val ) & 0x3u ) << 29 );
#define HCC_OTG_DOEPTSIZ0_STUPCNT_GET( base )            ( ( HCC_OTG_DOEPTSIZ0( base ) & ( 0x3u << 29 ) ) >> 29 )
#define HCC_OTG_DOEPTSIZ0_STUPCNT( val )                 ( ( val ) << 29 )

#define HCC_OTG_DOEPTSIZ_N( base, N )                    *(volatile uint32_t *)( ( base ) + 0x0B10u + ( ( N ) * 0x20u ) )
#define HCC_OTG_DOEPTSIZ_N_XFRSIZ_SET( base, N, val ) \
  HCC_OTG_DOEPTSIZ_N( base, N ) &= ~0x7ffu; \
  HCC_OTG_DOEPTSIZ_N( base, N ) |= ( ( val ) & 0x7ffu );
#define HCC_OTG_DOEPTSIZ_N_XFRSIZ_GET( base, N )         ( HCC_OTG_DOEPTSIZ_N( base, N ) & 0x7ffu )
#define HCC_OTG_DOEPTSIZ_N_PKTCNT_SET( base, N, val ) \
  HCC_OTG_DOEPTSIZ_N( base, N ) &= ~( 0xfu << 19 ); \
  HCC_OTG_DOEPTSIZ_N( base, N ) |= ( ( ( val ) & 0xfu ) << 19 );
#define HCC_OTG_DOEPTSIZ_N_PKTCNT_GET( base, N )         ( ( HCC_OTG_DOEPTSIZ_N( base, N ) & ( 0xfu << 19 ) ) >> 19 )
#define HCC_OTG_DOEPTSIZ_N_RXDPID_STUPCNT_SET( base, N, val ) \
  HCC_OTG_DOEPTSIZ_N( base, N ) &= ~( 0x3u << 29 ); \
  HCC_OTG_DOEPTSIZ_N( base, N ) |= ( ( ( val ) & 0x3u ) << 29 );
#define HCC_OTG_DOEPTSIZ_N_RXDPID_STUPCNT_GET( base, N ) ( ( HCC_OTG_DOEPTSIZ_N( base, N ) & ( 0x3u << 29 ) ) >> 29 )

#define HCC_OTG_DOEPDMA0( base )                         *(volatile uint32_t *)( ( base ) + 0x0B14u )

#define HCC_OTG_DOEPDMA_N( base, N )                     *(volatile uint32_t *)( ( base ) + 0x0B14u + ( ( N ) * 0x20u ) )

#define HCC_OTG_DOEPDMAB0( base )                        *(volatile uint32_t *)( ( base ) + 0x0B1Cu )

#define HCC_OTG_DOEPDMAB_N( base, N )                    *(volatile uint32_t *)( ( base ) + 0x0B1Cu + ( ( N ) * 0x20u ) )

#define HCC_OTG_PCGCCTL( base )                          *(volatile uint32_t *)( ( base ) + 0x0E00u )
#define HCC_PCGCCTL_STPPCLK  ( 1u << 0 )
#define HCC_PCGCCTL_GATEHCLK ( 1u << 1 )
#define HCC_PCGCCTL_PHYSUSP  ( 1u << 4 )

/* Base address of the HCC_OTG/OTG_HS peripheral and the data FIFO */
#define HCC_OTG_DATA_FIFO_BASE_ADDR( base )      (uint32_t)( ( base ) + 0x1000u )
#define HCC_OTG_DATA_FIFO_TX_CH_ADDR( base, ch ) (uint32_t)( HCC_OTG_DATA_FIFO_BASE_ADDR( base ) + ( ( ch ) * 0x1000u ) )
#define HCC_OTG_DATA_FIFO_TX_CH( base, ch )      *(volatile uint32_t *)( HCC_OTG_DATA_FIFO_BASE_ADDR( base ) + ( ( ch ) * 0x1000u ) )
#define HCC_OTG_DATA_FIFO_RX_ADDR( base )        HCC_OTG_DATA_FIFO_BASE_ADDR( base )
#define HCC_OTG_DATA_FIFO_RX( base )             *(volatile uint32_t *)( HCC_OTG_DATA_FIFO_BASE_ADDR( base ) )


/******************************* USB OTG FS for HOST ******************************/
#define HCC_OTG_FS_BASE        0x40040000

/***** OTG *****/
#define HCC_OTG_OTGCTL_HOST    *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x000 )

#define HCC_OTG_GOTGINT_HOST   *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x004 )

#define HCC_OTG_GAHBCFG_HOST   *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x008 )
#define HCC_GINT_m             1u
#define HCC_GINT_s             0

#define HCC_TXFELVL_m          1u
#define HCC_TXFELVL_s          7

#define HCC_PTXFELVL_m         1u
#define HCC_PTXFELVL_s         8

#define HCC_OTG_GUSBCFG_HOST   *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x00c )
#define HCC_FHMOD_m            1u
#define HCC_FHMOD_s            29
#define HCC_ULPIEVBUSD_m       1u
#define HCC_ULPIEVBUSD_s       20

#define HCC_OTG_GRSTCTL_HOST   *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x010 )
#define HCC_CRST_m             1u
#define HCC_CRST_s             0

#define HCC_RXFFLSH_m          1u
#define HCC_RXFFLSH_s          4

#define HCC_TXFFLSH_m          1u
#define HCC_TXFFLSH_s          5

#define HCC_TXFNUM_m           0x1f
#define HCC_TXFNUM_s           6

#define HCC_ULPIEVBUSD_m       1u
#define HCC_ULPIEVBUSD_s       20

#define HCC_AHBIDL_m           1u
#define HCC_AHBIDL_s           31

#define HCC_OTG_GINTSTS_HOST   *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x014 )
#define HCC_OTG_GINTMSK_HOST   *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x018 )
#define HCC_HCINT_m            1u
#define HCC_HCINT_s            25

#define HCC_HPRTINT_m          1u
#define HCC_HPRTINT_s          24

#define HCC_RXFLVL_m           1u
#define HCC_RXFLVL_s           4

#define HCC_SOF_m              1u
#define HCC_SOF_s              3

#define HCC_OTG_GRXSTSR_HOST   *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x01c )
#define HCC_OTG_GRXSTSP_HOST   *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x020 )
#define HCC_PKTSTS_m           0xf
#define HCC_PKTSTS_s           17
#define HCC_PKTSTS_IN_RECEIVED 2

#define HCC_RDPID_m            0x3
#define HCC_RDPID_s            15

#define HCC_BCNT_m             0x7ff
#define HCC_BCNT_s             4

#define HCC_CHNUM_m            0xf
#define HCC_CHNUM_s            0

#define HCC_OTG_GRXFSIZ_HOST   *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x024 )
#define HCC_RXFD_m             0xffff
#define HCC_RXFD_s             0

#define HCC_OTG_GNPTXFSIZ_HOST *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x028 )
#define HCC_NPTXFSA_m          0xffff
#define HCC_NPTXFSA_s          0

#define HCC_NPTXFD_m           0xffff
#define HCC_NPTXFD_s           16

#define HCC_OTG_GNPTXSTS_HOST  *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x02c )

#define HCC_OTG_GCCFG_HOST     *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x038 )
#define HCC_PWRDWN_m           1u
#define HCC_PWRDWN_s           16

#define HCC_OTG_CID_HOST       *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x03c )

#define HCC_OTG_HPTXFSIZ_HOST  *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x100 )
#define HCC_PTXSA_m            0xffff
#define HCC_PTXSA_s            0
#define HCC_PTXFSIZ_m          0xffff
#define HCC_PTXFSIZ_s          16


/***** HOST *****/
#define HCC_OTG_HCFG           *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x400 )
#define HCC_FSLSS_m            1
#define HCC_FSLSS_s            2

#define HCC_FSLSPCS_m          0x3
#define HCC_FSLSPCS_s          0

#define HCC_OTG_HFIR           *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x404 )
#define HCC_FRIVL_m            0xffff
#define HCC_FRIVL_s            0

#define HCC_OTG_HFNUM          *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x408 )
#define HCC_FRNUM_m            0x3fff
#define HCC_FRNUM_s            0

#define HCC_FTREM_m            0xffff
#define HCC_FTREM_s            16

#define HCC_OTG_HPTXSTS        *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x410 )
#define HCC_PTXQTOP_m          0xff
#define HCC_PTXQTOP_s          24

#define HCC_PTXQSAV_m          0xff
#define HCC_PTXQSAV_s          16

#define HCC_PTXFSAVL_m         0xffff
#define HCC_PTXFSAVL_s         0

#define HCC_OTG_HAINT          *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x414 )
#define HCC_OTG_HAINTMSK       *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x418 )

#define HCC_OTG_HPRT           *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x440 )
#define HCC_PSPD_m             0x3
#define HCC_PSPD_s             17
#define HCC_PSPD_HIGH          0
#define HCC_PSPD_FULL          1
#define HCC_PSPD_LOW           2

#define HCC_PTCTL_m            0xf
#define HCC_PTCTL_s            13

#define HCC_PPWR_m             1u
#define HCC_PPWR_s             12

#define HCC_PLSTS_m            0x3
#define HCC_PLSTS_s            10

#define HCC_PRST_m             1u
#define HCC_PRST_s             8

#define HCC_PSUSP_m            1u
#define HCC_PSUSP_s            7

#define HCC_PRES_m             1u
#define HCC_PRES_s             6

#define HCC_POCCHNG_m          1u
#define HCC_POCCHNG_s          5

#define HCC_POCA_m             1u
#define HCC_POCA_s             4

#define HCC_PENCHNG_m          1u
#define HCC_PENCHNG_s          3

#define HCC_PENA_m             1u
#define HCC_PENA_s             2

#define HCC_PCDET_m            1u
#define HCC_PCDET_s            1

#define HCC_PCSTS_m            1u
#define HCC_PCSTS_s            0

#define HCC_OTG_HCCHAR( x ) *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x500 + ( x << 5 ) )
#define HCC_CHENA_m            1u
#define HCC_CHENA_s            31

#define HCC_CHDIS_m            1u
#define HCC_CHDIS_s            30

#define HCC_ODDFRM_m           1u
#define HCC_ODDFRM_s           29

#define HCC_DAD_m              0x7f
#define HCC_DAD_s              22

#define HCC_MC_m               0x3
#define HCC_MC_s               20

#define HCC_EPTYP_m            0x3
#define HCC_EPTYP_s            18
#define HCC_EPTYP_CONTROL      0
#define HCC_EPTYP_ISO          1
#define HCC_EPTYP_BULK         2
#define HCC_EPTYP_INTERRUPT    3

#define HCC_LSDEV_m            1u
#define HCC_LSDEV_s            17

#define HCC_EPDIR_m            1u
#define HCC_EPDIR_s            15

#define HCC_EPNUM_m            0xf
#define HCC_EPNUM_s            11

#define HCC_MPSIZ_m            0x7ff
#define HCC_MPSIZ_s            0

#define HCC_OTG_HCSPLT( x ) *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x504 + ( x << 5 ) )
#define HCC_SPLITEN_m          1u
#define HCC_SPLITEN_s          31

#define HCC_COMPLSPLT_m        1u
#define HCC_COMPLSPLT_s        16

#define HCC_XACTPOS_m          0x3
#define HCC_XACTPOS_s          14

#define HCC_HUBADDR_m          0x7f
#define HCC_HUBADDR_s          7

#define HCC_PRTADDR_m          0x7f
#define HCC_PRTADDR_s          0

#define HCC_OTG_HCINT( x )    *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x508 + ( x << 5 ) )
#define HCC_OTG_HCINTMSK( x ) *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x50c + ( x << 5 ) )
#define HCC_DTERR_m            1u
#define HCC_DTERR_s            10

#define HCC_FRMOR_m            1u
#define HCC_FRMOR_s            9

#define HCC_BBERR_m            1u
#define HCC_BBERR_s            8

#define HCC_TXERR_m            1u
#define HCC_TXERR_s            7

#define HCC_NYET_m             1u
#define HCC_NYET_s             6

#define HCC_ACK_m              1u
#define HCC_ACK_s              5

#define HCC_NAK_m              1u
#define HCC_NAK_s              4

#define HCC_STALL_m            1u
#define HCC_STALL_s            3

#define HCC_CHH_m              1u
#define HCC_CHH_s              1

#define HCC_XFRC_m             1u
#define HCC_XFRC_s             0


#define HCC_OTG_HCTSIZ( x ) *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0x510 + ( x << 5 ) )
#define HCC_DPID_m             0x3
#define HCC_DPID_s             29
#define HCC_DPID_DATA0         0
#define HCC_DPID_DATA2         1
#define HCC_DPID_DATA1         2
#define HCC_DPID_SETUP         3

#define HCC_PKTCNT_m           0x3ff
#define HCC_PKTCNT_s           19

#define HCC_XFRSIZ_m           0x7ffff
#define HCC_XFRSIZ_s           0

#define HCC_OTG_PCGCCTL_HOST   *(volatile uint32_t *)( HCC_OTG_FS_BASE + 0xE00 )

/***** BUFFER ACCESS *****/
#define HCC_OTG_DFIFO( x ) (volatile uint32_t *)( HCC_OTG_FS_BASE + 0x1000 + ( x << 12 ) )


#ifdef __cplusplus
}
#endif


#endif /* ifndef _USBD_SYNOPSYS_OTG_REGS_H_*/
