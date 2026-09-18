#!/usr/bin/env python3

import csv
import sys
from datetime import datetime
from decimal import Decimal, InvalidOperation
from pathlib import Path


OUTPUT_COLUMNS = ["Date", "Amount", "Desc"]
REQUIRED_COLUMNS = [
    "币别",
    "钞汇",
    "交易日期",
    "交易金额",
    "摘要",
    "交易地点/附言",
    "对方账号与户名",
]


def validate_row(row):
    errors = []

    if row["币别"] != "人民币元":
        errors.append("币别应为`人民币元`")
    if row["钞汇"] != "钞":
        errors.append("钞汇应为`钞`")

    try:
        transaction_date = datetime.strptime(row["交易日期"], "%Y%m%d")
    except ValueError:
        transaction_date = None
        errors.append("交易日期格式应为`yyyyMMdd`")

    try:
        amount = Decimal(row["交易金额"].replace(",", ""))
    except InvalidOperation:
        amount = None
        errors.append("交易金额应为数字")

    peer = row["对方账号与户名"].strip()
    if not peer:
        account_number = ""
        account_name = ""
    else:
        peer_parts = peer.split("/", 1)
        if len(peer_parts) == 1:
            account_number = ""
            account_name = peer
        else:
            account_number, account_name = peer_parts
            if not account_name:
                account_number = None
                account_name = None
                errors.append("对方账号与户名应为账号/户名，或仅为户名")

    if errors:
        return None, errors

    description = format_description(
        row["摘要"], row["交易地点/附言"], account_name, account_number
    )
    return [transaction_date.strftime("%Y-%m-%d"), format(-amount, "f"), description], []


def format_description(summary, note, account_name, account_number):
    parts = []
    for value in (summary, note, account_name, account_number):
        normalized_value = " ".join(value.split())
        if normalized_value and normalized_value not in parts:
            parts.append(normalized_value)
    return ":".join(parts)


def transform(input_path, output_path):
    with input_path.open("r", encoding="utf-8-sig", newline="") as input_file:
        reader = csv.reader(input_file)
        for _ in range(3):
            next(reader, None)
        header = next(reader, None)
        if header is None:
            raise ValueError("未找到明细数据标题行")

        missing_columns = [column for column in REQUIRED_COLUMNS if column not in header]
        if missing_columns:
            raise ValueError(f"明细数据缺少列：{'、'.join(missing_columns)}")

        with output_path.open("w", encoding="utf-8", newline="") as output_file:
            writer = csv.writer(output_file)
            writer.writerow(OUTPUT_COLUMNS)
            for row in reader:
                if not row or all(not value.strip() for value in row):
                    continue
                if len(row) != len(header):
                    print(f"第 {reader.line_num} 行：列数应为 {len(header)}")
                    continue

                output_row, errors = validate_row(dict(zip(header, row)))
                if errors:
                    print(f"第 {reader.line_num} 行：{'；'.join(errors)}")
                    continue
                writer.writerow(output_row)


def main():
    if len(sys.argv) != 2:
        print(f"用法：{Path(sys.argv[0]).name} 输入文件", file=sys.stderr)
        return 2

    output_path = Path("output.csv")
    if output_path.exists():
        answer = input("文件 output.csv 已存在，是否覆盖？[y/N] ")
        if answer.lower() != "y":
            print("已取消输出")
            return 1

    try:
        transform(Path(sys.argv[1]), output_path)
    except (OSError, ValueError) as error:
        print(f"错误：{error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
