set shell = WScript.CreateObject("WScript.Shell")
set fso = CreateObject("Scripting.FileSystemObject")

TARGET_DIR = WScript.Arguments(0)
BOOT_NAME  = WScript.Arguments(1)
QEMU_EXE   = "qemu-system-x86_64w"
BOOT_DIR   = "..\boot\"
IMAGE_DIR   = "..\image\"
OVMFCODE   = "OVMF_CODE-pure-efi.fd"
OVMFVARS   = "OVMF_VARS-pure-efi.fd"

If Not fso.FileExists(IMAGE_DIR & OVMFVARS) Then
  Call fso.CopyFile(BOOT_DIR & OVMFVARS, IMAGE_DIR & FW_FILE)
End If

BOOT_NAME  = WScript.Arguments(1)
QEMU_EXE   = "qemu-system-x86_64w"



call shell.Run("cmd /c mkdir ""..\image\efi\boot""", 0, true)
call fso.CopyFile(TARGET_DIR & "boot.efi", "..\image\efi\boot\" & BOOT_NAME, true)
call fso.CopyFile("zap-ext-vga16.psf", "..\image\zap-ext-vga16.psf", true)
call fso.CopyFile(TARGET_DIR & "kernel.exe", "..\image\kernel.exe", true)
call shell.Run(QEMU_EXE + " -machine q35 -m 1G -cpu max -smp sockets=1,cores=2,threads=2 -serial file:serial.txt -net none -monitor none -parallel none -drive file=" & BOOT_DIR & OVMFCODE & ",if=pflash,index=0,format=raw,readonly=on -drive file=" & IMAGE_DIR & OVMFVARS & ",if=pflash,index=1,format=raw -hda fat:rw:..\image")

'-drive file=..\image\NvVars,if=pflash,format=raw,readonly=off
'call shell.Run(QEMU_EXE + " -m 256m -serial file:serial.txt -vga vmware -net none -monitor none -parallel none -drive file=" & FW_FILE & ",if=pflash,format=raw,readonly=off -hda fat:rw:..\image")