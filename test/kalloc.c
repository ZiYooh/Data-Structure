// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, pgae table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"

void freerange(void *vstart, void *vend);
extern char end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld
int numfreepages = 0;
int free_frame_cnt = 0; // 변수명 변경

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
  uint page_Reference_Count[PHYSTOP >> PGSHIFT]; // 변수명 수정하기
} kmem;

// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{
  initlock(&kmem.lock, "kmem");
  kmem.use_lock = 0;
  free_frame_cnt = 0; // 변수명 수정하기
  freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend)
{
  freerange(vstart, vend);
  kmem.use_lock = 1;
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE)
    kmem.page_Reference_Count[V2P(p) >> PGSHIFT] = 0;  // 변수명 수정하기
    kfree(p);
}
//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
    panic("kfree");

  // 수정하기
  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = (struct run*)v;

  // 수정하기
  if(kmem.page_Reference_Count[V2P(v) >> PGSHIFT] > 0)
    --kmem.page_Reference_Count[V2P(v) >> PGSHIFT];

  // 수정하기
  if(kmem.page_Reference_Count[V2P(v) >> PGSHIFT] == 0){       // Free the page only if there are no references to the page
    // Fill with junk to catch dangling refs.
    memset(v, 1, PGSIZE);
    free_frame_cnt++;       // Increment the number of free pages by 1 when a page is freed
    r->next = kmem.freelist;
    kmem.freelist = r;

  }

  /*
  // Fill with junk to catch dangling refs.
  memset(v, 1, PGSIZE);

  if(kmem.use_lock)
    acquire(&kmem.lock);
  numfreepages++;
  r = (struct run*)v;
  r->next = kmem.freelist;
  kmem.freelist = r;
  */
  if(kmem.use_lock)
    release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;

  if(kmem.use_lock)
    acquire(&kmem.lock);
  numfreepages--;
  r = kmem.freelist;

  // 수정하기
  if(r){
    kmem.freelist = r->next;
	free_frame_cnt--; 
  kmem.page_Reference_Count[V2P((char*)r) >> PGSHIFT] = 1; 
  }

  /*
  if(r)
    kmem.freelist = r->next;
  */


  if(kmem.use_lock)
    release(&kmem.lock);
  return (char*)r;
}

int freemem(){
  return numfreepages;
}


// 아래 함수들 변수랑 함수명 적당히 수정
uint num_Free_Frames(void)
{
  acquire(&kmem.lock);
  uint free_frame_cnt = free_frame_cnt;
  release(&kmem.lock);
  return free_frame_cnt;
}

void decrease_Page_Reference_Count(uint pa)
{
  if(pa < (uint)V2P(end) || pa >= PHYSTOP)
    panic("Decrement Reference Count of a Page");

  acquire(&kmem.lock);
  --kmem.page_Reference_Count[pa >> PGSHIFT];
  release(&kmem.lock);
}

void increase_Page_Reference_Count(uint pa)
{
  if(pa < (uint)V2P(end) || pa >= PHYSTOP)
    panic("Increment Reference Count of a Page");

  acquire(&kmem.lock);
  ++kmem.page_Reference_Count[pa >> PGSHIFT];
  release(&kmem.lock);
}
uint get_Page_Reference_Count(uint pa)
{
  if(pa < (uint)V2P(end) || pa >= PHYSTOP)
    panic("Retrieve the Reference Count of a Page");
  uint count;

  acquire(&kmem.lock);
  count = kmem.page_Reference_Count[pa >> PGSHIFT];
  release(&kmem.lock);

  return count;
}

