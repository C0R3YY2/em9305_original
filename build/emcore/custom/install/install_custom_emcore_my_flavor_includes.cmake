SET(dst "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/emcore/bin/v4.2/my_flavor")

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/AES.h"
    "${path}/AES_CBC.h"
    "${path}/AES_CCM.h"
    "${path}/AES_CMAC.h"
    "${path}/AES_CTR.h"
    "${path}/AES_ECB.h"
    "${path}/AES_GCM.h"
    "${path}/ECCSchnoor.h"
    "${path}/ECCStatus.h"
    "${path}/ECCTypedef.h"
    "${path}/ECCVerifyPubKey.h"
    "${path}/ECCkeyManagement.h"
    "${path}/ECDH.h"
    "${path}/ECDSA_Verify.h"
    "${path}/ExampleECCUtilities.h"
    "${path}/PRNG.h"
    "${path}/PRNG_NIST_SP_800_90A.h"
    "${path}/SHA1.h"
    "${path}/SHA224.h"
    "${path}/SHA256.h"
    "${path}/SHA384.h"
    "${path}/SHA512.h"
    "${path}/hw_aes.h"
    "${path}/rng.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/nvm_info/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/nvm_info.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/uart/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/uart.h"
    "${path}/uart_memory.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/spi_slave/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/spi_slave.h"
    "${path}/spi_slave_memory.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/gpio/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/gpio.h"
    "${path}/gpio_memory.h"
    "${path}/gpio_module.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/dma/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/dma.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/memory_manager/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/memory_manager.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/third_party/QPC/ports/arcv2em")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/bsp.h"
    "${path}/qf_port.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/third_party/QPC/source")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/qf_pkg.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/third_party/QPC/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/qassert.h"
    "${path}/qep.h"
    "${path}/qequeue.h"
    "${path}/qf.h"
    "${path}/qk.h"
    "${path}/qmpool.h"
    "${path}/qpset.h"
    "${path}/qs_dummy.h"
    "${path}/qxk.h"
    "${path}/stdint_c.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/transport/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/transport.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/sleep_manager/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/sleep_manager.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/rc_calib/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/rc_calib.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/sleep_timer/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/sleep_timer.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/prot_timer/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/prot_timer.h"
    "${path}/prot_timer_memory.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/radio/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/radio.h"
    "${path}/radio_config.h"
    "${path}/radio_memory.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/pml/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/pml.h"
    "${path}/pml_memory.h"
    "${path}/pml_power.h"
    "${path}/pml_volt_monitor.h"
    "${path}/pml_volt_monitor_memory.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/nvm_entry/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/nvm_entry.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/firmware_header/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/firmware_header.h"
    "${path}/firmware_header_symbol.h"
    "${path}/header_access.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/em_system/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/em_system.h"
    "${path}/em_system_cmd_nvm.h"
    "${path}/em_system_cmd_rom.h"
    "${path}/em_transport_manager.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/em_hw_api/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/boot.h"
    "${path}/common.h"
    "${path}/config.h"
    "${path}/driver.h"
    "${path}/fifo.h"
    "${path}/irq.h"
    "${path}/irq_user_isr.h"
    "${path}/mem_utils.h"
    "${path}/system.h"
    "${path}/system_memory.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/em_core/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/stack_monitor.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/em_system_stack/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/em_system_stack.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/common/9305/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/apexextensions.h"
    "${path}/arc.h"
    "${path}/aux_registers.h"
    "${path}/emm_arc_reg.h"
    "${path}/errno.h"
    "${path}/hw_versions.h"
    "${path}/interrupts.h"
    "${path}/jli.blank.h"
    "${path}/jli.h"
    "${path}/memory_map.h"
    "${path}/t9305_periph.h"
    "${path}/types.h"
)

SET(path "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/common/includes")
FILE(INSTALL DESTINATION "${dst}/includes" TYPE FILE FILES
    "${path}/macros.h"
    "${path}/t9305_adc.h"
    "${path}/t9305_aoad_matrix.h"
    "${path}/t9305_cu.h"
    "${path}/t9305_gpio.h"
    "${path}/t9305_i2c_master.h"
    "${path}/t9305_i2s.h"
    "${path}/t9305_irq.h"
    "${path}/t9305_nvm.h"
    "${path}/t9305_otp.h"
    "${path}/t9305_pck_proc.h"
    "${path}/t9305_pml.h"
    "${path}/t9305_prot_tim.h"
    "${path}/t9305_qdec.h"
    "${path}/t9305_radio.h"
    "${path}/t9305_rc_calib.h"
    "${path}/t9305_rf_ctrl.h"
    "${path}/t9305_rng.h"
    "${path}/t9305_spi_master.h"
    "${path}/t9305_spi_slave.h"
    "${path}/t9305_system.h"
    "${path}/t9305_uart.h"
    "${path}/t9305_uni_tim.h"
    "${path}/t9305_usb.h"
)
