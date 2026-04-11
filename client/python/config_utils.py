import os
import json

DEFAULT_CONFIG_FILE = os.path.expanduser("~/.config/ctodo/ctodo.conf")
CLIENT_DEFAULT_URL = 'http://localhost'
CLIENT_DEFAULT_PORT = 8080

def load_config(file_path=DEFAULT_CONFIG_FILE):
    try:
        with open(file_path, 'r') as file:
            data = json.load(file)
        return data
    except FileNotFoundError:
        if file_path == DEFAULT_CONFIG_FILE:
            print('Default config file could not be loaded, falling back to client defaults.')
        else:
            print(f'{file_path} does not exist, falling back to client defaults.')
    except json.decoder.JSONDecodeError:
        print('Invalid config file, falling back to defaults.')
    except Exception as e:
        print('Unexpected error with config file, falling back to defaults.')
    return None

def get_default_url(config):
    if not config is None:
        if not config['client'] is None:
            if not config['client']['server-url'] is None:
                return config['client']['server-url']
    return CLIENT_DEFAULT_URL

def get_default_port(config):
    if not config is None:
        if not config['client'] is None:
            if not config['client']['server-port'] is None:
                return config['client']['server-port']
    return CLIENT_DEFAULT_PORT
