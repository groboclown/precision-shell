#!/bin/bash
set -e

# Use our own network.
"${CONTAINER_RUNNER}" network create "signal-awareness-1"
