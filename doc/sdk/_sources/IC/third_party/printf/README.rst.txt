Overview
^^^^^^^^

The printf library is a lightweight printf library that can be used for debugging purpose.

The supported format specifiers are the following:

.. list-table:: Supported Types
   :widths: 20 80
   :header-rows: 1

   * - Type
     - Output
   * - d or i
     - Signed decimal integer
   * - u
     - Unsigned decimal integer
   * - b
     - Unsigned binary
   * - o
     - Unsigned octal
   * - x
     - Unsigned hexadecimal integer (lowercase)
   * - X
     - Unsigned hexadecimal integer (uppercase)
   * - f or F
     - Decimal floating point
   * - e or E
     - Scientific-notation (exponential) floating point
   * - g or G
     - Scientific or decimal floating point
   * - c
     - Single character
   * - s
     - String of characters
   * - p
     - Pointer address
   * - %
     - A % followed by another % character will write a single % 

.. list-table:: Supported Flags
   :widths: 20 80
   :header-rows: 1

   * - Flags
     - Description
   * - -
     - Left-justify within the given field width; Right justification is the default
   * - +
     - Forces to precede the result with a plus or minus sign (+ or -) event for positive numbers.
       By default, only negative numbers are preceded with a - sign.
   * - (space)
     - If no sign is going to be written, a blank space is inserted before the value.
   * - #
     - Used with o, b, x or X specifiers the value is preceded with 0, 0b, 0x or 0X respectively for values 
       different than zero.
       Used with f, F it forces the written output to contain a decimal point even if no more digits follow. By 
       default, if no digits follow, no decimal point is written.
   * - 0
     - Left-pads the number with zeros (0) instead of spaces when padding is specified (see width sub-specifier).

.. list-table:: Supported Width
   :widths: 20 80
   :header-rows: 1

   * - Width
     - Description
   * - (number)
     - Minimum number of characters to be printed. If the value to be printed is shorter than this number, 
       the result is padded with blank spaces. The value is not truncated even if the result is larger.
   * - *
     - The width is not specified in the format string, but as an additional integer value argument 
       preceding the argument that has to be formatted.

.. list-table:: Supported Precision
   :widths: 20 80
   :header-rows: 1

   * - Precision
     - Description
   * - .number
     - For integer specifiers (d, i, o, u, x, X): precision specifies the minimum number of digits to be written. 
       If the value to be written is shorter than this number, the result is padded with leading zeros. The 
       value is not truncated even if the result is longer. A precision of 0 means that no character is written 
       for the value 0.
       For f and F specifiers: this is the number of digits to be printed after the decimal point. By default, 
       this is 6, maximum is 9.
       For s: this is the maximum number of characters to be printed. By default all characters are printed 
       until the ending null character is encountered.
       If the period is specified without an explicit value for precision, 0 is assumed.
   * - .*
     - The precision is not specified in the format string, but as an additional integer value argument 
       preceding the argument that has to be formatted.

.. list-table:: Supported Length
   :widths: 20 60 80
   :header-rows: 1

   * - Length
     - d i
     - u o x X
   * - (none)
     - int
     - unsigned int
   * - hh
     - char
     - unsigned char
   * - h
     - short int
     - unsigned short int
   * - l
     - long int
     - unsigned long int
   * - ll
     - long long int
     - unsigned long long int
   * - j
     - intmax_t
     - uintmax_t
   * - z
     - size_t
     - size_t
   * - t
     - ptrdiff_t
     - ptrdiff_t
