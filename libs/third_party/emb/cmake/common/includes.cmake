# Common include files.
SET(EMB_COMMON_INCLUDES
    ${COMMON_INCLUDES}

    ${PACKETCRAFT_DIR}/ble-host/include
    ${PACKETCRAFT_DIR}/ble-host/sources/stack/att
    ${PACKETCRAFT_DIR}/ble-host/sources/stack/cfg
    ${PACKETCRAFT_DIR}/ble-host/sources/stack/dm
    ${PACKETCRAFT_DIR}/ble-host/sources/stack/hci
    ${PACKETCRAFT_DIR}/ble-host/sources/stack/l2c
    ${PACKETCRAFT_DIR}/ble-host/sources/stack/smp
    ${PACKETCRAFT_DIR}/ble-host/sources/hci/exactle

    ${PACKETCRAFT_DIR}/ble-profiles/include
    ${PACKETCRAFT_DIR}/ble-profiles/sources/af
    ${PACKETCRAFT_DIR}/ble-profiles/sources/profiles/include
    ${PACKETCRAFT_DIR}/ble-profiles/sources/profiles
    ${PACKETCRAFT_DIR}/ble-profiles/sources/profiles/gatt
    ${PACKETCRAFT_DIR}/ble-profiles/sources/profiles/uribeacon
    ${PACKETCRAFT_DIR}/ble-profiles/sources/services
    ${PACKETCRAFT_DIR}/ble-profiles/sources/profiles/dis
    ${PACKETCRAFT_DIR}/controller/include/ble
    ${PACKETCRAFT_DIR}/controller/include/common
    ${PACKETCRAFT_DIR}/controller/sources/ble/include
    ${PACKETCRAFT_DIR}/controller/sources/ble/lhci

    ${PACKETCRAFT_DIR}/platform/include
    ${PACKETCRAFT_DIR}/platform/targets/emm/include
    ${PACKETCRAFT_DIR}/platform/targets/emm/sources

    ${PACKETCRAFT_DIR}/../emb_bsp/includes
    ${PACKETCRAFT_DIR}/../emb_app_bsp/${TARGET_HW}/include
    ${PACKETCRAFT_DIR}/wsf/include
    ${PACKETCRAFT_DIR}/wsf/include/util
    ${PACKETCRAFT_DIR}/wsf/targets/qpc/${TARGET_HW}/include

    ${EMB_EXTENSIONS_DIR}/ble-host/include
    ${EMB_EXTENSIONS_DIR}/ble-host/sources
    ${EMB_EXTENSIONS_DIR}/controller/include/ble
    ${EMB_EXTENSIONS_DIR}/controller/sources/ble/include
    ${EMB_EXTENSIONS_DIR}/controller/sources/ble/lctr

    ${LIBS_DIR}/third_party/QPC/includes
    ${LIBS_DIR}/third_party/QPC/ports/arcv2em
    ${LIBS_DIR}/third_party/QPC/qpc/include
    ${LIBS_DIR}/third_party/QPC/source

    CACHE INTERNAL "EMB Common Includes"
)


SET(EMB_AUDIO_INCLUDES

    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/profiles/include
    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/profiles
    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/services
    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/profiles/aicps
    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/profiles/aicpc
    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/profiles/bapsk
    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/profiles/baps
    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/profiles/bapc
    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/profiles/bapba
    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/profiles/bapsd
    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/profiles/bapsc
    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/profiles/cap
    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/profiles/csipc
    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/profiles/micpc
    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/profiles/tmap
    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/profiles/vocps
    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/profiles/vocpc
    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/profiles/
    ${PACKETCRAFT_DIR}/ble-audio-profiles/sources/profiles/vcpc
    CACHE INTERNAL "EMB Audio Includes"
)
