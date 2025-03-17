Overview
--------

The firmware header library provides a set of helper functions to manipulate the firmware image headers from 
the end user application.
Most of these functions are getters to read various header fields releasing the end user from knowing the exact
header structure.
These functions are provided for several purposes which can be:

* implement a firmware update process
* monitor content of the NVM for data corruption
* identify content of the NVM 

All these above mentionned functionalities can be done because the firmware image header has been introduced
with relevant information inside.

Scanning the NVM example
------------------------

Here is an example of code that could be used to scan the NVM for any valid firmware images.
In this snippet, once a valid image has been found, it is counted.
Other actions could be done like searching for the customer application by reading the section code field for 
example.

This example is given to illustrate how to use some of the provided functions.

.. code-block:: c

    #include <header_access.h>

    uint32_t address = NVM_START_ADDRESS;
    uint32_t found;
    uint32_t count = 0;

    // Loop until no more valid header is found. We start from the beginning of the NVM (0x300000).
    while ((found = getNvmHdr_FindHdr(address)) != HEADER_ADDRESS_ERROR)
    {
        // A valid header has been found at address ’found’.

        // Check for firmware image integrity.
        if (getEmHdr_VerifyPayload(found) == CRC_PASSED)
        {
            // The checked image is ok!
            // Count this image.
            count++;
        }
        else 
        {
            // Image is corrupted.
        }

        // Go after current firmware image including the header size.
        address += getEmHdr_HdrVersionSize(found) + getEmHdr_FirmwareSize(found);

        // Then search for the next firmware image (next loop iteration).
    }

    // After this loop, the ’count’ variable contains the number of valid firmware images found in NVM.


