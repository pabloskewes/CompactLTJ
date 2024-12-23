SRC_FILES := $(shell find . -name "*.cpp" -o -name "*.h" -o -name "*.hpp")

# Target: Format all source files using clang-format
.PHONY: format
format:
	@echo "Formatting all C++ files..."
	@clang-format -i $(SRC_FILES)
	@echo "Formatting complete!"

# Target: Check formatting without applying changes
.PHONY: check-format
check-format:
	@echo "Checking formatting for C++ files..."
	@clang-format --dry-run --Werror $(SRC_FILES)
	@echo "All files are properly formatted!"

.PHONY: clean
clean:
	@echo "Cleaning up..."
	@rm -rf build
	@echo "Cleanup complete!"

.PHONY: build
build:
	@echo "Building project..."
	@mkdir -p build
	@cd build && cmake .. && make
	@echo "Build complete!"

.PHONY: run
run:
	@echo "Running project..."
	@./build/main
	@echo "Project execution complete!"

.PHONY: test
test:
	@echo "Running tests..."
	@echo "Tests complete!"

# Help target to display available commands
.PHONY: help
help:
	@echo "Available commands:"
	@echo "  make format        - Format all C++ files in the project"
	@echo "  make check-format  - Check formatting without making changes"
