# Memo

这个文件只能由人类手动修改，AI 勿动。

以下列出了项目的一些关键信息以供参考，每个二级标题为一项。如果需要的信息在这里找到，就不要去读源码了。如果发现此处的信息与源码不符，请报告“XXXX 信息似与源码不符，暂停动作”并暂停当前动作。如果收到回复中文“继续”或英文 "go on" 则以源码中的信息为准继续动作。

## 项目概况

应用名称：HomeAccounts
关联文档类型：*.ha

## ha 文档结构

ha 文档的内容由多个段 (Section) 组成，每个段有名称 (name) 和内容 (content) 两个属性，对段的操作由以下函数实现：

```cpp
const std::string &HaDocument::GetOrCreateSection(const std::string &name);
void HaDocument::SaveSection(const std::string &name, const std::string &content);
void HaDocument::DeleteSection(const std::string &name);
void HaDocument::SaveOrDeleteSection(const std::string &name, const std::string &content);
```

下表为各个段的说明：

| 段名                  | 内容类型 | 说明                                                                        |
| --------------------- | -------- | --------------------------------------------------------------------------- |
| `data/xxxx`           | HCSV     | xxxx 为数字，代表一个年份，保存此年度的数据，相关代码在目录 `src/gui/data/` |
| `accounts`            | HCSV     | 保存账户信息，相关代码在 `src/gui/accounts/`                                |
| `import`              | CSV      | 导入的 CSV 文件                                                             |
| `conf/import_col_map` | HCSV     | 导入 CSV 的字段映射配置                                                     |

HCSV 类型是一种经过改造的 CSV 格式，由 `CsvDoc` 类实现。

## CSV 字段类型

定义在 `src/csv/column_type.h`.

- CT_STR
- CT_INT
- CT_BOOL
- CT_MONEY
- CT_DATE
- CT_TIME
- CT_IGNORE

## data 字段

| 编号 | 字段名称    | 类型     | 含义         |
| ---: | ----------- | -------- | ------------ |
|    0 | DATE        | CT_DATE  | 交易日期     |
|    1 | TIME        | CT_TIME  | 交易时间     |
|    2 | AMOUNT      | CT_MONEY | 金额         |
|    3 | ACCOUNT     | CT_INT   | 账户 ID      |
|    4 | DESC        | CT_STR   | 说明         |
|    5 | REAL_AMOUNT | CT_MONEY | 真实金额     |
|    6 | REAL_DESC   | CT_STR   | 真实描述     |
|    7 | MEMO        | CT_STR   | 备注         |
|    8 | AUTO_SET    | CT_BOOL  | 是否自动处理 |

`comment_cols = 1`

## accounts 字段

| 编号 | 字段名称  | 类型     | 含义                                                   |
| ---: | --------- | -------- | ------------------------------------------------------ |
|    0 | TYPE      | CT_INT   | 类型（0 - 无效，1 - 借记卡，2 - 信用卡，3 - 支付账户） |
|    1 | ID        | CT_INT   | 账户 ID                                                |
|    2 | NAME      | CT_STR   | 名称                                                   |
|    3 | BANK      | CT_STR   | 开户行                                                 |
|    4 | OPEN_DATE | CT_STR   | 开户日                                                 |
|    5 | INITIAL   | CT_MONEY | 初始金额                                               |
|    6 | MEMO      | CT_STR   | 备注                                                   |

`comment_cols = 1`

## conf/import_col_map 字段

| 编号 | 字段名称 | 类型   | 含义              |
| ---: | -------- | ------ | ----------------- |
|    0 | FIELD    | CT_INT | data 字段编号     |
|    1 | TITLES   | CT_STR | 导入 CSV 的列标题 |

`comment_cols = 0`

## 源码结构

| 目录       | 编程语言 | 基础框架          | 说明            |
| ---------- | -------- | ----------------- | --------------- |
| `src/csv`  | C        | 无                | HCSV 解析器相关 |
| `src/file` | C++      | CryptoPP, sqlite3 | 文件存储相关    |
| `src/gui`  | C++      | wxWidgets         | GUI 和主要功能  |
