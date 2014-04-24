MEMORY
  {
    ram : ORIGIN = 0x80300000, LENGTH = 0x10000
  }
 
SECTIONS
  {
    .text : {
      main.o(.text*)
  } > ram
}