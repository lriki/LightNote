:==============================================================================
: Build_CommonManual
:==============================================================================

cd DocumentSource
xcopy .\CommonManualSource CommonManual /s /i /y
ruby ExtractLNCodeTag.rb



cd ..\doxygen

doxygen Doxy_CommonManual
xcopy src CommonManual\html /s /i /y
xcopy ..\DocumentSource\CommonManual\img CommonManual\html\img /s /i /y

: ブラウザに表示する
:CommonManual\html\index.html

pause

:==============================================================================
:
:==============================================================================
