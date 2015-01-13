
set LIB_PATH=..\..\LightNote\Projects\Release\LNote.dll

md .\Sample

copy %LIB_PATH% .\ /y
copy %LIB_PATH% .\Sample\ /y

