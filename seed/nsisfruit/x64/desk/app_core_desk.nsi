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
Name "Twitter Automator"

; The default installation directory
InstallDir "$PROGRAMFILES\twitter_automator2"

; The file to write
OutFile "C:\eco\setup\TwitterAutomatorInstaller.exe"


; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\ca2\app-ca2\twitter_automator2" "Install_Dir"

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
Section "twitter_automator2 (required)"

  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\app\appmatter\main"
  
  ; Put file there
  File /r "C:\eco\app\appmatter\main\"
  
  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\app-ca2\appmatter\twitter_automator2"
  
  ; Put file there
  File /r "C:\eco\app-ca2\appmatter\twitter_automator2\"

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\time\x64\cover"
  
  ; Put file there
  File "C:\eco\time\x64\cover\ace.dll"
  File "C:\eco\time\x64\cover\aqua.dll"
  File "C:\eco\time\x64\cover\aura.dll"
  File "C:\eco\time\x64\cover\axis.dll"
  File "C:\eco\time\x64\cover\base.dll"
  File "C:\eco\time\x64\cover\core.dll"
  File "C:\eco\time\x64\cover\draw2d_gdiplus.dll"
  File "C:\eco\time\x64\cover\wndfrm_anthill.dll"
  File "C:\eco\time\x64\cover\axisfreeimage.dll"
  File "C:\eco\time\x64\cover\image_decode_png.dll"
  File "C:\eco\time\x64\cover\image_decode_jpeg.dll"
  File "C:\eco\time\x64\cover\libcrypto-1_1-x64.dll"
  File "C:\eco\time\x64\cover\libssl-1_1-x64.dll"
  File "C:\eco\time\x64\cover\png.dll"
  File "C:\eco\time\x64\cover\jpeg.dll"
  File "C:\eco\time\x64\cover\pcre.dll"
  File "C:\eco\time\x64\cover\sqlite.dll"
  File "C:\eco\time\x64\cover\zlib.dll"
  File "C:\eco\time\x64\cover\bzip2.dll"
  File "C:\eco\time\x64\cover\app_common.dll"
  File "C:\eco\time\x64\cover\app_ca2_twitter_automator.dll"
  File "C:\eco\time\x64\cover\app_ca2_twitter_automator2.dll"
  File "C:\eco\time\x64\cover\app_ca2_twitter_automator2.exe"
  File "C:\eco\time\x64\cover\ace.pdb"
  File "C:\eco\time\x64\cover\aqua.pdb"
  File "C:\eco\time\x64\cover\aura.pdb"
  File "C:\eco\time\x64\cover\axis.pdb"
  File "C:\eco\time\x64\cover\base.pdb"
  File "C:\eco\time\x64\cover\core.pdb"
  File "C:\eco\time\x64\cover\draw2d_gdiplus.pdb"
  File "C:\eco\time\x64\cover\wndfrm_anthill.pdb"
  File "C:\eco\time\x64\cover\axisfreeimage.pdb"
  File "C:\eco\time\x64\cover\image_decode_png.pdb"
  File "C:\eco\time\x64\cover\image_decode_jpeg.pdb"
  File "C:\eco\time\x64\cover\libcrypto-1_1-x64.pdb"
  File "C:\eco\time\x64\cover\libssl-1_1-x64.pdb"
  File "C:\eco\time\x64\cover\png.pdb"
  File "C:\eco\time\x64\cover\jpeg.pdb"
  File "C:\eco\time\x64\cover\pcre.pdb"
  File "C:\eco\time\x64\cover\sqlite.pdb"
  File "C:\eco\time\x64\cover\zlib.pdb"
  File "C:\eco\time\x64\cover\bzip2.pdb"
  File "C:\eco\time\x64\cover\app_common.pdb"
  File "C:\eco\time\x64\cover\app_ca2_twitter_automator.pdb"
  File "C:\eco\time\x64\cover\app_ca2_twitter_automator2.pdb"
  File "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Redist\MSVC\14.10.25008\vcredist_x64.exe"
  
${If} ${RunningX64}
	ReadRegStr $1 HKLM "SOFTWARE\Wow6432Node\Microsoft\VisualStudio\14.0\VC\Runtimes\x64" "Installed"
	StrCmp $1 1 installed_redist
${Else}
	ReadRegStr $1 HKLM "SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64" "Installed"
	StrCmp $1 1 installed_redist
${EndIf}

  ;not installed, so run the installer
  ExecWait  '"$INSTDIR\time\x64\cover\vcredist_x64.exe" /install /passive /norestart'

installed_redist:

  ;we are done
  ExecWait '"$INSTDIR\time\x64\cover\app_ca2_twitter_automator2.exe" : install' $0
  
  ; Write the installation path into the registry
  WriteRegStr HKLM "SOFTWARE\ca2\app-ca2\twitter_automator2" "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_twitter_automator2" "DisplayName" "Twitter Automator"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_twitter_automator2" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_twitter_automator2" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_twitter_automator2" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
  CreateShortCut "$DESKTOP\Twitter Automator.lnk" "$INSTDIR\time\x64\cover\app_ca2_twitter_automator2.exe" ""
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Twitter Automator"
  CreateShortcut "$SMPROGRAMS\Twitter Automator\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortcut "$SMPROGRAMS\Twitter Automator\Twitter Automator.lnk" "$INSTDIR\time\x64\cover\app_ca2_twitter_automator2.exe" "" "$INSTDIR\time\x64\cover\app_ca2_twitter_automator2.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\app_ca2_twitter_automator2"
  DeleteRegKey HKLM "SOFTWARE\ca2\app-ca2\twitter_automator2"

  ; Remove files and uninstaller
  Delete "$INSTDIR\app_ca2_twitter_automator2.nsi"
  Delete "$INSTDIR\uninstall.exe"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\app_ca2_twitter_automator2\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\app_ca2_twitter_automator2"
  RMDir /r "$INSTDIR/time"
  RMDir "$INSTDIR"

SectionEnd
