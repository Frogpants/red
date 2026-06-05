.PHONY: all init build run clean help

all: build

init:
	bash scripts/init.sh

build:
	bash scripts/build.sh

run: build
	./build/red

clean:
	rm -rf build

help:
	@echo "Usage:"
	@echo "  make init   - install dependencies and fetch local sources"
	@echo "  make build  - compile the project"
	@echo "  make run    - build and run the project"
	@echo "  make clean  - remove build outputs"
	@echo "  make help   - show this help"
