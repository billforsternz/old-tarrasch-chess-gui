; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=Tarrasch Chess GUI
AppVerName=Tarrasch Chess GUI V2.03a
AppPublisher=Triple Happy Ltd.
AppPublisherURL=http://www.triplehappy.com
AppSupportURL=http://www.triplehappy.com
AppUpdatesURL=http://www.triplehappy.com
DefaultDirName={pf}\Tarrasch
DefaultGroupName=Tarrasch
LicenseFile=C:\Users\Bill\Documents\T2\install\licence.txt
OutputDir=C:\Users\Bill\Documents\T2\install\Setup
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes
ChangesAssociations=no

[Registry]

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Users\Bill\Documents\T2\install\Tarrasch.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Bill\Documents\T2\install\TarraschToyEngine.exe"; DestDir: "{app}\Engines"; Flags: ignoreversion
Source: "C:\Users\Bill\Documents\T2\install\Rybka v2.3.2a.mp.w32.exe"; DestDir: "{app}\Engines"; Flags: ignoreversion
Source: "C:\Users\Bill\Documents\T2\install\komodo3-32.exe"; DestDir: "{app}\Engines"; Flags: ignoreversion
Source: "C:\Users\Bill\Documents\T2\install\komodo3-64.exe"; DestDir: "{app}\Engines"; Flags: ignoreversion
Source: "C:\Users\Bill\Documents\T2\install\komodo3-64-sse.exe"; DestDir: "{app}\Engines"; Flags: ignoreversion
Source: "C:\Users\Bill\Documents\T2\install\stockfish-6-64.exe"; DestDir: "{app}\Engines"; Flags: ignoreversion
Source: "C:\Users\Bill\Documents\T2\install\stockfish-6-32.exe"; DestDir: "{app}\Engines"; Flags: ignoreversion
Source: "C:\Users\Bill\Documents\T2\install\Houdini_15a_w32.exe"; DestDir: "{app}\Engines"; Flags: ignoreversion
Source: "C:\Users\Bill\Documents\T2\install\Houdini_15a_x64.exe"; DestDir: "{app}\Engines"; Flags: ignoreversion
Source: "C:\Users\Bill\Documents\T2\install\book.pgn"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Bill\Documents\T2\install\book.pgn_compiled"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\Tarrasch"; Filename: "{app}\Tarrasch.exe"
Name: "{userdesktop}\Tarrasch"; Filename: "{app}\Tarrasch"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Tarrasch"; Filename: "{app}\Tarrasch.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\Tarrasch.exe"; Description: "{cm:LaunchProgram,Tarrasch}"; Flags: nowait postinstall skipifsilent

