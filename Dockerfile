FROM ubuntu:24.04

# Install Packages
RUN apt-get update -y && apt-get install -y --no-install-recommends \
    build-essential \
    python3 \
    python3-pip \
    python3-venv
    
# Setup venv for Python build
RUN mkdir workspace/ && cd workspace/ && \
    python3 -m venv venv && \
    . venv/bin/activate && \
    pip3 install requests && \
    pip3 install pyinstaller
