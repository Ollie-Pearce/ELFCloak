# ELFCloak Obfuscation Framework

## Set-up and compile:

Running `make` should compile:
    -Elfcloak, the obfuscation framework itself

    -dec_and_run, which can be used to decode and execute binaries encoded with Elfcloak

    -target, a simple C program which can be used to test Elfcloak's binary obfuscation.

## Obfuscation options:
Elfcloak provides four different obfuscation techniques detailed below, a list of all obfuscation techniques and options passed to ELFCloak to invoke them can be obtained by running the tool with no arguments.

### Encoding
    -Running `./elfcloak e` will invoke encoding obfuscation:

        -This technique iterates through all instructions in a target binary and applies a fixed value "XOR" to each of them in order to obfuscate the target binary.
        -The `h x` and `hr` options can be added to encoding obfuscation in order to insert a given or random number of heuristic rounds in order to further obfuscate control flow. Heuristic rounds are sequences of pushes, pops and no-ops which are inserted into the generated obfuscated binary in order to hinder binary analysis.

    -Binaries encoded with the `e` option can be executed by:
        -Running ELFCloak again with the `d` option for decoding and executing the resulting binary.
        -Running dec_and_run which will decode the obfuscated binary and then run the associated payload.

### Header Modification
In addition to obfuscation achieved by modifying binary instructions ELFCloak also implements anti-analysis techniques adapted from the ELFREVGO anti-analysis tool designed to hinder the reverse engineering of obfuscated binaries, these include:
    -`en`: Endian obfuscation, modifies the field of an ELF header which specifies Endianness. 
    -`c`: Class obfuscation, modifies the field of an ELF header specifying whether the binary is 32 or 64 bit.
    -`st`: Section Type obfuscation, erases ELF section type information
    -`g`: Global Offset Table obfuscation, replaces a library function called in the ELF with a custom, user-defined function. the provided `target` binary contains a call to the shared `time` function which can be overriden using GOT obfuscation with the following commands:
        -`./elfcloak g`
        -`target`
        -`time`
        -`custom_printf`

ELFCloak is designed to be modular and as such obfuscation options may be combined, with the exception of endian and class obfuscation which cannot be used with encoding obfuscation.