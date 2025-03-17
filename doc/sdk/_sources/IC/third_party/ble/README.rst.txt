Overview
^^^^^^^^

For more information related to the BLE configurations, please look at ``libs\third_party\emb\cmake\configs.cmake``.

.. warning::

  The file ``configs.cmake`` shall not be modified. It is provided for information purpose and corresponds to the associated libraries.
  An application shall always be compiled with the same configuration as the linked EMB libraries.

.. warning::

  This version of the EMB library includes, in addition to the fully-featured stack, a proof of concept version of the stack
  that supports only BLE advertisement using 3kB of PRAM.
  The PoC is just step #1 of the BLE stack improvement activity and it should not directly be used until the final solution is provided.
