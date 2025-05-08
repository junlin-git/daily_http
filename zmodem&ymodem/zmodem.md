# ZMODEM 协议格式详解

ZMODEM 是一种复杂的文件传输协议，涉及多个帧类型和命令。下面是对 ZMODEM 协议格式的详细解析，包括帧结构、命令帧、数据帧等。

## ZMODEM 协议概述

ZMODEM 是一种双向、可靠、流控制的文件传输协议，支持错误检测和纠正、断点续传、压缩等功能。它使用 ASCII 控制字符进行帧同步，并定义了一系列命令帧来协调文件传输过程。

## 帧结构

ZMODEM 协议中的帧分为以下几种主要类型：

1. **标志帧 (Header Frame)**：
   - 用于标识帧的开始。
   - 包含帧类型和长度信息。

2. **数据帧 (Data Frame)**：
   - 包含实际的数据内容。
   - 数据帧可能包含文件数据或命令信息。

3. **超时帧 (Timeout Frame)**：
   - 用于处理超时情况。
   - 不常用，通常由接收方发送以指示等待超时。

4. **结束帧 (End Frame)**：
   - 标识帧的结束。
   - 确保接收方正确识别帧边界。

## 帧格式

每个 ZMODEM 帧的基本结构如下：

```
<FLAG><TYPE><LENGTH><HEADER><DATA><CRC>
```


- **FLAG**：帧标志，通常是 `0x16`（DLE），表示帧的开始。
- **TYPE**：帧类型，占用一个字节，指示帧的内容和功能。
- **LENGTH**：帧长度，占用两个字节，表示后续数据的长度。
- **HEADER**：头部信息，根据帧类型不同而变化。
- **DATA**：实际数据内容。
- **CRC**：循环冗余校验码，用于错误检测。

### 具体字段说明

1. **FLAG (Frame Flag)**：
   - 值为 `0x16`（DLE）。
   - 表示帧的开始。

2. **TYPE (Frame Type)**：
   - 值范围从 `0x00` 到 `0xFF`。
   - 常见的帧类型包括：
     - `ZRQINIT` (`0x00`)：请求初始化。
     - `ZRINIT` (`0x01`)：初始化响应。
     - `ZSINIT` (`0x02`)：发送初始化。
     - `ZFERR` (`0x03`)：文件错误。
     - `ZFIN` (`0x04`)：文件传输结束。
     - `ZRPOS` (`0x05`)：请求位置。
     - `ZACK` (`0x06`)：确认。
     - `NAK` (`0x15`)：否定应答。
     - `CAN` (`0x18`)：取消传输。
     - `ABORT` (`0x1A`)：中止传输。
     - `ZCOMPL` (`0x0C`)：完成传输。
     - `ZDATA` (`0x07`)：数据帧。
     - `ZEOF` (`0x08`)：文件结束。
     - `ZSKIP` (`0x09`)：跳过文件。
     - `ZFREECNT` (`0x0A`)：自由计数。
     - `ZCOMMAND` (`0x0B`)：命令帧。
     - `ZFILE` (`0x0F`)：文件头。
     - `ZOPTION` (`0x10`)：选项设置。

3. **LENGTH (Frame Length)**：
   - 两个字节，大端序（MSB 高位在前）。
   - 表示 HEADER 和 DATA 字段的总长度。

4. **HEADER (Header Information)**：
   - 根据 TYPE 的不同而变化。
   - 包含与帧类型相关的附加信息。

5. **DATA (Data Field)**：
   - 实际数据内容。
   - 对于数据帧（`ZDATA`），包含文件的实际数据块。
   - 对于其他帧类型，可能包含命令参数或其他相关信息。

6. **CRC (Cyclic Redundancy Check)**：
   - 四个字节，大端序（MSB 高位在前）。
   - 使用 CRC-32 算法计算 HEADER 和 DATA 字段的校验值。
   - 接收方使用相同的算法计算校验值并与接收到的 CRC 进行比较，以验证数据的完整性。

## 示例帧分析

以下是一个典型的 ZMODEM 初始化帧（`ZRINIT`）的例子：

```
FLAG: 0x16
TYPE: 0x01 (ZRINIT)
LENGTH: 0x000E (14 bytes)
HEADER:
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
DATA: None
CRC: 0xAAAAAAAA (示例 CRC 值)
```



### 解析步骤

1. **读取 FLAG**：
   - 接收到 `0x16`，表示帧的开始。

2. **读取 TYPE**：
   - 接收到 `0x01`，表示这是一个 `ZRINIT` 帧。

3. **读取 LENGTH**：
   - 接收到 `0x000E`，表示 HEADER 和 DATA 字段的总长度为 14 字节。

4. **读取 HEADER**：
   - 接收到 14 字节的 HEADER 数据。
   - 对于 `ZRINIT` 帧，HEADER 通常包含各种配置信息，如最大包大小、窗口大小等。

5. **读取 DATA**：
   - 根据 LENGTH 字段，确定是否有 DATA 字段。
   - 对于 `ZRINIT` 帧，DATA 字段为空。

6. **读取 CRC**：
   - 接收到 4 字节的 CRC 值。
   - 计算 HEADER 和 DATA 字段的 CRC-32 值，并与接收到的 CRC 值进行比较。

## 常见帧类型详解

以下是几种常见 ZMODEM 帧类型的详细解析：

### ZRQINIT (Request Initialization)

- 发送方请求接收方初始化。
- 类型：`0x00`
- HEADER：无特定内容。
- DATA：无。
- 示例：

```
FLAG: 0x16
TYPE: 0x00
LENGTH: 0x0000
HEADER: None
DATA: None
CRC: 0xFFFFFFFF
```

### ZRINIT (Initialization Response)

- 接收方对初始化请求的响应。
- 类型：`0x01`
- HEADER：包含接收方的能力和配置信息。
- DATA：无。
- 示例：

```
FLAG: 0x16
TYPE: 0x01
LENGTH: 0x000E
HEADER:
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
DATA: None
CRC: 0xBBBBBBBB
```

### ZSINIT (Send Initialization)

- 发送方告知接收方自己的能力。
- 类型：`0x02`
- HEADER：包含发送方的能力和配置信息。
- DATA：无。
- 示例：
```
FLAG: 0x16
TYPE: 0x02
LENGTH: 0x000E
HEADER:
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
DATA: None
CRC: 0xCCCCCCCC
```

### ZFILE (File Header)

- 发送方通知接收方即将传输的文件信息。
- 类型：`0x0F`
- HEADER：包含文件名、文件大小、修改时间等信息。
- DATA：无。
- 示例：
```
FLAG: 0x16
TYPE: 0x0F
LENGTH: 0x0020
HEADER:
Filename: example.txt (ASCII 编码)
File size: 0x00000000 0x00001234 (3636 bytes)
Modification time: ...
DATA: None
CRC: 0xDDDDDDDD
```

### ZDATA (Data Frame)

- 发送方传输文件的实际数据块。
- 类型：`0x07`
- HEADER：包含数据块的位置信息。
- DATA：文件数据。
- 示例：

```
FLAG: 0x16
TYPE: 0x07
LENGTH: 0x0010
HEADER:
Data block position: 0x00000000 0x00000000
DATA:
Actual file data...
CRC: 0xEEEEEEEE
```


### ZEOF (End of File)

- 发送方通知接收方当前文件传输结束。
- 类型：`0x08`
- HEADER：包含文件的校验信息。
- DATA：无。
- 示例：

```
FLAG: 0x16
TYPE: 0x08
LENGTH: 0x0008
HEADER:
File CRC: 0x00000000 0x12345678
DATA: None
CRC: 0xFFFFFFFF

```

### ZFIN (File Transfer Finished)

- 发送方通知接收方所有文件传输已完成。
- 类型：`0x04`
- HEADER：无特定内容。
- DATA：无。
- 示例：
```
FLAG: 0x16
TYPE: 0x04
LENGTH: 0x0000
HEADER: None
DATA: None
CRC: 0xFFFFFFFF

```

### ZACK (Acknowledgment)

- 接收方确认已成功接收某个帧。
- 类型：`0x06`
- HEADER：无特定内容。
- DATA：无。
- 示例：
```
FLAG: 0x16
TYPE: 0x06
LENGTH: 0x0000
HEADER: None
DATA: None
CRC: 0xFFFFFFFF

```

### NAK (Negative Acknowledgment)

- 接收方无法确认某个帧。
- 类型：`0x15`
- HEADER：无特定内容。
- DATA：无。
- 示例：
```
FLAG: 0x16
TYPE: 0x15
LENGTH: 0x0000
HEADER: None
DATA: None
CRC: 0xFFFFFFFF

```

### CAN (Cancel Transmission)

- 发送方或接收方取消传输。
- 类型：`0x18`
- HEADER：无特定内容。
- DATA：无。
- 示例：

```
FLAG: 0x16
TYPE: 0x18
LENGTH: 0x0000
HEADER: None
DATA: None
CRC: 0xFFFFFFFF
```

### ABORT (Abort Transmission)

- 发送方或接收方中止传输。
- 类型：`0x1A`
- HEADER：无特定内容。
- DATA：无。
- 示例：
  
```
FLAG: 0x16
TYPE: 0x1A
LENGTH: 0x0000
HEADER: None
DATA: None
CRC: 0xFFFFFFFF

```

## 错误检测与恢复

ZMODEM 使用 CRC-32 校验码来进行错误检测。如果接收方发现 CRC 校验失败，则会发送 NAK 帧请求重新发送该帧。此外，ZMODEM 还支持多种机制来确保数据传输的可靠性，例如：

- **重发机制**：当接收到 NAK 或 CAN 帧时，发送方会重新发送相应的帧。
- **滑动窗口**：允许发送方在未收到确认的情况下发送多个帧，提高传输效率。
- **流量控制**：使用 XON/XOFF 或 RTS/CTS 流控信号来防止数据丢失。