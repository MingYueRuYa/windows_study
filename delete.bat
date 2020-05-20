del .\*.pdb /f /s /q /q
del .\*.lastbuildstate /f /s /q /q
del .\*.obj /f /s /q /q
del .\*.idb /f /s /q /q
del .\*.exp /f /s /q /q
del .\*.ipch /f /s /q /q

del .\Debug /f /s /q /q
del .\Release /f /s /q /q


@echo off
setlocal enabledelayedexpansion  

for /r . %%a in (Debug) do (  
  if exist %%a (
  echo "delete" %%a
  rd /s /q "%%a" 
 )
)

for /r . %%a in (Release) do (  
  if exist %%a (
  echo "delete" %%a
  rd /s /q "%%a" 
 )
)

for /r . %%a in (ipch) do (  
  if exist %%a (
  echo "delete" %%a
  rd /s /q "%%a" 
 )
)

for /r . %%a in (*.sdf) do (  
  if exist %%a (
  echo "delete" %%a
  del "%%a" 
 )
)

pause
