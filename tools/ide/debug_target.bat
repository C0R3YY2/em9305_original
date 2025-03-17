@echo off

SETLOCAL ENABLEDELAYEDEXPANSION

if "%~1" == "[exe]" (

    set file=

    rem Search the file in the build folder
    for /r build\projects %%a in (*.elf) do (
        if "%%~nxa"=="%2.elf" (
            set file=%%a
        )
    )

    if not "!file!" == "" (

        rem Start the debugger
        mdb -hard -digilent -off=prefer_soft_bp -off=download -on=reset_upon_restart -on=try_2wire -prop=dig_speed=3000000 -chipinit="tools/debug/chipinit.txt" !file!

    ) else (

        echo Error: File %2.elf not found. Be sure you built the target.
    )

) else (

    echo Please select a target before starting the debugger.

)