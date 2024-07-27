
# 设置输出文件
$outputFile = "includes.dot"
# 创建或清空输出文件，并写入初始内容
Set-Content $outputFile "digraph G {"

# 遍历上一级目录及子目录下的所有 .hpp 和 .cpp 文件
Get-ChildItem -Path .. -Recurse -Include *.hpp | ForEach-Object {
    $currentFile = $_.FullName
    # 对于每个文件，查找 #include 指令
    Get-Content $currentFile | ForEach-Object {
        if ($_ -match '#include "(.*)"' -or $_ -match '#include <(.*)>') {
            # 提取被包含的文件名
            $include = $matches[1]
            # 写入 DOT 文件
            $line = "    `"$($currentFile | Split-Path -Leaf)`" -> `"$include`";"
            Add-Content $outputFile $line
        }
    }
}

# 写入文件的结尾
Add-Content $outputFile "}"

# 使用 dot 工具生成图形
& 'dot' -Tpng includes.dot -o includes.png
