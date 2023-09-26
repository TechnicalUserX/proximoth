# Proximoth Docker File
ARG OS_VERSION=bookworm-slim
ARG OS=debian
FROM ${OS}:${OS_VERSION}
ENV PROXIMOTH_DOCKER=true
SHELL ["/bin/bash", "-c"]
RUN apt update
RUN apt install -y libpcap-dev
RUN apt install -y make
RUN apt install -y gcc
RUN mkdir -p /proximoth
COPY . /proximoth
WORKDIR /proximoth
RUN make
RUN make install
ENTRYPOINT ["proximoth"]
