import time
import os

from blengine.core import logger
from blengine.core.config import config, load
from blengine.core.transport import serial_from_config
from blengine.hci.engine import HciEngine
from blengine.hci.procedures.EMSystem import emsystem_prog
from blengine import default_parser, enable_cli_args


# Initial BLEngine configuration.

default_parser.add_argument(
    'files', type=str, help='hex file(s)', nargs='+')
default_parser.add_argument(
    '--spi_disable', help='Disable SPI after programming', action='store_true', default=False)
default_parser.add_argument(
    '--return_bd_address', help=''' 
        Read the bluetooth address of the device before programming.
        The value is stored in the engine parameter local_address''', action='store_true', default=False)
default_parser.add_argument(
    '--progress', help='''Show Progress''', action='store_true', default=True)

args = enable_cli_args()
log = logger.get("CurrentCalibration")


def main():
    # Instanciate the transport.
    transport = serial_from_config(config[args.device])

    # Instanciate the engine.
    with HciEngine(transport) as engine:
        emsystem_prog(engine, files=args.files,  
                  spi_disable=args.spi_disable, return_address=args.return_bd_address, progress=args.progress)


if __name__ == "__main__":
    main()
