<div align="center">

# 🔌 ActiveXPatchLibrary

![C++](https://img.shields.io/badge/C++-17-00599C?logo=cplusplus&logoColor=white)
![Platform](https://img.shields.io/badge/platform-Windows-0078D6)
![License](https://img.shields.io/badge/license-Apache%202.0-blue.svg)
![Type](https://img.shields.io/badge/type-PoC-orange)

**A Windows DLL injection library for runtime hooking of ActiveX controls** 🎯

</div>

---

## ✨ Features

- 🪝 **Function Hooking** - Intercepts specific ActiveX control methods using Microsoft Detours
- 📝 **Runtime Caption Modification** - Dynamically modifies the caption text of ImhLabel controls
- 🌐 **UDP Command Interface** - Receives commands via UDP on port 1305 to update caption text
- 🖥️ **Console Debug Output** - Provides real-time logging of intercepted function calls
- 🔄 **Clean Hook/Unhook** - Properly restores original functions on DLL detachment

## 📋 Overview

ActiveXPatchLibrary is a Proof of Concept (PoC) dynamic library that intercepts and modifies function calls of ActiveX controls. Specifically, it targets the `ImhLabel` ActiveX control (mhLbl.dll) and provides runtime patching capabilities through a UDP-based communication interface.

## 🏗️ Architecture

The library consists of four main components:

| Component | File | Description |
|-----------|------|-------------|
| 🎯 **Main Hook Engine** | `src/main.cpp` | Manages the DLL lifecycle and function hooking |
| 🌐 **UDP Server** | `inc/UdpServer.h` | Listens for external commands on UDP port 1305 |
| 🛠️ **Utility Functions** | `inc/Utils.h` | Provides string conversion and console setup utilities |
| 📦 **ActiveX Interface** | `inc/ImhLabel.h` | Defines the ImhLabel COM interface with RVA offsets |

## ⚙️ How It Works

1. 💉 The DLL is injected into a target process using the Detours library
2. 🚀 On `DLL_PROCESS_ATTACH`, it:
   - Sets up a debug console 🖥️
   - Hooks the `SetCaption` method of ImhLabel control at RVA offset 0x4c4d 🪝
   - Starts a UDP server on port 1305 🌐
3. 🔄 When `SetCaption` is called on any ImhLabel control:
   - The original caption is intercepted and logged 📊
   - If a new caption has been received via UDP, it replaces the original ✏️
   - Otherwise, the original caption is passed through unchanged ➡️
4. 🧹 On `DLL_PROCESS_DETACH`, all hooks are removed cleanly

## 🔧 Building

### Prerequisites 📦

- 🛠️ **Visual Studio 2022** (Platform Toolset v143)
- 🪟 **Windows SDK 10.0**
- 🔗 **Microsoft Detours** (included as git submodule)

### Build Steps 🚀

**Step 1: Clone the repository with submodules** 📥

```bash
git clone --recursive https://github.com/yourusername/ActiveXPatchLibrary.git
cd ActiveXPatchLibrary
```

**Step 2: If you already cloned without submodules** 🔄

```bash
git submodule update --init --recursive
```

**Step 3: Open in Visual Studio** 📂

Open `ActiveXPatchLibrary/ActiveXPatchLibrary.sln` in Visual Studio

**Step 4: Build the solution** 🏗️

- Configuration: **Release**
- Platform: **Win32**
- Output: **DLL library**

## 🚀 Usage

### Injecting the DLL 💉

Use the Detours `withdll.exe` utility or your preferred DLL injection method:

```bash
withdll.exe /d:ActiveXPatchLibrary.dll target_application.exe
```

### Sending Commands 📤

Send UTF-8 encoded text via UDP to localhost:1305 to change the caption:

**Using netcat** 🐱

```bash
echo "New Caption Text" | nc -u localhost 1305
```

**Using PowerShell** 💠

```powershell
$udpClient = New-Object System.Net.Sockets.UdpClient
$bytes = [System.Text.Encoding]::UTF8.GetBytes("New Caption Text")
$udpClient.Send($bytes, $bytes.Length, "localhost", 1305)
$udpClient.Close()
```

**Using Python** 🐍

```python
import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto("New Caption Text".encode('utf-8'), ('localhost', 1305))
sock.close()
```

## ⚙️ Configuration

Key configuration parameters can be found in `src/main.cpp`:

| Parameter | Default Value | Description |
|-----------|--------------|-------------|
| `BIND_PORT` | 1305 | UDP server listening port |
| `PATCH_TABLE` | See below | Maps function names to RVA offsets |

### Patch Table Structure 📋

```cpp
std::map<std::string, std::pair<uintptr_t, uintptr_t>> PATCH_TABLE = {
    {
        "SetCaption",
        {
            (uintptr_t)((BYTE*)GetModuleHandleW(L"mhLbl.dll") + 0x4c4d),
            (uintptr_t)(&NewSetCaption)
        }
    },
};
```

## 📁 Project Structure

```
ActiveXPatchLibrary/
├── ActiveXPatchLibrary/
│   ├── inc/
│   │   ├── ImhLabel.h        # 📦 ActiveX control interface definition
│   │   ├── UdpServer.h       # 🌐 UDP server implementation
│   │   └── Utils.h           # 🛠️ Utility functions
│   ├── src/
│   │   └── main.cpp          # 🎯 Main DLL entry point and hooking logic
│   ├── ActiveXPatchLibrary.sln
│   └── ActiveXPatchLibrary.vcxproj
├── Detours/                  # 🔗 Microsoft Detours (git submodule)
├── LICENSE                   # 📄 Apache License 2.0
└── README.md                # 📖 This file
```

## 🔍 Technical Details

### Hooked Functions 🪝

| Function | RVA Offset | Description |
|----------|-----------|-------------|
| SetCaption | 0x4c4d | Sets the caption/text of the label control |

### Dependencies 📦

| Dependency | Purpose |
|------------|---------|
| [Microsoft Detours](https://github.com/microsoft/Detours) | Function interception and hooking framework |
| Winsock2 | UDP socket communication |
| Windows COM | BSTR string handling |

### COM Interface Details 🔌

The `ImhLabel` interface is defined with the following key methods:

- **SetCaption** (0x4c4d) - Sets the label text
- **GetCaption** (0x4ed9) - Retrieves the label text
- **SetForeColor** (0x4e65) - Sets the foreground color
- **SetBackColor** (0x4ca7) - Sets the background color

## 🔒 Security Considerations

⚠️ **Important**: This library is designed for defensive security purposes such as:

- ✅ Security research and analysis
- ✅ Debugging and testing ActiveX controls
- ✅ Automated testing frameworks
- ✅ Reverse engineering for compatibility

❌ **Do not use this tool for:**

- Unauthorized modification of software
- Malicious purposes
- Violation of software licenses or terms of service

## 📄 License

Licensed under the Apache License, Version 2.0. See [LICENSE](LICENSE) for full text.

```
Copyright 2024 ActiveXPatchLibrary Contributors

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
```

## 🙏 Acknowledgments

- **Microsoft Detours**: https://github.com/microsoft/Detours
- This project uses Detours for runtime function hooking

## ⚠️ Troubleshooting

<details>
<summary><b>🚫 DLL fails to load</b></summary>

- ✅ Ensure all dependencies (Detours) are properly built
- ✅ Check that the target process architecture matches the DLL (x86)
- ✅ Verify you have proper permissions to inject into the target process
- ✅ Check Windows Defender or antivirus isn't blocking the DLL

</details>

<details>
<summary><b>❌ Function hooks not working</b></summary>

- ✅ Verify mhLbl.dll is loaded in the target process
- ✅ Confirm the RVA offsets match your version of mhLbl.dll
- ✅ Use a tool like PE Explorer or IDA Pro to verify offsets if needed
- ✅ Check the console output for "Patched:" messages

</details>

<details>
<summary><b>🌐 UDP commands not received</b></summary>

- ✅ Check firewall settings allow UDP port 1305
- ✅ Verify the console window shows "UDP Echo Server is running"
- ✅ Ensure you're sending to the correct IP (localhost/127.0.0.1)
- ✅ Try using a network monitoring tool like Wireshark to debug

</details>

<details>
<summary><b>🖥️ Console window not appearing</b></summary>

- ✅ Ensure `Utils::SetupConsole()` is being called in F:/workspace/ActiveXPatchLibrary/ActiveXPatchLibrary/src/main.cpp:126
- ✅ Check if the target process has permission to create console windows
- ✅ Try running the target application as Administrator

</details>

<details>
<summary><b>💥 Application crashes after injection</b></summary>

- ✅ Verify RVA offsets are correct for your mhLbl.dll version
- ✅ Check for conflicts with other hooks or security software
- ✅ Ensure the DLL was built with the correct configuration (Release/Win32)
- ✅ Look for error messages in the console before the crash

</details>

## 🛠️ Development

### Code Style 📝

- 🔤 **Naming**: Use camelCase for functions, PascalCase for classes
- 📏 **Indentation**: 4 spaces
- 💬 **Comments**: Document all hooked functions and RVA offsets

### Adding New Hooks 🪝

1. **Find the RVA offset** using a disassembler (IDA Pro, Ghidra, x64dbg)
2. **Add to ImhLabel.h** with the method signature
3. **Create a new hook function** in main.cpp
4. **Add to PATCH_TABLE** with the offset and hook function
5. **Test thoroughly** to ensure stability

### Debugging Tips 🐛

- 🖥️ Watch the console output for hook confirmation messages
- 📊 Use Process Monitor to track DLL loading and function calls
- 🔍 Attach a debugger (x64dbg/WinDbg) to the target process
- 📝 Enable verbose logging in your hook functions

---

