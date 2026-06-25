FROM ubuntu:24.04 AS builder

ARG DEBIAN_FRONTEND=noninteractive
ARG CROW_VERSION=v1.3.0

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    ca-certificates \
    pkg-config \
    libasio-dev \
    libssl-dev \
    zlib1g-dev \
    libcpp-httplib-dev \
    && rm -rf /var/lib/apt/lists/*

RUN git clone --branch "${CROW_VERSION}" --depth 1 \
        https://github.com/CrowCpp/Crow.git /tmp/crow \
    && cmake -S /tmp/crow -B /tmp/crow/build \
        -DCROW_BUILD_EXAMPLES=OFF \
        -DCROW_BUILD_TESTS=OFF \
    && cmake --build /tmp/crow/build --parallel \
    && cmake --install /tmp/crow/build

WORKDIR /app

COPY CMakeLists.txt .
COPY src/ src/

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --parallel


FROM ubuntu:24.04 AS runtime

RUN apt-get update && apt-get install -y --no-install-recommends \
    libcpp-httplib0.14t64 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /app/build/cpp_app_client /usr/local/bin/

EXPOSE 9092

ENTRYPOINT ["/usr/local/bin/cpp_app_client"]