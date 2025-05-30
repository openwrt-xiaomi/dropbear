/*
 * Dropbear SSH
 * 
 * Copyright (c) 2002,2003 Matt Johnston
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

#include "includes.h"
#include "dbutil.h"

static int runprog(const char *multipath,
		const char *progname, int argc, char ** argv, int *match) {
	*match = DROPBEAR_SUCCESS;

#ifdef DBMULTI_dropbear
		if (strcmp(progname, "dropbear") == 0) {
			return dropbear_main(argc, argv, multipath);
		}
#endif
#ifdef DBMULTI_dbclient
		if (strcmp(progname, "dbclient") == 0
				|| strcmp(progname, "ssh") == 0) {
			return cli_main(argc, argv);
		}
#endif
#ifdef DBMULTI_dropbearkey
		if (strcmp(progname, "dropbearkey") == 0
				|| strcmp(progname, "ssh-keygen") == 0) {
			return dropbearkey_main(argc, argv);
		}
#endif
#ifdef DBMULTI_dropbearconvert
		if (strcmp(progname, "dropbearconvert") == 0) {
			return dropbearconvert_main(argc, argv);
		}
#endif
#ifdef DBMULTI_scp
		if (strcmp(progname, "scp") == 0) {
			return scp_main(argc, argv);
		}
#endif
	*match = DROPBEAR_FAILURE;
	return 1;
}

int main(int argc, char ** argv) {
	int i;
	for (i = 0; i < 2; i++) {
		const char* multipath = NULL;
		if (i == 1) {
			multipath = argv[0];
		}
		/* Try symlink first, then try as an argument eg "dropbearmulti dbclient host ..." */
		if (argc > i) {
			int match, res;
			/* figure which form we're being called as */
			const char* progname = basename(argv[i]);
			if (strcmp(progname, "dropbear") == 0 && argc > i+1) {
				res = runprog(NULL, (const char *)argv[i+1], argc-i-1, &argv[i+1], &match);
				if (match == DROPBEAR_SUCCESS) {
					return res;
				}
			}
			res = runprog(NULL, progname, argc-i, &argv[i], &match);
			if (match == DROPBEAR_SUCCESS) {
				return res;
			}
		}
	}

#ifdef DBMULTI_dropbear
	return dropbear_main(argc, argv, NULL);
#endif

	fprintf(stderr, "Dropbear SSH multi-purpose v%s\n"
			"Make a symlink pointing at this binary with one of the\n"
			"following names or run 'dropbearmulti <command>'.\n"
#ifdef DBMULTI_dropbear
			"'dropbear' - the Dropbear server\n"
#endif
#ifdef DBMULTI_dbclient
			"'dbclient' or 'ssh' - the Dropbear client\n"
#endif
#ifdef DBMULTI_dropbearkey
			"'dropbearkey' or 'ssh-keygen' - the key generator\n"
#endif
#ifdef DBMULTI_dropbearconvert
			"'dropbearconvert' - the key converter\n"
#endif
#ifdef DBMULTI_scp
			"'scp' - secure copy\n"
#endif
			,
			DROPBEAR_VERSION);
	exit(1);

}
