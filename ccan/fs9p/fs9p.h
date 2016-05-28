#ifndef CCAN_FS9P_H_
#define CCAN_FS9P_H_

/*
 * What should this api look like?
 *
 * sd-bus?
 * fuse?
 *
 * The goal here is to provide an interface for programs to export state and
 * allow interaction with that state by external sources. sd-bus's goals more
 * closely match that than fuse (which is intended as a general purpose
 * filesystem interface). By avoiding being entirely general, we can provide an
 * interface more suited to our targeted use case.
 */

/**
 * represents a top-level exported filesystem. One can think of it like a
 * "superblock", except it doesn't represent state that is actually stored
 * to a block on disk.
 *
 */
struct fs9p {

};

/* define the contents of a directory (files, and other directories) */
struct fs9p_tree {

};

/* */
struct fs9p_file {

};



#endif
