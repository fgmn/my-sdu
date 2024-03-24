/* coff.h
 *   Data structures that describe the MIPS COFF format.
 */

#ifdef HOST_ALPHA		/* Needed because of gcc uses 64 bit long  */
#define _long int		/* integers on the DEC ALPHA architecture. */
#else
#define _long int
#endif

struct filehdr {
        unsigned short  f_magic;        /* magic number */
        unsigned short  f_nscns;        /* number of sections */
        _long           f_timdat;       /* time & date stamp */
        _long           f_symptr;       /* file pointer to symbolic header */
        _long           f_nsyms;        /* sizeof(symbolic hdr) */
        unsigned short  f_opthdr;       /* sizeof(optional hdr) */
        unsigned short  f_flags;        /* flags */
      };

#define  MIPSELMAGIC    0x0162

#define OMAGIC  0407
#define SOMAGIC 0x0701

typedef struct aouthdr {
        short   magic;          /* see above                            */
        short   vstamp;         /* version stamp                        */
        _long    tsize;         /* text size in bytes, padded to DW bdry*/
        _long    dsize;         /* initialized data "  "                */
        _long    bsize;         /* uninitialized data "   "             */
        _long    entry;         /* entry pt.                            */
        _long    text_start;    /* base of text used for this file      */
        _long    data_start;    /* base of data used for this file      */
        _long    bss_start;     /* base of bss used for this file       */
        _long    gprmask;       /* general purpose register mask        */
        _long    cprmask[4];    /* co-processor register masks          */
        _long    gp_value;      /* the gp value used for this object    */
      } AOUTHDR;
#define AOUTHSZ sizeof(AOUTHDR)
 

struct scnhdr {
        char            s_name[8];      /* section name */
        _long           s_paddr;        /* physical address, aliased s_nlib */
        _long           s_vaddr;        /* virtual address */
        _long           s_size;         /* section size */
        _long           s_scnptr;       /* file ptr to raw data for section */
        _long           s_relptr;       /* file ptr to relocation */
        _long           s_lnnoptr;      /* file ptr to gp histogram */
        unsigned short  s_nreloc;       /* number of relocation entries */
        unsigned short  s_nlnno;        /* number of gp histogram entries */
        _long           s_flags;        /* flags */
      };
 
