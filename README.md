# 尔英 ERYING Polestar i7-11800H Hackintosh

**主板**: 极星 POLESTAR i7-11800H DDR4（尔英 ERYING 魔改板）  
**芯片组**: Intel HM570  
**CPU**: Intel Core i7-11800H (Tiger Lake-H)  
**macOS**: Ventura 13.7.8  
**OpenCore**: DBG-104 (2025-03-04)  
**SMBIOS**: iMacPro1,1

---

## 目录说明

| 目录 | 内容 |
|------|------|
| `EFI-OpenCore引导备份/` | 完整的 OpenCore EFI 引导文件（BOOT + OC），包含所有 kext、ACPI、config.plist |
| `音频切换工具/` | 解决 USB 麦克风无法在系统设置中切换的问题 + 开机自启配置 |
| `docs/` | 详细文档 |

## 当前状态

- ✅ **GPU**: AMD 独立显卡，WhateverGreen 驱动
- ✅ **声音输出**: USB 声卡 (C-Media USB PnP Sound Device)，免驱
- ✅ **声音输入**: USB 电容麦克风 (DCMT Technology)，自启切换
- ⚠️ **板载 ALC897 声卡**: 不支持（详见文档）
- ✅ **有线网卡**: RealtekRTL8111
- ✅ **WiFi**: Intel AX201 (itlwm.kext + HeliPort)
- ⚠️ **蓝牙**: Intel AX201，kext 已更新至 v2.4.0，待重启验证
- ✅ **USB**: USBToolBox + UTBDefault 端口映射
- ✅ **电源管理**: CPUFriend + SSDT-PLUG-ALT

## 快速恢复

```bash
# 1. 挂载 ESP 分区
diskutil mount diskXs1

# 2. 复制 EFI 引导文件
cp -r EFI-OpenCore引导备份/* /Volumes/EFI/

# 3. 重启
```

## 音频切换工具

GUI 有 bug 无法切换输入设备时使用：

```bash
./音频切换工具/switch_audio              # 查看状态
./音频切换工具/switch_audio in "设备名"   # 切换输入
./音频切换工具/switch_audio out "设备名"  # 切换输出
```

编译：`cc 音频切换工具/switch_audio.c -o switch_audio -framework AudioToolbox -framework CoreAudio -framework CoreFoundation`
