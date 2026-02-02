FROM ubuntu:24.04

# Install Packages
RUN apt-get update -y && apt-get install -y --no-install-recommends \
    build-essential \
    python3 \
    python3-pip
    
# Install Python requirements
RUN pip3 install requests pyinstaller --break-system-packages
