project(
    'xstring', 'c',
    version: '0.1.0-dev',
    default_options: [
        'c_std=c89',
        'warning_level=3',
        'optimization=3',
        'b_sanitize=address,undefined'
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
    'src/main.c',
]

# ------------------------------------------------------------------------------- #
# Linking into executable ------------------------------------------------------- #
# ------------------------------------------------------------------------------- #
executable(
    'xstring',
    sources,
    include_directories: shared_include_directories,
    win_subsystem: 'console',
    c_pch: 'inc/pch/common_pch.h'
)
