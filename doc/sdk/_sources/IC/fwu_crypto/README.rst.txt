Overview
--------

This library provides the cryptographic toolbox used in the firmware update feature.

The cryptographic toolbox for firmware update includes the following functionalities:

1. AES Decryption API for AES-ECB, AES-CBC and AES-CTR modes.
2. Signature verification API for ECDSA signature scheme.
3. Digest calculation API for signature verification.
4. Key management (key loading) for ECDSA Public Key in User or EM Information Page area.
5. Definition of error codes specific to this cryptographic toolbox.

Refer to the `Firmware Update <../../sections/FirmwareUpdate.html>`_ section of the SDK Documentation for additional information on the firmware update functionality.

Refer to the `Firmware Updater Sample Application <../firmware_updater/index.html>`_ to see how the APIs of this cryptographic toolbox are used in an application.