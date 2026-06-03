echo deleting folders

@RD /S /q ".\Build"

@RD /S /q ".\Binaries"

@RD /S /q ".\DerivedDataCache"

@RD /S /q ".\Intermediate"

@RD /S /q ".\Saved"

@RD /S /q ".\.vs"

@RD /S /q ".\Backup"

echo deleting files

@DEL /Q "*.sln"

@DEL /Q "UpdateLog.html"

echo deleted