@echo off

mdb -hard -digilent -off=prefer_soft_bp -off=download -on=reset_upon_restart -on=try_2wire -prop=dig_speed=3000000 -chipinit="chipinit.txt" %1
