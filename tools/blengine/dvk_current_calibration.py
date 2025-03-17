import time
import os
import logging

from blengine.core import logger
from blengine.core.config import config, load
from blengine.core.transport import serial_from_config
from blengine.hci.engine import HciEngine
from blengine.hci.procedures.EMSystem import write_hex, check_hex
from blengine import default_parser, enable_cli_args

# Initial BLEngine configuration.
args = enable_cli_args()
log = logger.get("CurrentCalibration")

def main():
    # Instanciate the transport.
    transport = serial_from_config(config[args.device])

    # Instanciate the engine.
    with HciEngine(transport) as engine:

        # Execute a full calibration...
        log.info("Execute a full calibration...")
        pkt = engine.send("DVK_Execute_Current_Calibration", mode=2)
        assert 0 == pkt.response.get("status")
        log.info("Calibration done:")
        log.info("    - Range 1: Offset={:.3f}, Trim={:.3f}".format(
            pkt.response.get("offset_1"), pkt.response.get("trim_1")))
        log.info("    - Range 2: Offset={:.3f}, Trim={:.3f}".format(
            pkt.response.get("offset_2"), pkt.response.get("trim_2")))
        log.info("    - Range 3: Offset={:.3f}, Trim={:.3f}".format(
            pkt.response.get("offset_3"), pkt.response.get("trim_3")))

        # Connect the DUT power...
        log.info("Connect the DUT power...")
        pkt = engine.send("DVK_Connect_Power_DUT", state=1)
        assert 0 == pkt.response.get("status")

        # Reset the device in Active Mode...
        log.info("Reset the device in Active Mode...")
        pkt = engine.send("DVK_Target_Hard_Reset", mode=0, timeout=5)
        assert pkt.response.get("status") == 0x00


if __name__ == "__main__":
    main()
