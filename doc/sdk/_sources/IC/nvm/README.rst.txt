Overview
--------

This driver is used to write and erase the EM9305 FLASH memory (NVM).

The NVM driver is located in ROM. However, some fixes have been added and are mapped
into NVM since the ROM cannot be modified.

These fixes address an issue with NVM API call from ISR context with priority
higher than NVM priority.

Blocking mechanism
^^^^^^^^^^^^^^^^^^

The NVM write and erase functions are blocking because concurrent access to NVM is forbidden.
When the erase or write operation is running, the CPU is put in halt mode and is woken up
by NVM ISR.

NVM API call in ISR
^^^^^^^^^^^^^^^^^^^

The NVM write and erase functions disable all other ISR during operations and wait on
NVM ISR for completion. In order to work in ISR context, the NVM ISR priority is
increased to maximum priority during operation and reverted back to previous
priority when operation is done.

NVM Driver in NVM and ROM
^^^^^^^^^^^^^^^^^^^^^^^^^

Some NVM functions have been updated with new versions mapped in the NVM.
The NVM driver is split now between ROM and NVM. The functions remaining in ROM
are defined in rom/bin/vX.X/rom.sym file. All the other API functions are located 
in NVM.

.. list-table:: ROM and NVM functions for NVM driver
   :widths: 70 70 70
   :header-rows: 1

   * - Function
     - ROM
     - NVM
   * - NVM_GetLock
     - NO
     - YES
   * - NVM_SetLock
     - NO
     - YES
   * - NVM_EraseFull
     - NO
     - YES
   * - NVM_EraseMain
     - NO
     - YES
   * - NVM_ErasePage
     - NO
     - YES
   * - NVM_ErasePages_PhysAddr
     - NO
     - YES
   * - NVM_Write_PhysAddr
     - NO
     - YES
   * - NVM_Write
     - NO
     - YES
   * - NVM_GetLockPage
     - YES
     - NO
   * - NVM_LockPage
     - YES
     - NO
   * - NVM_SetTimings
     - YES
     - NO
   * - NVM_SetRedundancyPageMapping
     - YES
     - NO
   * - NVM_GetRedundancyPageMapping
     - YES
     - NO
