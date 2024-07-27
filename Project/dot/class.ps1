
# 设置输出文件
$outputFile = "inheritance.dot"
Set-Content $outputFile "digraph G {"

# 遍历当前目录及子目录下的所有 .h 和 .cpp 文件
Get-ChildItem -Recurse -Include *.hpp, *.cpp | ForEach-Object {
    $currentFile = $_.FullName
    # 对于每个文件，查找类继承关系
    Get-Content $currentFile | ForEach-Object {
        if ($_ -match 'class\s+(\w+)\s*:\s*public\s+(\w+)') {
            $derivedClass = $matches[1]
            $baseClass = $matches[2]
            # 写入 DOT 文件
            $line = "    `"$baseClass`" -> `"$derivedClass`";"
            Add-Content $outputFile $line
        }
    }
}

# 写入文件的结尾
Add-Content $outputFile "}"

# 使用 dot 工具生成图形
& 'dot' -Tpng inheritance.dot -o inheritance.png
