/* unzip.c -- decompress files in gzip or pkzip format.
 * Copyright (C) 1992-1993 Jean-loup Gailly
 * This is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License, see the file COPYING.
 *
 * The code in this file is derived from the file funzip.c written
 * and put in the public domain by Mark Adler.
 */

/*
   This version can extract files in gzip or pkzip format.
   For the latter, only the first entry is extracted, and it has to be
   either deflated or stored.
 */
/*
 * Changed: August 19, 1995. spd for MacGzip 1.0 (changed stderr)
 */

#ifdef RCSID
static char rcsid[] = "$Id$";
#endif

#include "tailor.h"
#include "gzip.h"
#include "crypt.h"

/* PKZIP header definitions */
#define LOCSIG 0x04034b50L      /* four-byte lead-in (lsb first) */
#define LOCFLG 6                /* offset of bit flag */
#define  CRPFLG 1               /*  bit for encrypted entry */
#define  EXTFLG 8               /*  bit for extended local header */
#define LOCHOW 8                /* offset of compression method */
#define LOCTIM 10               /* file mod time (for decryption) */
#define LOCCRC 14               /* offset of crc */
#define LOCSIZ 18               /* offset of compressed size */
#define LOCLEN 22               /* offset of uncompressed length */
#define LOCFIL 26               /* offset of file name field length */
#define LOCEXT 28               /* offset of extra field length */
#define LOCHDR 30               /* size of local header, including sig */
#define EXTHDR 16               /* size of extended local header, inc sig */


/* Globals */

int decrypt;        /* flag to turn on decryption */
char *key;          /* not used--needed to link crypt.c */
int pkzip = 0;      /* set for a pkzip file */
int ext_header = 0; /* set if extended local header */

/* ===========================================================================
 * Check zip file and advance inptr to the start of the compressed data.
 * Get ofname from the local header if necessary.
 */
int check_zipfile(in)
    int in;   /* input file descriptors */
{
    uch *h = inbuf + inptr; /* first local header */

    ifd = in;

    /* Check validity of local header, and skip name and extra fields */
    inptr += LOCHDR + SH(h + LOCFIL) + SH(h + LOCEXT);

    if (inptr > insize || LG(h) != LOCSIG) {
#ifndef _MAC_ERRORS_H_ /* spd */
	fprintf(stderr, "\n%s: %s: not a valid zip file\n",
#else
	DoError(NO_ERR, WARN_ERR, "%s: %s: not a valid zip file",
#endif
		progname, ifname);
	exit_code = ERROR;
	return ERROR;
    }
    method = h[LOCHOW];
    if (method != STORED && method != DEFLATED) {
#ifndef _MAC_ERRORS_H_ /* spd */
	fprintf(stderr,
		"\n%s: %s: first entry not deflated or stored -- use unzip\n",
#else
	DoError(NO_ERR, WARN_ERR,
		"%s: %s: first entry not deflated or stored -- use unzip",
#endif
		progname, ifname);
	exit_code = ERROR;
	return ERROR;
    }

    /* If entry encrypted, decrypt and validate encryption header */
    if ((decrypt = h[LOCFLG] & CRPFLG) != 0) {
#ifndef _MAC_ERRORS_H_ /* spd */
	fprintf(stderr, "\n%s: %s: encrypted file -- use unzip\n",
#else
	DoError(NO_ERR, WARN_ERR, "%s: %s: encrypted file -- use unzip",
#endif
		progname, ifname);
	exit_code = ERROR;
	return ERROR;
    }

    /* Save flags for unzip() */
    ext_header = (h[LOCFLG] & EXTFLG) != 0;
    pkzip = 1;

    /* Get ofname and time stamp from local header (to be done) */
    return OK;
}

/* ===========================================================================
 * Unzip in to out.  This routine works on both gzip and pkzip files.
 *
 * IN assertions: the buffer inbuf contains already the beginning of
 *   the compressed data, from offsets inptr to insize-1 included.
 *   The magic header has already been checked. The output buffer is cleared.
 */
int unzip(in, out)
    int in, out;   /* input and output file descriptors */
{
    ulg orig_crc = 0;       /* original crc */
    ulg orig_len = 0;       /* original uncompressed length */
    int n;
    uch buf[EXTHDR];        /* extended local header */

    ifd = in;
    ofd = out;

    updcrc(NULL, 0);           /* initialize crc */

    if (pkzip && !ext_header) {  /* crc and length at the end otherwise */
	orig_crc = LG(inbuf + LOCCRC);
	orig_len = LG(inbuf + LOCLEN);
    }

    /* Decompress */
    if (method == DEFLATED)  {

	int res = inflate();

	if (res == 3) {
	    error("out of memory");
	} else if (res != 0) {
	    error("invalid compressed data--format violated");
	}

    } else if (pkzip && method == STORED) {

	register ulg n = LG(inbuf + LOCLEN);

	if (n != LG(inbuf + LOCSIZ) - (decrypt ? RAND_HEAD_LEN : 0)) {

#ifndef _MAC_ERRORS_H_ /* spd */
	    fprintf(stderr, "len %ld, siz %ld\n", n, LG(inbuf + LOCSIZ));
	    error("invalid compressed data--length mismatch");
#else
	DoError(NO_ERR, WARN_ERR, "invalid compressed data--length mismatch\nlen %ld, siz %ld",
		n, LG(inbuf + LOCSIZ));
#endif
	}
	while (n--) {
	    uch c = (uch)get_byte();
#ifdef CRYPT
	    if (decrypt) zdecode(c);
#endif
	    put_ubyte(c);
	}
	flush_window();
    } else {
	error("internal error, invalid method");
    }

    /* Get the crc and original length */
    if (!pkzip) {
        /* crc32  (see algorithm.doc)
	 * uncompressed input size modulo 2^32
         */
	for (n = 0; n < 8; n++) {
	    buf[n] = (uch)get_byte(); /* may cause an error if EOF */
	}
	orig_crc = LG(buf);
	orig_len = LG(buf+4);

    } else if (ext_header) {  /* If extended header, check it */
	/* signature - 4bytes: 0x50 0x4b 0x07 0x08
	 * CRC-32 value
         * compressed size 4-bytes
         * uncompressed size 4-bytes
	 */
	for (n = 0; n < EXTHDR; n++) {
	    buf[n] = (uch)get_byte(); /* may cause an error if EOF */
	}
	orig_crc = LG(buf+4);
	orig_len = LG(buf+12);
    }

    /* Validate decompression */
    if (orig_crc != updcrc(outbuf, 0)) {
	error("invalid compressed data--crc error");
    }
    if (orig_len != (ulg)bytes_out) {
	error("invalid compressed data--length error");
    }

    /* Check if there are more entries in a pkzip file */
    if (pkzip && inptr + 4 < insize && LG(inbuf+inptr) == LOCSIG) {
	if (to_stdout) {
	    WARN((stderr,
		  "%s: %s has more than one entry--rest ignored\n",
		  progname, ifname));
	} else {
	    /* Don't destroy the input zip file */
#ifndef _MAC_ERRORS_H_ /* spd */
	    fprintf(stderr,
		    "%s: %s has more than one entry -- unchanged\n",
#else
		DoError(NO_ERR, WARN_ERR, "%s: %s has more than one entry -- unchanged",
#endif
		    progname, ifname);
	    exit_code = ERROR;
	    ext_header = pkzip = 0;
	    return ERROR;
	}
    }
    ext_header = pkzip = 0; /* for next file */
    return OK;
}
