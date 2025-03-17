============
EMB Lite API
============

The `EMB Lite` library provides a wrapper library on top of the *em|bleu Bluetooth LE* stack that makes writing Bluetooth LE application easier by reducing the number of primitives required for common BLE operations.

This `EMB Lite` sits on top of the existing Device Manager (DM) API and uses a centralized configuration module to allow for easy customization without requiring major application changes. It is a lightweight replacement for the previously supplied Application framework (AF). Consequently, an application using `EMB Lite` is intended to use the `EMB Lite` APIs in combination with the device manager APIs instead of any application framework APIs.

The `EMB Lite` exclusively supports BLE operations in the Peripheral role with a limited selection of BLE features. Thus, it is not planned to support advanced features like advertising extension, direction finding, periodic advertising with response or isochronous channels.

In case there is a need for advanced operations, then the end user shall fall back using the regular *em|bleu Bluetooth LE* stack.
