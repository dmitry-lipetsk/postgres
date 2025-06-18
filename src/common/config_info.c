/*-------------------------------------------------------------------------
 *
 * config_info.c
 *		Common code for pg_config output
 *
 *
 * Portions Copyright (c) 1996-2025, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/common/config_info.c
 *
 *-------------------------------------------------------------------------
 */

#ifndef FRONTEND
#include "postgres.h"
#else
#include "postgres_fe.h"
#endif

#include "common/config_info.h"
#include "common/relaxmem.h"


/*
 * get_configdata(const char *my_exec_path, size_t *configdata_len)
 *
 * Get configure-time constants. The caller is responsible
 * for pfreeing the result.
 */
ConfigData *
get_configdata(const char *my_exec_path, size_t *configdata_len)
{
	ConfigData *configdata;
	char		path[MAXPGPATH];
	char	   *lastsep;
	int			i = 0;

	/* Adjust this to match the number of items filled below */
	*configdata_len = 23;
	configdata = relaxmem__palloc_array(ConfigData, *configdata_len);

	configdata[i].name = relaxmem__pstrdup("BINDIR");
	strlcpy(path, my_exec_path, sizeof(path));
	lastsep = strrchr(path, '/');
	if (lastsep)
		*lastsep = '\0';
	cleanup_path(path);
	configdata[i].setting = relaxmem__pstrdup(path);
	i++;

	configdata[i].name = relaxmem__pstrdup("DOCDIR");
	get_doc_path(my_exec_path, path);
	cleanup_path(path);
	configdata[i].setting = relaxmem__pstrdup(path);
	i++;

	configdata[i].name = relaxmem__pstrdup("HTMLDIR");
	get_html_path(my_exec_path, path);
	cleanup_path(path);
	configdata[i].setting = relaxmem__pstrdup(path);
	i++;

	configdata[i].name = relaxmem__pstrdup("INCLUDEDIR");
	get_include_path(my_exec_path, path);
	cleanup_path(path);
	configdata[i].setting = relaxmem__pstrdup(path);
	i++;

	configdata[i].name = relaxmem__pstrdup("PKGINCLUDEDIR");
	get_pkginclude_path(my_exec_path, path);
	cleanup_path(path);
	configdata[i].setting = relaxmem__pstrdup(path);
	i++;

	configdata[i].name = relaxmem__pstrdup("INCLUDEDIR-SERVER");
	get_includeserver_path(my_exec_path, path);
	cleanup_path(path);
	configdata[i].setting = relaxmem__pstrdup(path);
	i++;

	configdata[i].name = relaxmem__pstrdup("LIBDIR");
	get_lib_path(my_exec_path, path);
	cleanup_path(path);
	configdata[i].setting = relaxmem__pstrdup(path);
	i++;

	configdata[i].name = relaxmem__pstrdup("PKGLIBDIR");
	get_pkglib_path(my_exec_path, path);
	cleanup_path(path);
	configdata[i].setting = relaxmem__pstrdup(path);
	i++;

	configdata[i].name = relaxmem__pstrdup("LOCALEDIR");
	get_locale_path(my_exec_path, path);
	cleanup_path(path);
	configdata[i].setting = relaxmem__pstrdup(path);
	i++;

	configdata[i].name = relaxmem__pstrdup("MANDIR");
	get_man_path(my_exec_path, path);
	cleanup_path(path);
	configdata[i].setting = relaxmem__pstrdup(path);
	i++;

	configdata[i].name = relaxmem__pstrdup("SHAREDIR");
	get_share_path(my_exec_path, path);
	cleanup_path(path);
	configdata[i].setting = relaxmem__pstrdup(path);
	i++;

	configdata[i].name = relaxmem__pstrdup("SYSCONFDIR");
	get_etc_path(my_exec_path, path);
	cleanup_path(path);
	configdata[i].setting = relaxmem__pstrdup(path);
	i++;

	configdata[i].name = relaxmem__pstrdup("PGXS");
	get_pkglib_path(my_exec_path, path);
	strlcat(path, "/pgxs/src/makefiles/pgxs.mk", sizeof(path));
	cleanup_path(path);
	configdata[i].setting = relaxmem__pstrdup(path);
	i++;

	configdata[i].name = relaxmem__pstrdup("CONFIGURE");
	configdata[i].setting = relaxmem__pstrdup(CONFIGURE_ARGS);
	i++;

	configdata[i].name = relaxmem__pstrdup("CC");
#ifdef VAL_CC
	configdata[i].setting = relaxmem__pstrdup(VAL_CC);
#else
	configdata[i].setting = relaxmem__pstrdup(_("not recorded"));
#endif
	i++;

	configdata[i].name = relaxmem__pstrdup("CPPFLAGS");
#ifdef VAL_CPPFLAGS
	configdata[i].setting = relaxmem__pstrdup(VAL_CPPFLAGS);
#else
	configdata[i].setting = relaxmem__pstrdup(_("not recorded"));
#endif
	i++;

	configdata[i].name = relaxmem__pstrdup("CFLAGS");
#ifdef VAL_CFLAGS
	configdata[i].setting = relaxmem__pstrdup(VAL_CFLAGS);
#else
	configdata[i].setting = relaxmem__pstrdup(_("not recorded"));
#endif
	i++;

	configdata[i].name = relaxmem__pstrdup("CFLAGS_SL");
#ifdef VAL_CFLAGS_SL
	configdata[i].setting = relaxmem__pstrdup(VAL_CFLAGS_SL);
#else
	configdata[i].setting = relaxmem__pstrdup(_("not recorded"));
#endif
	i++;

	configdata[i].name = relaxmem__pstrdup("LDFLAGS");
#ifdef VAL_LDFLAGS
	configdata[i].setting = relaxmem__pstrdup(VAL_LDFLAGS);
#else
	configdata[i].setting = relaxmem__pstrdup(_("not recorded"));
#endif
	i++;

	configdata[i].name = relaxmem__pstrdup("LDFLAGS_EX");
#ifdef VAL_LDFLAGS_EX
	configdata[i].setting = relaxmem__pstrdup(VAL_LDFLAGS_EX);
#else
	configdata[i].setting = relaxmem__pstrdup(_("not recorded"));
#endif
	i++;

	configdata[i].name = relaxmem__pstrdup("LDFLAGS_SL");
#ifdef VAL_LDFLAGS_SL
	configdata[i].setting = relaxmem__pstrdup(VAL_LDFLAGS_SL);
#else
	configdata[i].setting = relaxmem__pstrdup(_("not recorded"));
#endif
	i++;

	configdata[i].name = relaxmem__pstrdup("LIBS");
#ifdef VAL_LIBS
	configdata[i].setting = relaxmem__pstrdup(VAL_LIBS);
#else
	configdata[i].setting = relaxmem__pstrdup(_("not recorded"));
#endif
	i++;

	configdata[i].name = relaxmem__pstrdup("VERSION");
	configdata[i].setting = relaxmem__pstrdup("PostgreSQL " PG_VERSION);
	i++;

	Assert(i == *configdata_len);

	return configdata;
}
