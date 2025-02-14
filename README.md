# ELFCloak Obfuscation Framework

## Set-up and Compile

Running `make` should compile:

- **Elfcloak**, the obfuscation framework itself.
- **dec_and_run**, which can be used to decode and execute binaries encoded with Elfcloak.
- **target**, a simple C program that can be used to test Elfcloak's binary obfuscation.

## Obfuscation Options

Elfcloak provides four different obfuscation techniques detailed below. A list of all obfuscation techniques and options passed to ELFCloak to invoke them can be obtained by running the tool with no arguments.

### Encoding

Running:

```sh
./elfcloak e
```

will invoke encoding obfuscation:

- This technique iterates through all instructions in a target binary and applies a fixed-value XOR operation to each of them to obfuscate the target binary.
- The `h x` and `hr` options can be added to encoding obfuscation to insert a given or random number of heuristic rounds for further obfuscation of control flow. 
  - Heuristic rounds are sequences of `push`, `pop`, and `nop` instructions inserted into the generated obfuscated binary to hinder binary analysis.

Binaries encoded with the `e` option can be executed by:

- Running ELFCloak again with the `d` option for decoding and executing the resulting binary:
  
  ```sh
  ./elfcloak d
  targetobf
  ```

- Running `dec_and_run`, which will decode the obfuscated binary and then execute the associated payload.

### Header Modification

In addition to obfuscation achieved by modifying binary instructions, ELFCloak also implements anti-analysis techniques adapted from the **ELFREVGO** anti-analysis tool, designed to hinder the reverse engineering of obfuscated binaries. These include:

- **`en`**: Endian obfuscation – modifies the field of an ELF header that specifies endianness.
- **`c`**: Class obfuscation – modifies the field of an ELF header specifying whether the binary is 32-bit or 64-bit.
- **`st`**: Section Type obfuscation – erases ELF section type information.
- **`g`**: Global Offset Table (GOT) obfuscation – replaces a library function call in the ELF with a custom, user-defined function.

#### GOT Obfuscation Example

The provided `target` binary contains a call to the shared `time` function, which can be overridden using GOT obfuscation with the following commands:

```sh
./elfcloak g
target
time
custom_printf
```

### Combining Obfuscation Techniques

ELFCloak is designed to be modular, allowing obfuscation options to be combined. However, **endian (`en`) and class (`c`) obfuscation cannot be used with encoding (`e`) obfuscation**.
