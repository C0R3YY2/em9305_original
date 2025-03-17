# Compiled Metaware libraries repo
This repo contains the compiled version of the MW standard libraries for the EM9305.
The libraries are compiled using a TCF file containing the hardware configuration of the EM9305.

# Link
To link with the previously built libraries, you have to add a LD flag.

```
-Hlib=${LIBS_DIR}/Metaware/em9305_buildlib
```

# Rebuild
If you would like to recompile the libraries, use the MW buildlib util as follow:

```
buildlib buildlib202103 -bd libs/Metaware/bin -tcf=common/9305/tool_config/arc.tcf -Hon=verbose_buildlib
```
