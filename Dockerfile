FROM gcc:10 AS builder

WORKDIR /app
COPY src .
COPY include ./include

RUN g++ -std=c++11 -Iinclude -o server main.cpp -lpthread

FROM debian:11-slim
WORKDIR /app

COPY --from=builder /app/server .
COPY static/ ./static
COPY systemPrompt.txt .

EXPOSE 8080
CMD ["./server"]