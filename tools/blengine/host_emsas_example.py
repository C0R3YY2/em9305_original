import time
import os
import logging

from blengine import logger, default_parser, enable_cli_args
from blengine.core.transport import serial_from_config
from blengine.core.config import config, load
from blengine.core import storage
from blengine.hci.engine import HciEngine

import blengine.hci.procedures as procs
from blengine.hci.protocol import ResponseTimeoutException
from blengine.core.packets import Fragment, GenericPacket
from blengine.hci.packets.EM9305commands import register_emsas_cmd
from blengine.hci.packets.events import register_emsas_event

# Initial BLEngine configuration.
args = enable_cli_args()
log = logger.get("EMSAS_SampleApp")
CONF_NAME_DUT = "DUT"

# Register EM System Application Specific EM9305commands
# EMSAS Op Code 0x00 reperesnts the EM Vendor Op Code 0xFFC0
# EMSAS Op Code range is from 0x00 to 0x0F.
register_emsas_cmd("EMSAS_Timer_Control",  0x00,
    description='''EMSAS - timer control command.''',
    parameters=[
        Fragment("Timer_Control", structure="B",
            description='''Timer Control:
0x00: Timer Disabled
0x01: Timer Enabled
0x02-0xFF: Reserved''')
    ])

emsas_timer_event = register_emsas_event("EMSAS_Timer_Event", 0x01,
    description="EMSAS Timer Event",
    parameters=[
        Fragment(name="Timer_Event_Incremental_Value",
                 structure="L",
                 description="Incremental value provided by the timer event"),
    ])
def emsas_example():
    # Instanciate the transport.
    transport = serial_from_config(config[args.device])
    # Instanciate the engine.
    with HciEngine(transport) as dut_engine:
        def on_emsas_timer_event(pkt):
            print("EMSAS_Timer_Event: " + str(pkt.get("Timer_Event_Incremental_Value")))

        # bind EMSAS Event
        dut_engine.bind("on_EMSAS_Timer_Event", on_emsas_timer_event)

        # Enable Timer via EMSAS_Timer_Control command
        dut_engine.send("EMSAS_Timer_Control", Timer_Control=0x01)

        time.sleep(5)

        # Disable Timer via EMSAS_Timer_Control command
        dut_engine.send("EMSAS_Timer_Control", Timer_Control=0x00)

if __name__ == "__main__":
    emsas_example()
