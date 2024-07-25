!include "MUI2.nsh"

; Define the name of the installer
Name "Chess Masters"
; Define the output filename for the installer
OutFile "ChessMastersSetup.exe"
; Define the installation directory
InstallDir "$PROGRAMFILES\ChessMasters"
; Request user permissions
RequestExecutionLevel user

; Define installer pages
Page directory
Page instfiles

; Define the sections of the installer
Section "Main Files"
  
  ; Set the installation path
  SetOutPath "$INSTDIR"

  ; Include all files from the source folder
  File "Chess"
  File "Chess Masters.exe"
  File "Makefile"
  File "README.md"
  File "installer.nsi"
  
  ; Create a shortcut on the desktop
  CreateShortCut "$DESKTOP\ChessMasters.lnk" "$INSTDIR\Chess Masters.exe"
SectionEnd

; Define uninstaller section
Section "Uninstall"
  ; Remove installed files
  Delete "$INSTDIR\Chess Masters.exe"
  ; Remove the desktop shortcut
  Delete "$DESKTOP\ChessMasters.lnk"
  ; Remove the installation directory
  RMDir "$INSTDIR"
SectionEnd