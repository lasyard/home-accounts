# Features

这是一个使用 wxWidgets 实现的图形界面应用。

*注意：*写代码前请阅读 `.github/instructions` 目录下的文件并遵循代码规范。

## Auto backup file

保存文件时，将原文件重命名为“原文件名_日期_时间.扩展名”的形式作为备份，以原文件名保存新的内容。

要求：

1. 日期格式为 YYYY-MM-DD, 时间格式为 hhmmss
2. 打开文件后如果没有保存，则不生成备份
3. 备份文件的个数控制在 n 个以内，如果同目录下已经有 n 个及以上的备份文件，把较旧的那些删除

## Import CSV file

导入指定 CSV 文件中的数据，保存在当前文档中。

CSV 文件的格式：

1. 第一行为每一列的标题
2. 分隔符为 `,`

要求：

1. 当前有文档打开时生效
2. 由菜单“edit::import”触发
3. 只能修改 `HaDocument::OnImport` 这个函数和 `gui/import` 目录下的文件

执行的动作：

1. 如果当前文档包含名为 `ImportPanel::IMPORT_SECTION_NAME` 的 Section, 则读取这个 Section, 否则弹出一个文件对话框让用户选择一个 CSV 文件并读取这个文件（用 `ImportDoc::ReadStream` 实现）
2. 添加/切换到 `ImportPanel`, 以表格方式显示读到的内容

如何读文件：

1. 使用 `parse_count` 获取首行的列数
2. 使用 `parse_str` 从第一行中获得每列的标题
3. 构造 `parser` 并从第二行开始用 `parse_line` 解析到文件末尾
