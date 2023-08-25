#ifndef _BUNDLE_H_
#define _BUNDLE_H_


/* it's like closure */

#define BNDL_ALLOC(size) malloc(size)

#define BNDL_BEGIN struct {
#define BNDL_END_MAKE(name) } *name = BNDL_ALLOC(sizeof(*name)); if (!name) return 0

#define BNDL_END_CAST(name, ptr) } *bndl = ptr
#define BNDL_RETURN(name) return name

#define BNDL_PTR void*

#endif/*_BUNDLE_H_*/
