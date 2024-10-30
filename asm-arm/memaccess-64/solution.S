.text
  .global sum
sum:
  mov x4, 0
LoopBegin:
  cmp x4, x1
  bge LoopEnd
  mov x3, 0
  ldr x3, [x2]
  add x0, x0, x3
  add x2, x2, 4
  add x4, x4, 1
  b LoopBegin
LoopEnd:
  ret