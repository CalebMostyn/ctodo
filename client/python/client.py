import sys
import requests
import argparse

DEFAULT_URL = 'http://localhost'
DEFAULT_PORT = 8080

# Parsing Args
parser = argparse.ArgumentParser(prog='ctodo')
parser.add_argument('-u', '--url', help='Server URL.', default=DEFAULT_URL)
parser.add_argument('-p', '--port', help='Server port.', default=DEFAULT_PORT)

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
url = args.url
port = args.port

def __get_request(url, data=None):
    try:
        response = requests.get(url, data=data)
    except requests.exceptions.ConnectionError:
        print('Failed to connect to server.')
        sys.exit(1)
    except requests.exceptions.RequestException as e:
        print(f'Error {e}')
        sys.exit(1)

    if response.status_code >= 400:
        print(response.json()['message'])
        sys.exit(1)
    return response.json()

def __post_request(url, data=None):
    try:
        response = requests.post(url, data=data)
    except requests.exceptions.ConnectionError:
        print('Failed to connect to server.')
        sys.exit(1)
    except requests.exceptions.RequestException as e:
        print(f'Error {e}')
        sys.exit(1)

    if response.status_code >= 400:
        print(response.json()['message'])
        sys.exit(1)
    return response.json()

def __patch_request(url, data=None):
    try:
        response = requests.patch(url, data=data)
    except requests.exceptions.ConnectionError:
        print('Failed to connect to server.')
        sys.exit(1)
    except requests.exceptions.RequestException as e:
        print(f'Error {e}')
        sys.exit(1)

    if response.status_code >= 400:
        print(response.json()['message'])
        sys.exit(1)
    return response.json()

def __delete_request(url, data=None):
    try:
        response = requests.delete(url, data=data)
    except requests.exceptions.ConnectionError:
        print('Failed to connect to server.')
        sys.exit(1)
    except requests.exceptions.RequestException as e:
        print(f'Error {e}')
        sys.exit(1)

    if response.status_code >= 400:
        print(response.json()['message'])
        sys.exit(1)
    return response.json()

if args.command == 'ls':
    response = __get_request(f'{url}:{port}/tasks')
    print('Tasks:')
    index = 0
    for task in response['tasks']:
        print(f'{index}: {task['title']}{' - X' if bool(task['complete']) else ''}')
        index += 1
elif args.command == 'add':
    data = {'title': " ".join(args.title)}
    response = __post_request(f'{url}:{port}/tasks', data=data)
    print(response['message'])
elif args.command == 'delete':
    data = {'task': args.task}
    response = __delete_request(f'{url}:{port}/tasks', data=data)
    print(response['message'])
elif args.command == 'done':
    data = {'task': args.task, 'done': True}
    response = __patch_request(f'{url}:{port}/tasks', data=data)
    print(response['message'])
elif args.command == 'undo':
    data = {'task': args.task, 'done': False}
    response = __patch_request(f'{url}:{port}/tasks', data=data)
    print(response['message'])
elif args.command == 'rename':
    data = {'task': args.task, 'title': " ".join(args.title)}
    response = __patch_request(f'{url}:{port}/tasks', data=data)
    print(response['message'])
elif args.command == 'clear':
    tasks = args.tasks
    if args.tasks == "all":
        tasks = "*"
    data = {'task': tasks}
    response = __delete_request(f'{url}:{port}/tasks', data=data)
    print(response['message'])
else:
    parser.print_help()
