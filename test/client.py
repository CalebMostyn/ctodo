import requests

response = requests.get('http://localhost:8080/hi')
print(response.text)
