@echo off

SETLOCAL ENABLEDELAYEDEXPANSION

if "%~1" == "[exe]" (

    set file=

    rem Search the file in the build folder
    for /r build\projects %%a in (*.ihex) do (
        if "%%~nxa"=="%2.ihex" (
            set file=%%a
        )
    )

    if not "!file!" == "" (

        rem Program the DVK
        cd tools\blengine\
        python program_nvm_main.py !file! %3

    ) else (

        echo Error: File %2.ihex not found. Be sure you built the target.
    )

) else (

    echo Please select a target before running.

)
