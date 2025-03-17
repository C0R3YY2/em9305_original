import time
import os
import logging
import math

from blengine.core import logger
from blengine.core.config import config, load
from blengine.core.transport import serial_from_config
from blengine.hci.engine import HciEngine
from blengine.hci.procedures.EMSystem import write_hex, check_hex
from blengine import default_parser, enable_cli_args

# Initial BLEngine configuration.
args = enable_cli_args()
log = logger.get("CurrentMeasurement")

def main():
    # Instanciate the transport.
    transport = serial_from_config(config[args.device])

    # Instanciate the engine.
    with HciEngine(transport) as engine:

        # Select the range 3...
        log.info("Select the range 3...")
        pkt = engine.send("DVK_Set_Current_Range", range=3)
        assert 0 == pkt.response.get("status")

        # Do a average measure on 10 samples...
        log.info("Do a average measure on 10 samples...")
        pkt = engine.send("DVK_Measure_Current", samples=10)
        assert 0 == pkt.response.get("status")
        current = pkt.response.get("current")
        if not math.isnan(current):
            log.info("Result: {:.3f}uA".format(current))
        else:
            log.info("Result: ERROR")

        # Select the range 0...
        log.info("Select the range 0...")
        pkt = engine.send("DVK_Set_Current_Range", range=0)
        assert 0 == pkt.response.get("status")


if __name__ == "__main__":
    main()
