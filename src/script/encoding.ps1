cd ..
$sourceEncoding = [System.Text.Encoding]::Default
$targetEncoding = [System.Text.Encoding]::GetEncoding("windows-1251")

Get-ChildItem -Recurse -Include *.cpp, *.h |
    ForEach-Object {
        $content = Get-Content $_.FullName
        [System.IO.File]::WriteAllLines($_.FullName, $content, $targetEncoding)
    }

cd ui
$sourceEncoding = [System.Text.Encoding]::Default
$targetEncoding = [System.Text.Encoding]::GetEncoding("windows-1251")

Get-ChildItem -Recurse -Include *.cpp, *.h |
    ForEach-Object {
        $content = Get-Content $_.FullName
        [System.IO.File]::WriteAllLines($_.FullName, $content, $targetEncoding)
    }

cd ../script
