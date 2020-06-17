NTOSKRNL Emu_Extender
===========================
Library of missing functions for Windows XP/2003/Vista/7 NTOSKRNL.EXE

Project is intended to help in porting drivers from Windows 7/8/8.1/10 for work with Windows XP/2003/Vista/7

**How-To:**
1) Compile sources to make **ntoskrn8.sys**
2) Make corrections to xxx.sys so that it loads **ntoskrn8.sys** instead of the original **ntoskrnl.exe**
3) Place **ntoskrn8.sys** to **X:/Windows/sytem32/drivers/** folder

**Compiling:**
1) Install Windows 7 DDK v7.1.0 (download from Microsoft site)

2) Copy project files to any local folder 

       git clone https://github.com/MovAX0xDEAD/NTOSKRNL_Emu

3) Select Target OS
Keep in mind that depending on the target operating system the way of exporting the functions changes,
if a function already exists in the kernel it will be simply redirected without injecting emulation code

4) Run shell Start Menu\Programs\Windows Driver Kits\Win7 7600.16385.1\Build Environments\Windows XXX\x86 Free Build Environment
**XXX** - target OS

5) In shell change current directory to local project folder

6) Use shell command BLD to compile project

7) Compiled ntoskrn8.sys will be in **objfre_XXX_x86/i386** folder



Ported drivers:
===========================


**WDF 1.11 for Windows XP/2003 x32**
---------------------------
Last version for Windows XP/2003 is 1.9, but possible to backport 1.11 version:
1) Get files from Windows 7 Updates:

       WDF01000.SYS	v 1.11.9200.20755
       WdfLdr.sys	v 1.11.9200.16384

2) In WDF01000.SYS replace string **ntoskrnl.exe** to **ntoskrn8.sys** in import section,
now WDF01000.SYS will import all kernel functions only from Emu_Extender
4) Recalc checksum

If need coexist with original WDF1.9 drivers:
5)  Rename WDF01000.SYS->WDF01_W8.SYS, WdfLdr.sys->WdfLdr8.sys
6)  In WDF01_W8.SYS replace string **WdfLdr.sys** to **WdfLdr8.sys** in import section
7)  In WdfLdr8.sys replace unicode string **\Registry\Machine\System\CurrentControlSet\Services\Wdf%02d000** to **\Registry\Machine\System\CurrentControlSet\Services\Wdf%02d_w8**
8)  In WdfLdr8.sys replace hex pattern **F6 78 1B F6** to **F6 EB 1B F6**
9)  In ported driver xxx.sys replace string **WdfLdr.sys** to **WdfLdr8.sys** in import section
10) In .INF of ported driver add creating new service:
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
Storport was released only for Windows 2003, but possible to backport Windows 7 version:
1) Get files from Windows 7 Updates (KB3125574):

       storport.sys	v 6.1.7600.23403

2) In storport.sys replace string **ntoskrnl.exe** to **ntoskrn8.sys** in import section,
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
3) In files ucx01000.sys, usbhub3.sys, usbxhci.sys, ksecdd.sys replace string name "ntoskrnl.exe" to "ntoskrn8.sys' in import section,
now *.sys will import kernel functions only from Emu_Extender
4) Rename ksecdd.sys->ksecd8.sys, usbd.sys->usbd_w8.sys
5) In usbhub3.sys replace string name "ksecdd.sys" to "ksecd8.sys' in import section
6) In usbhub3.sys replace string name "usbd.sys" to "usbd_w8.sys' in import section,
7) recalc checksum

