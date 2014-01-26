if not exist Rhesus mkdir Rhesus
if exist "Rhesus/Toolkit" rd "Rhesus/Toolkit"
if exist "Rhesus/Messenger" rd "Rhesus/Messenger"
mklink /J "Rhesus/Toolkit" "../Include/PPC603gnu/Rhesus/Toolkit"
mklink /J "Rhesus/Messenger" "../Include/common/Rhesus/Messenger"