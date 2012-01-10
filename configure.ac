AC_INIT([egrl], [0.0.1], [driodeiros@gmail.com],
  [egrl], [http://github.com/drio/egrl])
AM_INIT_AUTOMAKE([1.10 no-define foreign])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 -Wall no-define])
AC_CONFIG_HEADERS([config.h])

# Check for programs
AC_PROG_CXX
AC_PROG_CC

# Check for headers
AC_LANG([C++])
AC_CHECK_HEADER([zlib.h])

# Check for libraries
AC_SEARCH_LIBS([gzopen],[z],,[AC_MSG_ERROR([libz not found, please install zlib (http://www.zlib.net/)])])

# Check for the google sparse hash
AC_ARG_WITH(sparsehash, AS_HELP_STRING([--with-sparsehash=PATH],
  [specify directory containing the google sparsehash headers http://code.google.com/p/google-sparsehash/)]))
if test "$with_sparsehash" -a -d "$with_sparsehash"; then
  sparsehash_include="-I$with_sparsehash/include"
fi

# Set compiler flags.
AC_SUBST(AM_CXXFLAGS, "-Wall -Wextra -Werror")
AC_SUBST(CXXFLAGS, "-O3")
AC_SUBST(CFLAGS, "-O3")
AC_SUBST(CPPFLAGS, "$CPPFLAGS $sparsehash_include $bamtools_include")

# Ensure the sparse hash is available
# Not using sparse_hash_map but I will check for it anyway.
AC_CHECK_HEADERS([google/sparse_hash_map],,[AC_MSG_ERROR([google sparse hash library is required: http://code.google.com/p/google-sparsehash/])])
AC_CHECK_HEADERS([google/dense_hash_map],,[AC_MSG_ERROR([google sparse hash library is required: http://code.google.com/p/google-sparsehash/])])

# Check for hash table headers
AC_CHECK_HEADERS([ \
 google/dense_hash_map \
 google/sparse_hash_map \
])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
