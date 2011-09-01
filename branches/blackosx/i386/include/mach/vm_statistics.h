/*
 * Copyright (c) 2000-2009 Apple Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 * 
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */
/*
 * @OSF_COPYRIGHT@
 */
/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989,1988,1987 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 */
/*
 *	File:	mach/vm_statistics.h
 *	Author:	Avadis Tevanian, Jr., Michael Wayne Young, David Golub
 *
 *	Virtual memory statistics structure.
 *
 */

#ifndef	_MACH_VM_STATISTICS_H_
#define	_MACH_VM_STATISTICS_H_

#include <mach/machine/vm_types.h>


/*
 * vm_statistics
 *
 * History:
 *	rev0 - 	original structure.
 *	rev1 - 	added purgable info (purgable_count and purges).
 *	rev2 - 	added speculative_count.
 *
 * Note: you cannot add any new fields to this structure. Add them below in
 * 	 vm_statistics64.
 */

struct vm_statistics {
	natural_t	free_count;		/* # of pages free */
	natural_t	active_count;		/* # of pages active */
	natural_t	inactive_count;		/* # of pages inactive */
	natural_t	wire_count;		/* # of pages wired down */
	natural_t	zero_fill_count;	/* # of zero fill pages */
	natural_t	reactivations;		/* # of pages reactivated */
	natural_t	pageins;		/* # of pageins */
	natural_t	pageouts;		/* # of pageouts */
	natural_t	faults;			/* # of faults */
	natural_t	cow_faults;		/* # of copy-on-writes */
	natural_t	lookups;		/* object cache lookups */
	natural_t	hits;			/* object cache hits */

	/* added for rev1 */
	natural_t	purgeable_count;	/* # of pages purgeable */
	natural_t	purges;			/* # of pages purged */

	/* added for rev2 */
	/*
	 * NB: speculative pages are already accounted for in "free_count",
	 * so "speculative_count" is the number of "free" pages that are
	 * used to hold data that was read speculatively from disk but
	 * haven't actually been used by anyone so far.
	 */
	natural_t	speculative_count;	/* # of pages speculative */
};

/* Used by all architectures */
typedef struct vm_statistics	*vm_statistics_t;
typedef struct vm_statistics	vm_statistics_data_t;

#if defined(__ppc__) /* On ppc, vm statistics are still 32-bit */

typedef struct vm_statistics	*vm_statistics64_t;
typedef struct vm_statistics	vm_statistics64_data_t;

#define VM_STATISTICS_TRUNCATE_TO_32_BIT(value) value

#else /* !(defined(__ppc__))  */

/* 
 * vm_statistics64
 *
 * History:
 *	rev0 - 	original structure.
 *	rev1 - 	added purgable info (purgable_count and purges).
 *	rev2 - 	added speculative_count.
 *	   ----
 *	rev3 - 	changed name to vm_statistics64.
 *		changed some fields in structure to 64-bit on 
 *		arm, i386 and x86_64 architectures.
 *
 */

struct vm_statistics64 {
	natural_t	free_count;		/* # of pages free */
	natural_t	active_count;		/* # of pages active */
	natural_t	inactive_count;		/* # of pages inactive */
	natural_t	wire_count;		/* # of pages wired down */
	uint64_t	zero_fill_count;	/* # of zero fill pages */
	uint64_t	reactivations;		/* # of pages reactivated */
	uint64_t	pageins;		/* # of pageins */
	uint64_t	pageouts;		/* # of pageouts */
	uint64_t	faults;			/* # of faults */
	uint64_t	cow_faults;		/* # of copy-on-writes */
	uint64_t	lookups;		/* object cache lookups */
	uint64_t	hits;			/* object cache hits */

	/* added for rev1 */
	uint64_t	purges;			/* # of pages purged */
	natural_t	purgeable_count;	/* # of pages purgeable */

	/* added for rev2 */
	/*
	 * NB: speculative pages are already accounted for in "free_count",
	 * so "speculative_count" is the number of "free" pages that are
	 * used to hold data that was read speculatively from disk but
	 * haven't actually been used by anyone so far.
	 */
	natural_t	speculative_count;	/* # of pages speculative */

}
#ifdef __arm__
__attribute__((aligned(8)))
#endif
;

typedef struct vm_statistics64	*vm_statistics64_t;
typedef struct vm_statistics64	vm_statistics64_data_t;

/* 
 * VM_STATISTICS_TRUNCATE_TO_32_BIT
 *
 * This is used by host_statistics() to truncate and peg the 64-bit in-kernel values from
 * vm_statistics64 to the 32-bit values of the older structure above (vm_statistics).
 */
#define VM_STATISTICS_TRUNCATE_TO_32_BIT(value) ((uint32_t)(((value) > UINT32_MAX ) ? UINT32_MAX : (value)))

#endif /* !(defined(__ppc__)) */


/* included for the vm_map_page_query call */

#define VM_PAGE_QUERY_PAGE_PRESENT      0x1
#define VM_PAGE_QUERY_PAGE_FICTITIOUS   0x2
#define VM_PAGE_QUERY_PAGE_REF          0x4
#define VM_PAGE_QUERY_PAGE_DIRTY        0x8
#define VM_PAGE_QUERY_PAGE_PAGED_OUT    0x10
#define VM_PAGE_QUERY_PAGE_COPIED       0x20
#define VM_PAGE_QUERY_PAGE_SPECULATIVE	0x40
#define VM_PAGE_QUERY_PAGE_EXTERNAL	0x80
#define VM_PAGE_QUERY_PAGE_CS_VALIDATED	0x100
#define VM_PAGE_QUERY_PAGE_CS_TAINTED	0x200


/*
 * VM allocation flags:
 * 
 * VM_FLAGS_FIXED
 * 	(really the absence of VM_FLAGS_ANYWHERE)
 *	Allocate new VM region at the specified virtual address, if possible.
 * 
 * VM_FLAGS_ANYWHERE
 *	Allocate new VM region anywhere it would fit in the address space.
 *
 * VM_FLAGS_PURGABLE
 *	Create a purgable VM object for that new VM region.
 *
 * VM_FLAGS_NO_PMAP_CHECK
 *	(for DEBUG kernel config only, ignored for other configs)
 *	Do not check that there is no stale pmap mapping for the new VM region.
 *	This is useful for kernel memory allocations at bootstrap when building
 *	the initial kernel address space while some memory is already in use.
 *
 * VM_FLAGS_OVERWRITE
 *	The new VM region can replace existing VM regions if necessary
 *	(to be used in combination with VM_FLAGS_FIXED).
 *
 * VM_FLAGS_NO_CACHE
 *	Pages brought in to this VM region are placed on the speculative
 *	queue instead of the active queue.  In other words, they are not
 *	cached so that they will be stolen first if memory runs low.
 */
#define VM_FLAGS_FIXED		0x0000
#define VM_FLAGS_ANYWHERE	0x0001
#define VM_FLAGS_PURGABLE	0x0002
#define VM_FLAGS_NO_CACHE	0x0010

/*
 * VM_FLAGS_SUPERPAGE_MASK
 *	3 bits that specify whether large pages should be used instead of
 *	base pages (!=0), as well as the requested page size.
 */
#define VM_FLAGS_SUPERPAGE_MASK	0x70000	/* bits 0x10000, 0x20000, 0x40000 */
#define VM_FLAGS_SUPERPAGE_SHIFT 16

#define SUPERPAGE_NONE			0	/* no superpages, if all bits are 0 */
#define VM_FLAGS_SUPERPAGE_NONE		(SUPERPAGE_NONE<<VM_FLAGS_SUPERPAGE_SHIFT)
#if defined(__x86_64__) || !defined(KERNEL)
#define SUPERPAGE_SIZE_2MB		1
#define VM_FLAGS_SUPERPAGE_SIZE_2MB	(SUPERPAGE_SIZE_2MB<<VM_FLAGS_SUPERPAGE_SHIFT)
#endif

#define VM_FLAGS_ALIAS_MASK	0xFF000000
#define VM_GET_FLAGS_ALIAS(flags, alias)			\
		(alias) = ((flags) & VM_FLAGS_ALIAS_MASK) >> 24	
#define VM_SET_FLAGS_ALIAS(flags, alias)			\
		(flags) = (((flags) & ~VM_FLAGS_ALIAS_MASK) |	\
		(((alias) & ~VM_FLAGS_ALIAS_MASK) << 24))

/* These are the flags that we accept from user-space */
#define VM_FLAGS_USER_ALLOCATE	(VM_FLAGS_FIXED |		\
				 VM_FLAGS_ANYWHERE |		\
				 VM_FLAGS_PURGABLE |		\
				 VM_FLAGS_NO_CACHE |		\
				 VM_FLAGS_SUPERPAGE_MASK |	\
				 VM_FLAGS_ALIAS_MASK)
#define VM_FLAGS_USER_MAP	VM_FLAGS_USER_ALLOCATE

#define VM_MEMORY_MALLOC 1
#define VM_MEMORY_MALLOC_SMALL 2
#define VM_MEMORY_MALLOC_LARGE 3
#define VM_MEMORY_MALLOC_HUGE 4
#define VM_MEMORY_SBRK 5// uninteresting -- no one should call
#define VM_MEMORY_REALLOC 6
#define VM_MEMORY_MALLOC_TINY 7
#define VM_MEMORY_MALLOC_LARGE_REUSABLE 8
#define VM_MEMORY_MALLOC_LARGE_REUSED 9

#define VM_MEMORY_ANALYSIS_TOOL 10

#define VM_MEMORY_MACH_MSG 20
#define VM_MEMORY_IOKIT	21
#define VM_MEMORY_STACK  30
#define VM_MEMORY_GUARD  31
#define	VM_MEMORY_SHARED_PMAP 32
/* memory containing a dylib */
#define VM_MEMORY_DYLIB	33
#define VM_MEMORY_OBJC_DISPATCHERS 34

// Placeholders for now -- as we analyze the libraries and find how they
// use memory, we can make these labels more specific.
#define VM_MEMORY_APPKIT 40
#define VM_MEMORY_FOUNDATION 41
#define VM_MEMORY_COREGRAPHICS 42
#define VM_MEMORY_CARBON 43
#define VM_MEMORY_JAVA 44
#define VM_MEMORY_ATS 50
#define VM_MEMORY_LAYERKIT 51
#define VM_MEMORY_CGIMAGE 52
#define VM_MEMORY_TCMALLOC 53

/* private raster data (i.e. layers, some images, QGL allocator) */
#define	VM_MEMORY_COREGRAPHICS_DATA	54

/* shared image and font caches */
#define VM_MEMORY_COREGRAPHICS_SHARED	55

/* Memory used for virtual framebuffers, shadowing buffers, etc... */
#define	VM_MEMORY_COREGRAPHICS_FRAMEBUFFERS	56

/* Window backing stores, custom shadow data, and compressed backing stores */
#define VM_MEMORY_COREGRAPHICS_BACKINGSTORES	57

/* catch-all for other uses, such as the read-only shared data page */
#define VM_MEMORY_COREGRAPHICS_MISC VM_MEMORY_COREGRAPHICS

/* memory allocated by the dynamic loader for itself */
#define VM_MEMORY_DYLD 60
/* malloc'd memory created by dyld */
#define VM_MEMORY_DYLD_MALLOC 61

/* Used for sqlite page cache */
#define VM_MEMORY_SQLITE 62

/* JavaScriptCore heaps */
#define VM_MEMORY_JAVASCRIPT_CORE 63
/* memory allocated for the JIT */
#define VM_MEMORY_JAVASCRIPT_JIT_EXECUTABLE_ALLOCATOR 64
#define VM_MEMORY_JAVASCRIPT_JIT_REGISTER_FILE 65

/* memory allocated for GLSL */
#define VM_MEMORY_GLSL  66

/* Reserve 240-255 for application */
#define VM_MEMORY_APPLICATION_SPECIFIC_1 240
#define VM_MEMORY_APPLICATION_SPECIFIC_16 255

#define VM_MAKE_TAG(tag) ((tag) << 24)

#endif	/* _MACH_VM_STATISTICS_H_ */
