# HomeAccounts 文档

AI Agent 请阅读以下引用内容：

> 不要修改此目录下的文件，除非收到明确指令并指明修改哪个文件。
>
> 如果需要的信息在这些文档里能找到，则无需再从源码中挖掘。如果在进行修改源码的动作时发现文档中的信息与源码不符，请报告“XXXX 信息似与源码不符，暂停动作”并暂停当前动作。下一步如果收到回复中文“继续”或英文 "go on" 则以源码中的信息为准继续动作。

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

| 段名        | 内容类型 | 说明                                                                        |
| ----------- | -------- | --------------------------------------------------------------------------- |
| `data/xxxx` | HCSV     | xxxx 为数字，代表一个年份，保存此年度的数据，相关代码在目录 `src/gui/data/` |
| `years`     | CSV      | 保存历年的汇总数据                                                          |
| `accounts`  | HCSV     | 保存账户信息，相关代码在 `src/gui/accounts/`                                |
| `import`    | CSV      | 导入的 CSV 文件                                                             |

HCSV 类型是一种经过改造的 CSV 格式，由 `HaCsv` 类实现，具体的读写操作在 `src/csv` 目录中，为 C 语言模块。

## HCSV 格式

HCSV 格式简要说明如下：

第一行为各列的标题（字段名），前面可以加几个 `#` 号，表示前几列的值将从以 `#` 开头的特殊行中解析；也可以没有 `#` 号，此时文件格式退化为普通的 CSV.

各数据行不再包含标有 `#` 号的这些列，这些列的值将从前面最近的 `#` 开头的行得到。如果前面没有出现过这种行，将设为默认值。

## CSV 字段类型

定义在 `src/csv/column_type.h`.

- CT_STR
- CT_INT
- CT_BOOL
- CT_MONEY
- CT_YEAR
- CT_DATE
- CT_TIME
- CT_IGNORE

## data/xxxx 字段

标题行：`#Date,Time,Account,Amount,Desc,Memo`

| 编号 | 字段名称 | 类型     | 含义     |
| ---: | -------- | -------- | -------- |
|    0 | Date     | CT_DATE  | 交易日期 |
|    1 | Time     | CT_TIME  | 交易时间 |
|    2 | Account  | CT_INT   | 账户 ID  |
|    3 | Amount   | CT_MONEY | 金额     |
|    4 | Desc     | CT_STR   | 说明     |
|    5 | Memo     | CT_STR   | 备注     |

## years 字段

标题行：`Year,Income,Outlay,Records`

| 编号 | 字段名称 | 类型     | 含义     |
| ---: | -------- | -------- | -------- |
|    0 | Year     | CT_YEAR  | 年份     |
|    1 | Income   | CT_MONEY | 总收入   |
|    2 | Outlay   | CT_MONEY | 总支出   |
|    3 | Records  | CT_INT   | 总记录数 |

## accounts 字段

标题行：`#Type,Id,Name`

| 编号 | 字段名称 | 类型   | 含义                                                   |
| ---: | -------- | ------ | ------------------------------------------------------ |
|    0 | Type     | CT_INT | 类型（0 - 无效，1 - 借记卡，2 - 信用卡，3 - 支付账户） |
|    1 | Id       | CT_INT | 账户 ID                                                |
|    2 | Name     | CT_STR | 名称                                                   |

## 源码结构

| 目录       | 编程语言 | 基础框架          | 说明            |
| ---------- | -------- | ----------------- | --------------- |
| `src/csv`  | C        | 无                | HCSV 解析器相关 |
| `src/file` | C++      | CryptoPP, sqlite3 | 文件存储相关    |
| `src/gui`  | C++      | wxWidgets         | GUI 和主要功能  |
