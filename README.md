NTOSKRNL Emu\_Extender
===========================
Library of missing functions for Windows XP/2003/Vista/7 NTOSKRNL.EXE

Project is intended to help in porting drivers from Windows 7/8/8.1/10 for work with Windows XP/2003/Vista/7

**How-To:**
1) Compile sources to make **ntoskrn8.sys**
2) Make corrections to target xxx.sys so that it loads ntoskrn8.sys instead of the original ntoskrnl.exe
3) Place ntoskrn8.sys to **X:/Windows/system32/drivers/** folder


**Compiling:**
1) Install Windows 7 DDK v7.1.0 (download from Microsoft site)

1a) Original DDK header files has mistake with definintion of MmAllocateContiguousMemorySpecifyCacheNode, apply fix:
       **\WinDDK\Win7\inc\ddk\ntddk.h**:
       **\WinDDK\Win7\inc\ddk\wdm.h**:

       #if (NTDDI_VERSION >= NTDDI_WIN2K)
       typedef ULONG NODE_REQUIREMENT;
to:

       #if (NTDDI_VERSION >= NTDDI_VISTA)
       typedef ULONG NODE_REQUIREMENT;


2) Download project files to any local folder 

       git clone https://github.com/MovAX0xDEAD/NTOSKRNL_Emu

3) Select Target OS
Keep in mind that depending on the target operating system the way of exporting the functions changes,
if a function already exists in the kernel it will be simply redirected without injecting emulation code

4) Run shell Start Menu\Programs\Windows Driver Kits\Win7 7600.16385.1\Build Environments\Windows XXX\YYY Free Build Environment
**XXX** - target OS, **YYY** - target CPU, 

5) In shell change current directory to local project folder

6) Use shell command BLD to compile project

7) Compiled ntoskrn8.sys will be in **ntoskrn8/objfre\_XXX\_x86/YYY** folder


STORPORT Windows 7 Emu\_Extender
===========================
This is Library of missing functions for Windows 7 STORPORT.SYS to emulate Windows 8 STORPORT.SYS

**How-To:**
1) Compile ntoskrnl Emu\_Extender
2) Place storpor8.sys to **X:/Windows/system32/drivers/** folder
3) Make corrections to target xxx.sys so that it loads **storpor8.sys** instead of the original **storport.sys**


Ported drivers:
===========================


**WDF 1.11 for Windows XP/2003 x32**
---------------------------
Last version for Windows XP/2003 is 1.9, but possible to backport 1.11 version:
1) Get files from Windows 7 Updates:

       WDF01000.SYS	v 1.11.9200.20755
       WdfLdr.sys	v 1.11.9200.16384

2) In WDF01000.SYS replace string "**ntoskrnl.exe**" to "**ntoskrn8.sys**" in import section,
now WDF01000.SYS will import all kernel functions only from Emu\_Extender
3) Recalc checksum

If need coexist with original WDF1.9 drivers:

4) Rename WDF01000.SYS->WDF01\_W8.SYS, WdfLdr.sys->WdfLdr8.sys
5) In WDF01\_W8.SYS replace string "**WdfLdr.sys**" to "**WdfLdr8.sys**" in import section
6) In WdfLdr8.sys replace unicode string "**\Registry\Machine\System\CurrentControlSet\Services\Wdf%02d000** to **\Registry\Machine\System\CurrentControlSet\Services\Wdf%02d_w8**"
7) In WdfLdr8.sys replace hex pattern **F6 78 1B F6** to **F6 EB 1B F6**
8) In ported driver xxx.sys replace string "**WdfLdr.sys**" to "**WdfLdr8.sys**" in import section
9) In .INF of ported driver add creating new service:

       AddService=WDF01_W8,,  WDF.AddService
       ....
       [WDF.AddService]
       DisplayName    = "Windows Driver Framework v1.11 for XP/2003"
       ServiceType    = 1                  ; SERVICE_KERNEL_DRIVER
       StartType      = 3                  ; SERVICE_DEMAND_START
       ErrorControl   = 1                  ; SERVICE_ERROR_NORMAL
       ServiceBinary  = %12%\WDF01_W8.SYS
       LoadOrderGroup = Base


Windows 7's Storport.sys for Windows XP x32
---------------------------
Storport was released started from Windows 2003, but possible to backport Windows 7 version:
1) Get files from Windows 7 Updates (KB3125574):

       storport.sys	v 6.1.7600.23403

2) In storport.sys replace string "**ntoskrnl.exe**" to "**ntoskrn8.sys**" in import section,
now storport.sys will import all kernel functions only from Emu_Extender

3) Storport uses MSI interrupts, need to force use only legacy/compatible interrupts

replace hex pattern **8B 8E 3C 01 00** to **B9 00 00 00 90**

4) Recalc checksum


Windows 7's NVMe driver for Windows XP x32
---------------------------
1) Get files from Windows 7 Updates (KB3125574):

       stornvme.sys	v 6.1.7600.23403

2) MS Win7 NVMe driver require Win7 Storport.sys, use backported one



Windows 8's USB3 driver for Windows XP x32
---------------------------
1) Get required files from Windows 8:

       ucx01000.sys
       usbhub3.sys
       usbxhci.sys
       wpprecorder.sys
       usbd.sys

from Vista Beta/Longhorn 5456.5:

       ksecdd.sys

2) In files ucx01000.sys, usbhub3.sys, usbxhci.sys, wpprecorder.sys, usbd.sys change **security_cookie** to random value
3) In files ucx01000.sys, usbhub3.sys, usbxhci.sys, ksecdd.sys replace string name "**ntoskrnl.exe**" to "**ntoskrn8.sys**" in import section,
now \*.sys will import kernel functions only from Emu\_Extender
4) Rename ksecdd.sys->ksecd8.sys, usbd.sys->usbd\_w8.sys
5) In usbhub3.sys replace string name "**ksecdd.sys**" to "**ksecd8.sys**" in import section
6) In usbhub3.sys replace string name "**usbd.sys**" to "**usbd\_w8.sys**" in import section,
7) recalc checksum


Windows 8's STORAHCI driver for Windows XP x32
---------------------------
STORAHCI driver requires storport.sys from Windows 8, but possible to use storport.sys from Windows 7

storport.sys from Windows 7 more compatible with Windows Xp/2003 because it still call required PoStartNextPowerIrp
when processing power IRPs. MS removed calls to PoStartNextPowerIrp in Windows 8's storport.sys, without this call
Windows XP/2003 kernel cannot finish current power IRP and start next IRP, also it generate BSOD (0x0000009F)

Take attention:
Win8's STORAHCI + Win7's STORPORT have **significal performance drop and high CPU usage**, there is no fix yet

1) Get files from Windows 8 ():

       storahci.sys	v 6.2.9200.16384

2) In storahci.sys replace string "**storport.sys**" to "**storpor8.sys**" in import section,
now storahci.sys will import all storport functions only from storport Emu\_Extender

3) storahci.sys was compiled with Windows 8 DDK's storport.h and writes to new fields of \_PORT\_CONFIGURATION\_INFORMATION,
these fields not exist in Windows 7's storport.sys

Need to skip these writes to avoid damaging structures in memory:
replace hex pattern **83 A6 C8 00 00 00 00** to **90 90 90 90 90 90 90**
replace hex pattern **83 8E CC 00 00 00 03** to **90 90 90 90 90 90 90**

If you compile storahci from sources (from Windows 8 DDK Samples), comment two lines:

       ConfigInfo->BusResetHoldTime = 0;
       ConfigInfo->FeatureSupport |= STOR_ADAPTER_FEATURE_STOP_UNIT_DURING_POWER_DOWN;

4) In storahci.sys change **security_cookie** to random value

5) Recalc checksum


Windows 7's MSAHCI driver for Windows XP x32
---------------------------
1) Get files from Windows 7 Updates(KB3125574):

       atapi.sys	v 6.1.7600.23403
       ataport.sys	v 6.1.7600.23403
       msahci.sys	v 6.1.7600.23403
       pciidex.sys	v 6.1.7600.23403

2) In ataport.sys, pciidex.sys replace string "**ntoskrnl.exe**" to "**ntoskrn8.sys**" in import section,
now these *.sys will import all kernel functions only from Emu\_Extender

3) pciidex.sys uses MS Internal/Undocumented HalDispatchTable way to call functions from Kernel/HAL,

For Windows XP/2003 x32 need to use compatible variant:
replace hex pattern **FF 50 3C** to **FF 50 40**

same in asm code:

       mov     eax, ds:HalDispatchTable
       ...
       call    dword ptr [eax+3Ch] => call    dword ptr [eax+40h]

4) Recalc checksum

5) MSHDC.INF from Windows 7 may conflict with original mshdc.inf from Windows XP/2003
For example msahci enumerates IDE/SATA channels as Internal\_IDE\_Channel and compatible ID as \*PNP0600.
Original mshdc.inf for \*PNP0600 will install wrong **"Standard IDE/ESDI Hard Disk Controller"** driver



Intel RSTe (Enterprise/Premium) AHCI/RAID driver 4.7.0.1098 for Windows XP/2003
---------------------------

1) In file iaStorA.sys replace string "**ntoskrnl.exe**" to "**ntoskrn8.sys**" in import section,
now these *.sys will import kernel functions only from storport Emu\_Extender

2) Recalc checksum


Intel RST AHCI/RAID drivers (any version, last compatible is 15.9.8.1050) for Windows XP/2003
---------------------------

1) In file iaStorA.sys or iaStorAC.sys replace string "**ntoskrnl.exe**" to "**ntoskrn8.sys**" in import section (do not change second string "**ntoskrnl.exe**")
now these *.sys will import kernel functions only from Emu\_Extender

3) Recalc checksum