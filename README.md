# NTOSKRNL Emu\_Extender #
Library of missing functions for Windows XP/2003/Vista/7 NTOSKRNL.EXE

Project is intended to help in porting drivers from Windows 7/8/8.1/10 for work with Windows XP/2003/Vista/7

**How-To:**
1) Compile sources to make **ntoskrn8.sys**
2) Make corrections to target driver XXX.sys so that it loads ntoskrn8.sys instead of the original ntoskrnl.exe
3) If XXX.sys is driver made for Windows 8, change **security_cookie** to random value, security_cookie is constant 0x**4EE640BB**(x32) / 0x**32A2DF2D992B**(x64) inside file, change only first match !
4) Place ntoskrn8.sys to **X:/Windows/system32/drivers/** folder


**Compiling:**

1. Install Windows 7 DDK v7.1.0 (download from Microsoft Web site)

2. Original DDK header files has mistake with definintion of MmAllocateContiguousMemorySpecifyCacheNode, apply fix to two files **\WinDDK\Win7\inc\ddk\ntddk.h** and **\WinDDK\Win7\inc\ddk\wdm.h**:

```
       #if (NTDDI_VERSION >= NTDDI_WIN2K)
       typedef ULONG NODE_REQUIREMENT;
```
to:

```
       #if (NTDDI_VERSION >= NTDDI_VISTA)
       typedef ULONG NODE_REQUIREMENT;
```


3. Download project files to any local folder 

       git clone https://github.com/MovAX0xDEAD/NTOSKRNL_Emu

4. Choose Target OS (XP, 2003, Vista or WIndows 7) for which OS Emu\_Extender will be compiled. Keep in mind that depending on the target operating system the way of exporting the functions changes, if a function already exists in the kernel it will be simply redirected without injecting emulation code

5. Run shell: "*Start Menu\Programs\Windows Driver Kits\Win7 7600.16385.1\Build Environments\Windows XXX\YYY Free Build  Environment*"
   (**XXX** - target OS, **YYY** - target CPU) 

6. In shell change current directory to local project folder

7. In shell type command **BLD** to compile project

8. Compiled ntoskrn8.sys will be in **ntoskrn8/objfre\_XXX\_x86/YYY** folder



# STORPORT Windows 7 Emu\_Extender #
This is Library of missing functions for Windows 7' STORPORT.SYS v6.1.7601.xxxxx to emulate Windows 8' STORPORT.SYS

**How-To:**
1) Compile ntoskrnl Emu\_Extender
2) Place **storpor8.sys** to **X:/Windows/system32/drivers/** folder
3) Make corrections to target xxx.sys so that it loads **storpor8.sys** instead of the original **storport.sys**
4) Place backported **storport.sys** from Windows 7 to **X:/Windows/system32/drivers/** folder


# Ported drivers: #

## Windows 7's WDF 1.11 for Windows XP/2003 ##

Last version for Windows XP/2003 is 1.9, but possible to backport 1.11 version:
1) Get files from Windows 7 Updates (KB3125574):

       WDF01000.SYS   v1.11.9200.20755
       WdfLdr.sys     v1.11.9200.16384

2) In WDF01000.SYS replace string "**ntoskrnl.exe**" to "**ntoskrn8.sys**" in import section

3) Recalc checksum

If need coexist with original WDF1.9 drivers:

4) Rename WDF01000.SYS->WDF01\_W8.SYS, WdfLdr.sys->WdfLdr8.sys
5) In WDF01\_W8.SYS replace string "**WdfLdr.sys**" to "**WdfLdr8.sys**" in import section
6) In WdfLdr8.sys replace unicode string "**\Registry\Machine\System\CurrentControlSet\Services\Wdf%02d000**" to "**\Registry\Machine\System\CurrentControlSet\Services\Wdf%02d_w8**"
7) In WdfLdr8.sys replace hex pattern **F6 78 1B F6** to **F6 EB 1B F6** (x32), **85 FF 78 2A** to **85 FF EB 2A** (x64)
8) In target driver XXX.sys replace string "**WdfLdr.sys**" to "**WdfLdr8.sys**" in import section
9) In .INF of ported driver add creating new service:
```
       AddService=WDF01_W8,,  WDF.AddService
       ....
       [WDF.AddService]
       DisplayName    = "Windows Driver Framework v1.11 for XP/2003"
       ServiceType    = 1                  ; SERVICE_KERNEL_DRIVER
       StartType      = 3                  ; SERVICE_DEMAND_START
       ErrorControl   = 1                  ; SERVICE_ERROR_NORMAL
       ServiceBinary  = %12%\WDF01_W8.SYS
       LoadOrderGroup = Base
```
10) Recalc checksum of all edited *.sys

## Windows 7's Storport.sys for Windows XP/2003 ##

Storport was released since Windows 2003, but possible to backport Windows 7 version:
1) Get files from Windows 7 Updates (KB5022338):

       storport.sys	v6.1.7601.25735

2) In storport.sys replace string "**ntoskrnl.exe**" to "**ntoskrn8.sys**" in import section

3) Storport may use MSI interrupts, need to force use only legacy/non-MSI interrupts:

   x32 - replace hex pattern **8B 8E 3C 01 00 00** to **B9 00 00 00 00 90** (mov ecx, [esi+13Ch] -> mov ecx, 0)

   x64 - replace hex pattern **8B 83 C0 01 00 00** to **B8 00 00 00 00 90** (mov eax, [rbx+1C0h] -> mov eax, 0)

4) Windows 7 storport does not report SMART data when using STORAHCI, need to patch RaUnitScsiMiniportIoctl in storport.sys to enable SMART reporting:

   x32 - replace hex pattern **75 36 8B 45 0C 81 78 10 00 06 1B 00 75 2A** to **90 90 8B 45 0C 81 78 10 00 06 1B 00 90 90**

   x64 - replace hex pattern **75 36 81 7F 10 00 06 1B 00 75 2D** to **90 90 81 7F 10 00 06 1B 00 90 90**

5) Recalc checksum


## Windows 7's NVMe driver for Windows XP ##

1) Get files from Windows 7 Updates (KB3125574):

       stornvme.sys	v6.1.7601.23403

2) MS Windows 7 NVMe driver require storport.sys from Windows 7, use backported one


## Windows 8's USB3 driver for Windows XP ##
USB3 driver from Windows 8 require WDF 1.11, use backported one.

1) Get required files from Windows 8 (x64 KB5040485, x86 KB2984005,KB5028232, x64/x86 RTM ISO):

       ucx01000.sys         v6.2.9200.22453
       usbhub3.sys          v6.2.9200.21180 (x86), v6.2.9200.24610 (x64)
       usbxhci.sys          v6.2.9200.22099
       wpprecorder.sys      v6.2.9200.16384
       usbd.sys             v6.2.9200.20761

from Vista Beta/Longhorn 5456.5:

       ksecdd.sys           v6.0.5456.5

2) In files ucx01000.sys, usbhub3.sys, usbxhci.sys, wpprecorder.sys, usbd.sys change **security_cookie** to random value
3) In files ucx01000.sys, usbhub3.sys, usbxhci.sys, ksecdd.sys replace string name "**ntoskrnl.exe**" to "**ntoskrn8.sys**" in import section
4) For x64 usbd.sys, replace string name "**ntoskrnl.exe**" to "**ntoskrn8.sys**" in import section
5) Rename ksecdd.sys->**ksecd8.sys**, usbd.sys->**usbd\_w8.sys**, wpprecorder.sys->**wpprecor.sys**
6) In files ucx01000.sys, usbhub3.sys, usbxhci.sys replace string name "**WppRecorder.sys**" to "**WppRecor.sys**" in import section
7) In usbhub3.sys replace string name "**ksecdd.sys**" to "**ksecd8.sys**" in import section
8) In usbhub3.sys replace string name "**usbd.sys**" to "**usbd\_w8.sys**" in import section
9) Windows 8 USB3 driver always report USB2 speed on inserted USB3 devices, need to patch usbhub3.sys for proper speed reporting:

   x32 - replace hex pattern **83 7E 54 03 75 09** to **83 7E 54 03 EB 09**

   x64 - replace hex pattern **00 00 00 03 75 0A** to **00 00 00 03 EB 0A**

10) Recalc checksum


## Windows 8's UASP (USB Attached SCSI) driver for Windows XP ##
Needs files from Windows 8 USB3 driver (usbd_w8.sys and wpprecor.sys).

Note that some motherboards may have a deadlock issue when trying to reboot with the UAS device connected.  Windows will turn off but not finish the reboot cycle.  If you Safe Remove the UAS device reboot will finish normally.  There is the option of using the deadlock patched version of the driver that fixes the issue.

1) Get required files from Windows 8 (x64 KB5040485, x86 RTM ISO):

       uaspstor.sys      v6.2.9200.16384 (x86), v6.2.9200.24763 (x64)

2) In uaspstor.sys change **security_cookie** to random value
3) In uaspstor.sys replace string "**ntoskrnl.exe**" to "**ntoskrn8.sys**","**storport.sys**" to "**storpor8.sys**","**usbd.sys**" to "**usbd\_w8.sys**", "**WppRecorder.sys**" to "**WppRecor.sys**"   in import section
4) Recalc checksum

Deadlock patched UASP (CAUTION: Same as with a USB flash drive, rebooting while writing data will corrupt your data)

2) In uaspstor.sys change **security_cookie** to random value
3) In uaspstor.sys replace string "**ntoskrnl.exe**" to "**ntoskrn8.sys**","**storport.sys**" to "**stor_ua8.sys**","**usbd.sys**" to "**usbd\_w8.sys**", "**WppRecorder.sys**" to "**WppRecor.sys**"  in import section
4) Create a copy of "**storport.sys**" and rename to "**stor_uas.sys**", create a copy of "**storpor8.sys**" and rename to "**stor_ua8.sys**"
5) In stor_ua8.sys replace string "**storport.sys**" to "**stor_uas.sys**"
6) In stor_uas.sys patch RaidAdapterSetSystemPowerCompletion to fix deadlock.  Only the UASP driver will use this patched storport.

   x32 - replace hex pattern **75 0B 85 FF** to **EB 0B 85 FF**

   x64 - replace hex pattern **75 0C 41 3B FE** to **EB 0C 41 3B FE**
   
7) In stor_uas.sys replace unicode string "\ D e v i c e \ R a i d P o r t" to something else like "\ D e v i c e \ U a s p P o r t"		

8) Recalc checksum



## Windows 8's STORAHCI driver for Windows XP ##

STORAHCI driver requires storport.sys from Windows 8, but possible to use storport.sys v6.1.7601.xxxxx from Windows 7
Storport.sys from Windows 7 more compatible with Windows XP/2003 because it still call required *PoStartNextPowerIrp* when processing power IRPs. Microsoft removed calls to PoStartNextPowerIrp in Windows 8's storport.sys, without this call Windows XP/2003 kernel cannot finish current power IRP and start next IRP => it generate BSOD (0x0000009F).
Also storport.sys from Windows 7 has compatibility mode to allow old XP/2003 kernels write crashdumps through storport based disk drivers. In storport.sys from Windows 8 compatibility mode was removed, writing crashdumps possible only with new kernels.


Take attention:
Windows 8's STORAHCI + Windows 7's STORPORT may have **significal performance drop and high CPU usage**, there is no fix yet


1) Get files from Windows 8 Updates (x64 KB5040485, x86 KB5028232):

       storahci.sys	v6.2.9200.20652

2) In storahci.sys replace string "**storport.sys**" to "**storpor8.sys**" in import section

3) Storahci.sys was compiled with Windows 8 DDK's storport.h and writes values to new fields of
 *\_PORT\_CONFIGURATION\_INFORMATION* struct, these fields not exist in Windows 7's storport.sys. Need to skip these writes to avoid damaging other structures in memory:

* x32:
1) Replace hex pattern **83 A6 C8 00 00 00 00** to **90 90 90 90 90 90 90** (and dword ptr [esi+0C8h], 0 -> nop)
2) Replace hex pattern **83 8E CC 00 00 00 03** to **90 90 90 90 90 90 90** (or  dword ptr [esi+0CCh], 3 -> nop)
* x64:
1) Replace hex pattern **44 89 B7 D8 00 00 00** to **90 90 90 90 90 90 90** (mov [rdi+0D8h], r14d        -> nop)
2) Replace hex pattern **83 8F DC 00 00 00 03** to **90 90 90 90 90 90 90** (or  dword ptr [rdi+0DCh], 3 -> nop)

If you want compile storahci from sources (from Windows 8 DDK Samples), comment two lines:
```
       ConfigInfo->BusResetHoldTime = 0;
       ...
       ConfigInfo->FeatureSupport |= STOR_ADAPTER_FEATURE_STOP_UNIT_DURING_POWER_DOWN;
```
4) In storahci.sys change **security_cookie** to random value

5) Recalc checksum



## Windows 7's MSAHCI driver for Windows XP ##

1) Get files from Windows 7 Updates (KB5022338):

       atapi.sys	v6.1.7601.26057
       ataport.sys	v6.1.7601.26057
       msahci.sys	v6.1.7601.26057
       pciidex.sys	v6.1.7601.26057

2) In ataport.sys, pciidex.sys replace string "**ntoskrnl.exe**" to "**ntoskrn8.sys**" in import section

3) Pciidex.sys uses MS Internal/Undocumented **HalDispatchTable** way to call functions from Kernel/HAL, for Windows XP/2003 need to use compatible variant:

* x32: replace hex pattern **FF 50 3C** to **FF 50 40**, same in asm code:
```
       mov     eax, ds:HalDispatchTable
       ...
       call    dword ptr [eax+3Ch] => call    dword ptr [eax+40h]
```
* x64: replace hex patterns: 
1) **FF 50 78** to **EB 2A 90**
2) offset +2Ch: **CC CC CC CC CC CC CC CC 48 89 6C** to **FF 90 80 00 00 00 EB CF 48 89 6C**, same in asm code:
```
       mov     rax, cs:HalDispatchTable
       ...
       call    qword ptr [rax+78h] => jmp patch

orig:
       mov     r10d, eax

patch:
      call    qword ptr [eax+80h]
      jmp     orig
```


4) Recalc checksum

5) MSHDC.INF from Windows 7 conflict with original mshdc.inf from Windows XP/2003.
msahci.sys enumerates IDE/SATA channels as "Internal\_IDE\_Channel" and compatible ID is "\*PNP0600". Original mshdc.inf from Windows XP/2003 for compatible Device ID "\*PNP0600" will install wrong **"Standard IDE/ESDI Hard Disk Controller"** driver



## Intel RSTe Enterprise AHCI/RAID driver 4.7.0.1098 for Windows XP/2003 + ##
## Intel RST AHCI/RAID driver (v 11.x-16.x, last tested 16.8.3) for Windows XP/2003 ##

These drivers require storport.sys from Windows 7, use backported one 

1) In file iaStorA.sys/iaStorAC.sys/iaStorAVC.sys replace string "**ntoskrnl.exe**" to "**ntoskrn8.sys**" in import section (do not change second string "**NTOSKRNL.exe**")

2) Recalc checksum


## AMD SATA AHCI Driver 1.2.1.402 for Windows XP ##

This driver requires storport.sys from Windows 7, use backported one 

1) In file amd_xata.sys replace string "**ntoskrnl.exe**" to "**ntoskrn8.sys**" in import section

2) Recalc checksum


## Samsung NVMe Driver 3.3.0.2003 for Windows XP ##

This driver requires storport.sys from Windows 7, use backported one 

1) In file secnvme.sys/secnvmeF.sys replace string "**ntoskrnl.exe**" to "**ntoskrn8.sys**" in import section

2) Recalc checksum
