.intel_syntax noprefix
.text
.global sum
sum:
  mov r12, rcx
  mov rcx, rdi
  loopbegin:
  mov ebx, [rsi]
  mov eax, [rdx]
  add eax, ebx
  mov [r12], eax
  add r12, 4
  add rsi, 4
  add rdx, 4
  loop loopbegin
  ret