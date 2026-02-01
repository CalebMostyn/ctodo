import requests
import argparse

url = 'http://localhost'
port = 8080

parser = argparse.ArgumentParser(prog='ctodo')
subparsers = parser.add_subparsers(dest='command', help='commands')

ls = subparsers.add_parser('ls', help='List tasks.')

add = subparsers.add_parser('add', help='Add task.')
add.add_argument('title', nargs='+')

delete = subparsers.add_parser('delete', help='Delete task.')
delete.add_argument('task', type=int)

done = subparsers.add_parser('done', help='Mark task as done.')
done.add_argument('task', type=int)

undo = subparsers.add_parser('undo', help='Mark task as not done.')
undo.add_argument('task', type=int)

rename = subparsers.add_parser('rename', help='Rename task.')
rename.add_argument('task', type=int)
rename.add_argument('title', nargs='+')

rename = subparsers.add_parser('clear', help='Clear multiple tasks.')
rename.add_argument('tasks')

args = parser.parse_args()

if args.command == 'ls':
    response = requests.get(f'{url}:{port}/tasks')
    print(response.text, end='')
elif args.command == 'add':
    data = {'title': " ".join(args.title)}
    response = requests.post(f'{url}:{port}/tasks', data=data)
    print(response.text, end='')
elif args.command == 'delete':
    data = {'task': args.task}
    response = requests.delete(f'{url}:{port}/tasks', data=data)
    print(response.text, end='')
elif args.command == 'done':
    data = {'task': args.task, 'done': True}
    response = requests.patch(f'{url}:{port}/tasks', data=data)
    print(response.text, end='')
elif args.command == 'undo':
    data = {'task': args.task, 'done': False}
    response = requests.patch(f'{url}:{port}/tasks', data=data)
    print(response.text, end='')
elif args.command == 'rename':
    data = {'task': args.task, 'title': " ".join(args.title)}
    response = requests.patch(f'{url}:{port}/tasks', data=data)
    print(response.text, end='')
elif args.command == 'clear':
    tasks = args.tasks
    if args.tasks == "all":
        tasks = "*"
    data = {'task': tasks}
    response = requests.delete(f'{url}:{port}/tasks', data=data)
    print(response.text, end='')
else:
    parser.print_help()
