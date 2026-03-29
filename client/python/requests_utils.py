import sys
import requests

def _safe_request(request_method, url, data=None):
    try:
        response = request_method(url, data=data)
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

def get_request(url, data=None):
    return _safe_request(requests.get, url, data)

def post_request(url, data=None):
    return _safe_request(requests.post, url, data)

def patch_request(url, data=None):
    return _safe_request(requests.patch, url, data)

def delete_request(url, data=None):
    return _safe_request(requests.delete, url, data)

