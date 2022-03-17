#/bin/bash
find . \( -name "*.cpp" -or -name "*.h" \) -exec clang-format-9 --verbose -i {} \;

