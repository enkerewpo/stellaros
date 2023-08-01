use x86_64::{PhysAddr, structures::paging::PageTable, VirtAddr};
use x86_64::registers::control::Cr3;
use x86_64::structures::paging::OffsetPageTable;

pub unsafe fn active_level4_table(physical_memory_offset: VirtAddr) -> &'static mut PageTable {
  let (level4_table_frame, _) = Cr3::read();
  let phys = level4_table_frame.start_address();
  let virt = physical_memory_offset + phys.as_u64();
  let page_table_ptr: *mut PageTable = virt.as_mut_ptr();
  &mut *page_table_ptr
}

pub unsafe fn init(physical_memory_offset: VirtAddr) -> OffsetPageTable<'static> {
  let level4_table = active_level4_table(physical_memory_offset);
  OffsetPageTable::new(level4_table, physical_memory_offset)
}

pub unsafe fn vaddr_to_paddr(addr: VirtAddr, physical_memory_offset: VirtAddr) -> Option<PhysAddr> {
  __vaddr_to_paddr(addr, physical_memory_offset)
}

fn __vaddr_to_paddr(addr: VirtAddr, physical_memory_offset: VirtAddr) -> Option<PhysAddr> {
  use x86_64::structures::paging::page_table::FrameError;
  let (level_4_table_frame, _) = Cr3::read(); // get the level 4 frame
  let table_indexes = [
    addr.p4_index(), addr.p3_index(), addr.p2_index(), addr.p1_index()
  ];
  let mut frame = level_4_table_frame;
  for &index in &table_indexes {
    let virt = physical_memory_offset + frame.start_address().as_u64();
    let table_ptr: *const PageTable = virt.as_ptr();
    let table = unsafe { &*table_ptr };
    let entry = &table[index];
    frame = match entry.frame() {
      Ok(frame) => frame,
      Err(FrameError::FrameNotPresent) => return None,
      Err(FrameError::HugeFrame) => panic!("huge pages not supported"),
    };
  }
  Some(frame.start_address() + u64::from(addr.page_offset()))
}