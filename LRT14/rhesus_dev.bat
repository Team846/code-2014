if not exist Rhesus mkdir Rhesus
if exist "Rhesus/Toolkit" del "Rhesus/Toolkit" /y
if exist "Rhesus/Messenger" del "Rhesus/Messenger" /y
mklink /J "Rhesus/Toolkit" "../Include/PPC603gnu/Rhesus/Toolkit"
mklink /J "Rhesus/Messenger" "../Include/common/Rhesus/Messenger"