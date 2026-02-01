import requests
import argparse

parser = argparse.ArgumentParser(prog='ctodo')
subparsers = parser.add_subparsers(dest='command', help='commands')

ls = subparsers.add_parser('ls', help='List tasks.')

add = subparsers.add_parser('add', help='Add task.')
add.add_argument('title', nargs='+')

delete = subparsers.add_parser('delete', help='Delete task.')
delete.add_argument('task', type=int)

args = parser.parse_args()

if args.command == 'ls':
    response = requests.get('http://localhost:8080/tasks')
    print(response.text, end='')
elif args.command == 'add':
    data = {'title': " ".join(args.title)}
    response = requests.post('http://localhost:8080/tasks', data=data)
    print(response.text, end='')
elif args.command == 'delete':
    data = {'task': args.task}
    response = requests.delete('http://localhost:8080/tasks', data=data)
    print(response.text, end='')
else:
    parser.print_help()
