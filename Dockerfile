FROM gcc:10 AS builder

RUN apt-get update && apt-get install -y cmake

WORKDIR /app
COPY src ./src
COPY include ./include
COPY CMakeLists.txt .

RUN mkdir build && cd build && \
    cmake .. && \
    make

FROM debian:11-slim
WORKDIR /app

COPY --from=builder /app/build/server .
COPY static/ ./static
COPY systemPrompt.txt .

EXPOSE 8080
CMD ["./server"]