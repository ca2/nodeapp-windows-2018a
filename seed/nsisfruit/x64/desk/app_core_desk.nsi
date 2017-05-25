; example2.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------


;!include "${NSISDIR}\Contrib\Modern UI\System.nsh"
!include x64.nsh
;!define MUI_ICON "C:\eco\app-ca2\appmatter\twitter_automator2\_std\_std\main\icon.ico"

; The name of the installer
Name "desk"

; The default installation directory
InstallDir "$PROGRAMFILES\ca2"

; The file to write
OutFile "C:\ca2\time\nsisfruit\x64\stage\app_core_desk.exe"


; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\ca2\app-core\desk" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "desk (required)"

  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\app\appmatter\main"
  
  ; Put file there
  File /r "C:\ca2\app\appmatter\main\"
  
  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\app-core\appmatter\desk"
  
  ; Put file there
  File /r "C:\eco\app-core\appmatter\desk\"

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\time\x64\stage"
  
  ; Put file there
  File "C:\ca2\time\x64\stage\ace.dll"
  File "C:\ca2\time\x64\stage\aqua.dll"
  File "C:\ca2\time\x64\stage\aura.dll"
  File "C:\ca2\time\x64\stage\axis.dll"
  File "C:\ca2\time\x64\stage\base.dll"
  File "C:\ca2\time\x64\stage\core.dll"
  File "C:\ca2\time\x64\stage\draw2d_gdiplus.dll"
  File "C:\ca2\time\x64\stage\wndfrm_core.dll"
  File "C:\ca2\time\x64\stage\axisfreeimage.dll"
  File "C:\ca2\time\x64\stage\image_decode_png.dll"
  File "C:\ca2\time\x64\stage\image_decode_jpeg.dll"
  File "C:\ca2\time\x64\stage\libcrypto-1_1-x64.dll"
  File "C:\ca2\time\x64\stage\libssl-1_1-x64.dll"
  File "C:\ca2\time\x64\stage\png.dll"
  File "C:\ca2\time\x64\stage\jpeg.dll"
  File "C:\ca2\time\x64\stage\pcre.dll"
  File "C:\ca2\time\x64\stage\sqlite.dll"
  File "C:\ca2\time\x64\stage\zlib.dll"
  File "C:\ca2\time\x64\stage\bzip2.dll"
  File "C:\ca2\time\x64\stage\app_common.dll"
  File "C:\ca2\time\x64\stage\app_core_desk.dll"
  File "C:\ca2\time\x64\stage\app_core_desk.exe"
  File "C:\ca2\time\x64\stage\ace.pdb"
  File "C:\ca2\time\x64\stage\aqua.pdb"
  File "C:\ca2\time\x64\stage\aura.pdb"
  File "C:\ca2\time\x64\stage\axis.pdb"
  File "C:\ca2\time\x64\stage\base.pdb"
  File "C:\ca2\time\x64\stage\core.pdb"
  File "C:\ca2\time\x64\stage\draw2d_gdiplus.pdb"
  File "C:\ca2\time\x64\stage\wndfrm_core.pdb"
  File "C:\ca2\time\x64\stage\axisfreeimage.pdb"
  File "C:\ca2\time\x64\stage\image_decode_png.pdb"
  File "C:\ca2\time\x64\stage\image_decode_jpeg.pdb"
  File "C:\ca2\time\x64\stage\libcrypto-1_1-x64.pdb"
  File "C:\ca2\time\x64\stage\libssl-1_1-x64.pdb"
  File "C:\ca2\time\x64\stage\png.pdb"
  File "C:\ca2\time\x64\stage\jpeg.pdb"
  File "C:\ca2\time\x64\stage\pcre.pdb"
  File "C:\ca2\time\x64\stage\sqlite.pdb"
  File "C:\ca2\time\x64\stage\zlib.pdb"
  File "C:\ca2\time\x64\stage\bzip2.pdb"
  File "C:\ca2\time\x64\stage\app_common.pdb"
  File "C:\ca2\time\x64\stage\app_core_desk.pdb"
  File "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Redist\MSVC\14.10.25008\vcredist_x64.exe"
  
${If} ${RunningX64}
	ReadRegStr $1 HKLM "SOFTWARE\Wow6432Node\Microsoft\VisualStudio\14.0\VC\Runtimes\x64" "Installed"
	StrCmp $1 1 installed_redist
${Else}
	ReadRegStr $1 HKLM "SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64" "Installed"
	StrCmp $1 1 installed_redist
${EndIf}

  ;not installed, so run the installer
  ExecWait  '"$INSTDIR\time\x64\stage\vcredist_x64.exe" /install /passive /norestart'

installed_redist:

  ;we are done
  ExecWait '"$INSTDIR\time\x64\stage\app_core_desk.exe" : install' $0
  
  ; Write the installation path into the registry
  WriteRegStr HKLM "SOFTWARE\ca2\app-core\desk" "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_core_desk" "DisplayName" "Twitter Automator"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_core_desk" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_core_desk" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_core_desk" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
  ; CreateShortCut "$DESKTOP\Twitter Automator.lnk" "$INSTDIR\time\x64\cover\app_core_desk.exe" ""
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\desk"
  CreateShortcut "$SMPROGRAMS\desk\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortcut "$SMPROGRAMS\desk\desk.lnk" "$INSTDIR\time\x64\cover\app_core_desk.exe" "" "$INSTDIR\time\x64\cover\app_core_desk.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_core_desk"
  DeleteRegKey HKLM "SOFTWARE\ca2\app-core\desk"

  ; Remove files and uninstaller
  Delete "$INSTDIR\app_core_desk.nsi"
  Delete "$INSTDIR\uninstall.exe"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\app_core_desk\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\app_core_desk"
  RMDir /r "$INSTDIR/time"
  RMDir "$INSTDIR"

SectionEnd
