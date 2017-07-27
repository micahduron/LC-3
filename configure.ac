AC_INIT([lc3], [0.0.0])
AM_INIT_AUTOMAKE

# Set language to C++
AC_LANG(C++)

# Check for C++ compiler
#   Currently I am only supporting g++. Supporting more than one
#   compiler requires a more complex autoconf script, and that's something
#   I want to avoid right now.
AC_PROG_CXX([g++])
CXXFLAGS="-std=c++17 -Werror -Wall -Wextra"

# Check for C++ preprocessor
AC_PROG_CXXCPP

AC_ARG_ENABLE(
    debug,
    AS_HELP_STRING([--enable-debug], [Build executables with debug symbols.])
)
AS_IF([test "$enable_debug" = yes], [
    CXXFLAGS="$CXXFLAGS -g"
])

# Check for C++17 compiler support
#   This isn't a definitive check. Some compilers only have partial support
#   for C++17. I may add checks for specific C++17 features later on, but
#   for now, this will do.
AC_MSG_CHECKING(for C++17 feature support)
saved_CXXFLAGS="$CXXFLAGS"
CXXFLAGS="-std=c++17"

cxx17_support=no

# Attempt to compile a program containing a nested namespace definition, a
# feature introduced in C++17.
AC_COMPILE_IFELSE(
  [
    AC_LANG_PROGRAM(
      [[namespace A::B {int a = 42;}]],
      [[]]
    )
  ],
  [cxx17_support=yes],
  []
)
AC_MSG_RESULT($cxx17_support)

if test $cxx17_support = no; then
    AC_MSG_ERROR(Support for the C++17 standard or higher is required.)
fi
CXXFLAGS="$saved_CXXFLAGS"

AC_CONFIG_FILES([Makefile tests/Makefile])
AC_OUTPUT
