
# 设置要统计的文件扩展名
$extensions = @("*.cpp", "*.hpp")

# 初始化行数计数器
$totalLines = 0

# 遍历每个扩展名
foreach ($extension in $extensions) {
    # 获取上一级目录及子目录下所有匹配的文件
    $files = Get-ChildItem -Path .. -Recurse -Include $extension

    # 遍历文件，统计行数
    foreach ($file in $files) {
        $lineCount = (Get-Content $file.FullName | Measure-Object -Line).Lines
        $totalLines += $lineCount
    }
}

# 设置输出文件
$outputFile = "CountSourceLines.txt"
# 创建或清空输出文件，并输出总行数
Set-Content $outputFile "Total lines: $totalLines"
