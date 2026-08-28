#!/usr/bin/env bash
set -euo pipefail

PORT=8080
HOST=127.0.0.1

TEST_FILE="public/hello.txt"
DOWNLOADED_FILE="/tmp/hello.txt"

cleanup() {
    if [[ -n "${SERVER_PID:-}" ]]; then
        kill "$SERVER_PID" 2>/dev/null || true
        wait "$SERVER_PID" 2>/dev/null || true
    fi

    rm -f "$DOWNLOADED_FILE"
    make clean
}

trap cleanup EXIT

echo "Building..."
make

echo "Starting server..."
./server &
SERVER_PID=$!

# Give the server a moment to start.
sleep 0.2

echo "GET /hello.txt"

STATUS=$(curl -s \
    -o "tmp/hello.txt" \
    -w "%{http_code}" \
    "http://127.0.0.1:8080/hello.txt")

if [[ "$STATUS" != "200" ]]; then
    echo "FAIL: expected HTTP 200, got $STATUS"
    exit 1
fi

if ! cmp -s "$TEST_FILE" "$DOWNLOADED_FILE"; then
    echo "FAIL: response body does not exactly match $TEST_FILE"

    echo
    echo "Expected:"
    xxd "$TEST_FILE"

    echo
    echo "Received:"
    xxd "$DOWNLOADED_FILE"

    exit 1
fi
if cmp -s "$TEST_FILE" "$DOWNLOADED_FILE"; then
    echo "PASS: GET returned exact file bytes"
    exit 1
fi

echo "FAIL"