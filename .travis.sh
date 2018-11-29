#!/bin/sh
set -ex
hhvm --version

composer install --ignore-platform-reqs

hh_client

hhvm vendor/bin/hacktest tests/
# Circular dependency that needs updating
# hhvm vendor/bin/hhast-lint
