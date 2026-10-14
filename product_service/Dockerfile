FROM ubuntu:24.04 AS builder
WORKDIR /src
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential cmake git pkg-config libpqxx-dev ca-certificates  libasio-dev \
 && rm -rf /var/lib/apt/lists/*
COPY . .
RUN cmake -S . -B /build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF \
 && cmake --build /build

FROM ubuntu:24.04
RUN apt-get update && apt-get install -y --no-install-recommends \
    libpqxx-7.8t64 libpq5 ca-certificates \
 && rm -rf /var/lib/apt/lists/*
COPY --from=builder /build/product_service /usr/local/bin/product_service
EXPOSE 8081
CMD ["/usr/local/bin/product_service"]
