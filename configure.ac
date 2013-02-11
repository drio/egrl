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

# Google perf tools
#AC_ARG_ENABLE(tcmalloc, [  --disable-tcmalloc disable tcmalloc for memory allocation], [], [AC_CHECK_LIB([tcmalloc],malloc)])
AC_ARG_ENABLE(perftools, [  --enable-perftools  enable google performance tools for profiling and heap checking], [LDFLAGS="${LDFLAGS} -Wl --no-as-needed -lprofiler"; LDADD="${LDADD} -lprofiler"], [])

AC_CHECK_LIB(pthread, pthread_create,, [AC_MSG_ERROR([required   library pthread missing])])

#
AC_MSG_CHECKING(whether this is a git repository)
gitrev="`git log 2>/dev/null | grep -m 1 commit | awk '{print $2}'`";
if test -n "${gitrev}"; then
    GITREV="${gitrev}";
    AC_REVISION(["${GITREV}"])
    AC_DEFINE_UNQUOTED(GIT_REV, "${GITREV}", [Define to be the git revision number])
    AC_MSG_RESULT(yes)
else
    AC_MSG_RESULT(no)
fi

# Set compiler flags.
#AC_SUBST(AM_CXXFLAGS, "-Wall -Wextra -Werror -g")
AC_SUBST(AM_CXXFLAGS, "-Wall -Wextra -g")
AC_SUBST(CXXFLAGS, "-O3")
AC_SUBST(CFLAGS, "-O3")
AC_SUBST(CPPFLAGS, "$CPPFLAGS $sparsehash_include $bamtools_include")

# Ensure the sparse hash is available
AC_CHECK_HEADERS([google/sparse_hash_map],,[AC_MSG_ERROR([google sparse hash library is required: http://code.google.com/p/google-sparsehash/])])
AC_CHECK_HEADERS([google/dense_hash_map],,[AC_MSG_ERROR([google sparse hash library is required: http://code.google.com/p/google-sparsehash/])])

# Check for hash table headers
AC_CHECK_HEADERS([ \
 google/dense_hash_map \
 google/sparse_hash_map \
])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
