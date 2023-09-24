project(
    'xstring', 'c',
    version: '0.1.0-dev',
    default_options: [
        'c_std=c17',
        'warning_level=3',
        'optimization=3',
    ]
)

# ------------------------------------------------------------------------------- #
# Variables --------------------------------------------------------------------- #
# ------------------------------------------------------------------------------- #
compiler = meson.get_compiler('c')
os = host_machine.system()

shared_include_directories = [
    include_directories('src'),
]

sources = [
    'src/test/tests.c',
    'src/main.c',
]

# ------------------------------------------------------------------------------- #
# Dependencies ------------------------------------------------------------------ #
# ------------------------------------------------------------------------------- #
all_deps = []

# ------------------------------------------------------------------------------- #
# Linking into executable ------------------------------------------------------- #
# ------------------------------------------------------------------------------- #
executable(
    'xstring',
    sources,
    include_directories: shared_include_directories,
    dependencies: all_deps,
    c_pch: 'inc/pch/common_pch.h'
)
