ENTRY(main)

PHDRS {
  text PT_LOAD FLAGS(5);
}

SECTIONS {
  . = 0x400000 + SIZEOF_HEADERS;
  .text     : { *(.text*) } :text
  /DISCARD/ : { *(.note*) *(.comment*) *(.eh_frame*) *(.gnu*) }
}
