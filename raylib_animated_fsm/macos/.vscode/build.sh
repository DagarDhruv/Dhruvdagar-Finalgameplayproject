#!/bin/bash

# Check for command argument
if [ "$#" -lt 1 ]; then
    echo "Usage: $0 {all|build|run|build_web|run_web|clean|clean_toolchain} [debug|release]"
    exit 1
fi

COMMAND=$1
CONFIG=${2:-}  # Optional second argument, default to empty

# Execute the make command with the specified command and config if provided
if [ -z "$CONFIG" ]; then
    make "$COMMAND"
else
    make "$COMMAND" CONFIG="$CONFIG"
fi

