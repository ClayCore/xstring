.PHONY: build check clean docs format

BUILDDIR := target

$(BUILDDIR):
	mkdir -p $@

# Builds and compiles the project using the `meson.build` file
build: | $(BUILDDIR)
	meson $(BUILDDIR)
	meson compile -C $(BUILDDIR)

# Cleans the build directory.
clean:
	rm -rf $(BUILDDIR)

# Runs static analysis targets provided by the `meson.build` file.
check: | $(BUILDDIR)
	meson compile clang-tidy -C $(BUILDDIR)
	meson compile cppcheck -C $(BUILDDIR)

# Generates documentation using `doxygen`
docs: | $(BUILDDIR)
	doxygen Doxyfile

# Formats all the files using `clang-format`
format: | $(BUILDIR)
	ninja -C $(BUILDDIR) clang-format
