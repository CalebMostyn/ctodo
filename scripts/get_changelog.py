import argparse
from markdown_extract import MarkdownExtractor

def read_file(path):
    with open(path, 'r', encoding='utf-8') as file:
        file_content = file.read()
    return file_content

def get_content_by_version(md, version):
    for section in md.list():
        if f"[{version}]" in section:
            return md[section]
    return f"Version {version} has no entry."

parser = argparse.ArgumentParser()
parser.add_argument("file_path", help="Path to CHANGELOG file.")
parser.add_argument("version", help="Version as used in CHANGELOG, ex. 'v1.0.0'.")
args = parser.parse_args()

md = MarkdownExtractor(read_file(args.file_path))
print(get_content_by_version(md["Changelog"], args.version))
