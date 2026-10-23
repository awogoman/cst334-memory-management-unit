#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

#include "student_code.h"

#define VALID_BIT (1ULL << 63)
#define READ_BIT (1ULL << 62)
#define WRITE_BIT (1ULL << 61)
#define EXECUTE_BIT (1ULL << 60)
#define PFN_MASK ((1ULL << 60) - 1)

// Page Helpers

// check if page table entry is valid
bool is_entry_valid(PageTableEntry pte) {
  return (pte & VALID_BIT) != 0;
}

// check if page has read permission
bool is_read_enabled(PageTableEntry pte) {
  return (pte & READ_BIT) != 0;
}

// check if page has write permission
bool is_write_enabled(PageTableEntry pte) {
  return (pte & WRITE_BIT) != 0;
}

// check if page has execute permission
bool is_execute_enabled(PageTableEntry pte) {
  return (pte & EXECUTE_BIT) != 0;
}


// MMU Helpers

// get vpn (va - offset)
VPN get_vpn_from_va(VirtualAddress va) {
  return va >> NUM_OFFSET_BITS;
}

// search for free frame and return pfn, or -1 if none found
PFN find_free_page(MMU m) {
  // loop through to find free page
  for (PFN pfn = 0; pfn < NUM_FRAMES; pfn++) {
    if (!m.page_used[pfn]) {
      return pfn;
    }
  }
  // if no page found
  return -1;
}

// get pfn from pte using mask
PFN convert_PageTableEntry_to_PFN(PageTableEntry pte) {
  return pte & PFN_MASK;
}


// Page Table Functions

// find pte of a given vpn
PageTableEntry get_pagetableentry__MMU_pagetable(MMU m, VPN vpn) {
  return m.page_pointer[vpn];
}

// create pte and map va to pf with given permissions
PFN map_page__MMU_pagetable(MMU* m, VPN vpn, bool can_read, bool can_write, bool can_exec) {
  PFN pfn = find_free_page(*m);
  if (pfn == -1) return -1;
  m->page_used[pfn] = true;
  PageTableEntry entry = (PageTableEntry)pfn;
  if (can_exec) entry |= EXECUTE_BIT;
  if (can_write) entry |= WRITE_BIT;
  if (can_read) entry |= READ_BIT;
  entry |= VALID_BIT;
  m->page_pointer[vpn] = entry;
  return pfn;
}


// Page Directory Functions

// look up page with given va
Page* get_page(MMU m, VirtualAddress va, bool for_read, bool for_write, bool for_execute) {
  VPN vpn = get_vpn_from_va(va);
  PageTableEntry pte = get_pagetableentry__MMU_pagetable(m, vpn);
  if (!is_entry_valid(pte)) return NULL;
  if (for_read && !is_read_enabled(pte)) return NULL;
  if (for_write && !is_write_enabled(pte)) return NULL;
  if (for_execute && !is_execute_enabled(pte)) return NULL;
  PFN pfn = convert_PageTableEntry_to_PFN(pte);
  return (Page*)(m.physical_memory + (pfn * PAGE_SIZE));
}

// read a byte from va (if read access)
char read_byte(MMU m, VirtualAddress va) {
  Page* page = get_page(m, va, true, false, false);
  if (page == NULL) return 0;
  uint64_t offset = va & ((1 << NUM_OFFSET_BITS) - 1);
  return page->data[offset];
}

// write a byte to va (if write access)
void write_byte(MMU m, VirtualAddress va, char val) {
  Page* page = get_page(m, va, false, true, false);
  if (page == NULL) return;
  uint64_t offset = va & ((1 << NUM_OFFSET_BITS) - 1);
  page->data[offset] = val;
}
