set shell = WScript.CreateObject("WScript.Shell")
set fso = CreateObject("Scripting.FileSystemObject")

NO_CACHE = false
UEFI_EXT  = "x64"
QEMU_ARCH = "x86_64"
FW_BASE   = "OVMF"
BIOS_OPT  = "-pflash"

FW_ARCH = UCase(UEFI_EXT)
FW_DIR  = "https://efi.akeo.ie/" & FW_BASE & "/"
FW_ZIP  = FW_BASE & "-" & FW_ARCH & ".zip"
FW_FILE = FW_BASE & "_" & FW_ARCH & ".fd"
FW_URL  = FW_DIR & FW_ZIP

Sub DownloadFtp(Server, Path)
  Set file = fso.CreateTextFile("ftp.txt", True)
  Call file.Write("open " & Server & vbCrLf &_
    "anonymous" & vbCrLf & "user" & vbCrLf & "bin" & vbCrLf &_
    "get " & Path & vbCrLf & "bye" & vbCrLf)
  Call file.Close()
  Call shell.Run("%comspec% /c ftp -s:ftp.txt > NUL", 0, True)
  Call fso.DeleteFile("ftp.txt")
End Sub

Sub DownloadHttp(Url, File)
  Const BINARY = 1
  Const OVERWRITE = 2
  Set xHttp = createobject("Microsoft.XMLHTTP")
  Set bStrm = createobject("Adodb.Stream")
  Call xHttp.Open("GET", Url, False)
  If NO_CACHE = True Then
    Call xHttp.SetRequestHeader("If-None-Match", "some-random-string")
    Call xHttp.SetRequestHeader("Cache-Control", "no-cache,max-age=0")
    Call xHttp.SetRequestHeader("Pragma", "no-cache")
  End If
  Call xHttp.Send()
  If Not xHttp.Status = 200 Then
    Call WScript.Echo("Unable to access file - Error " & xHttp.Status)
    Call WScript.Quit(1)
  End If
  With bStrm
    .type = BINARY
    .open
    .write xHttp.responseBody
    .savetofile File, OVERWRITE
  End With
End Sub

Sub Unzip(Archive, File)
  Const NOCONFIRMATION = &H10&
  Const NOERRORUI = &H400&
  Const SIMPLEPROGRESS = &H100&
  unzipFlags = NOCONFIRMATION + NOERRORUI + SIMPLEPROGRESS
  Set objShell = CreateObject("Shell.Application")
  Set objSource = objShell.NameSpace(fso.GetAbsolutePathName(Archive)).Items()
  Set objTarget = objShell.NameSpace(fso.GetAbsolutePathName("."))
  ' Only extract the file we are interested in
  For i = 0 To objSource.Count - 1
    If objSource.Item(i).Name = File Then
      Call objTarget.CopyHere(objSource.Item(i), unzipFlags)
    End If
  Next
End Sub

If Not fso.FileExists(FW_FILE) Then
  Call WScript.Echo("The UEFI firmware file, needed for QEMU, " &_
    "will be downloaded from: " & FW_URL & vbCrLf & vbCrLf &_
    "Note: Unless you delete the file, this should only happen once.")
  Call DownloadHttp(FW_URL, FW_ZIP)
End If
If Not fso.FileExists(FW_ZIP) And Not fso.FileExists(FW_FILE) Then
  Call WScript.Echo("There was a problem downloading the QEMU UEFI firmware.")
  Call WScript.Quit(1)
End If
If fso.FileExists(FW_ZIP) Then
  Call Unzip(FW_ZIP, FW_BASE & ".fd")
  Call fso.MoveFile(FW_BASE & ".fd", FW_FILE)
  Call fso.DeleteFile(FW_ZIP)
End If
If Not fso.FileExists(FW_FILE) Then
  Call WScript.Echo("There was a problem unzipping the QEMU UEFI firmware.")
  Call WScript.Quit(1)
End If

BOOT_NAME  = WScript.Arguments(1)
QEMU_EXE   = "qemu-system-x86_64w"



call shell.Run("cmd /c mkdir ""..\image\efi\boot""", 0, true)
call fso.CopyFile(WScript.Arguments(0), "..\image\efi\boot\" & BOOT_NAME, true)
call shell.Run(QEMU_EXE + " -net none -monitor none -parallel none -bios " & FW_FILE & " -hda fat:rw:..\image")