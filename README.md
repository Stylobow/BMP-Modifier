# BMP-Modifier

BMP-Modifier is a C++ command-line tool designed to manipulate 24-bit BMP images and perform steganography by hiding and extracting secret text messages within pixel data.

## Requirements & Build

### Requirements
* Windows (x64)
* Visual Studio 2026 (C++20 support and Windows SDK)

### Build Instructions

Run the generation script located in the `bin/` directory:
```cmd
bin\make.bat
```

## Usage

All commands require the `-` prefix.

```cmd
BMP-Modifier.exe -[command] [arguments...]
```

### Help
Display all available commands and options.
```cmd
BMP-Modifier.exe -help
# Alias:
BMP-Modifier.exe -h
```

### Steganography

#### Hide a message in an image
```cmd
BMP-Modifier.exe -hide <file.bmp> "<message>"
# Alias:
BMP-Modifier.exe -hi <file.bmp> "<message>"
```

#### Extract a hidden message from an image
```cmd
BMP-Modifier.exe -show <file.bmp>
# Alias:
BMP-Modifier.exe -s <file.bmp>
```

### Image Manipulation

#### Clone an image
```cmd
BMP-Modifier.exe -clone <file.bmp>
# Alias:
BMP-Modifier.exe -c <file.bmp>
```

#### Switch colors
```cmd
BMP-Modifier.exe -switchcolor <file.bmp>
# Alias:
BMP-Modifier.exe -sc <file.bmp>
```

#### Resize an image
```cmd
BMP-Modifier.exe -resize <file.bmp> <width> <height>
# Alias:
BMP-Modifier.exe -r <file.bmp> <width> <height>
```
