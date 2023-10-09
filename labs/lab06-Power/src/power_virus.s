/*
Copyright 2019 ARM Ltd. and University of Cyprus
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

        .file   "main.s"
        .data
        .align 32
        .simdvalue:
            .long   0xaaaaaaaa
            .long   0x55555555
            .long   0x33333333
            .long   0xcccccccc
            .long   0xaaaaaaaa
            .long   0x55555555
            .long   0x33333333
            .long   0xcccccccc
        .text
        .globl  main
        main:
.LFB0:
        .cfi_startproc
        #pushq   %rbp
        .cfi_def_cfa_offset 8
        .cfi_offset 5, -8
        movl    %esp, %ebp
        .cfi_def_cfa_register 5



        #reg init


        mov $0x55555555, %rax
        mov $0x33333333, %rbx
        mov $0x22222222, %rdx
        mov $0x44444444, %rsi
        mov $0x77777777, %rdi

        fldpi
        fldpi
        fldpi
        fldpi
        fldpi
        fldpi
        fldpi
        
        vmovdqa .simdvalue(%rip), %ymm0
        vmovdqa .simdvalue(%rip), %ymm1
        vmovdqa .simdvalue(%rip), %ymm2
        vmovdqa .simdvalue(%rip), %ymm3
        vmovdqa .simdvalue(%rip), %ymm4
        vmovdqa .simdvalue(%rip), %ymm5
        vmovdqa .simdvalue(%rip), %ymm6
        vmovdqa .simdvalue(%rip), %ymm7
        vmovdqa .simdvalue(%rip), %ymm8
        vmovdqa .simdvalue(%rip), %ymm9
        vmovdqa .simdvalue(%rip), %ymm10
        vmovdqa .simdvalue(%rip), %ymm11
        vmovdqa .simdvalue(%rip), %ymm12
        vmovdqa .simdvalue(%rip), %ymm13
        vmovdqa .simdvalue(%rip), %ymm14
        vmovdqa .simdvalue(%rip), %ymm15

        mov $1000000000, %rcx  #leave for i--

        #subq    $304, %rsp

.L2:
      
	vmulpd %ymm9, %ymm13, %ymm11
	vaddpd %ymm5, %ymm12, %ymm10
	add %rsi, 68(%rsp)
	add %rax, 116(%rsp)
	mov 28(%rsp), %rbx
	vsubpd %ymm15, %ymm14, %ymm6
	vmulpd %ymm6, %ymm12, %ymm11
	vmulpd %ymm2, %ymm8, %ymm2
	vmaxpd %ymm4, %ymm1, %ymm7
	add %rsi, 96(%rsp)
	mov 116(%rsp), %rdi
	mov 20(%rsp), %rbx
	vmaxpd %ymm12, %ymm1, %ymm9
	vmulpd %ymm10, %ymm12, %ymm10
	shl $31, %rsi
	add 88(%rsp), %rbx
	vmulpd %ymm4, %ymm0, %ymm10
	vmulpd %ymm2, %ymm15, %ymm15
	vmulpd %ymm15, %ymm1, %ymm0
	mov 96(%rsp), %rsi
	vaddpd %ymm5, %ymm5, %ymm15
	mov 124(%rsp), %rbx
	vsubpd %ymm11, %ymm11, %ymm4
	vxorpd %ymm14, %ymm9, %ymm0
	vaddpd %ymm2, %ymm5, %ymm5
	vmulpd %ymm3, %ymm10, %ymm0
	vmulpd %ymm4, %ymm12, %ymm10
	vmulpd %ymm1, %ymm12, %ymm12
	mov %rdx, 28(%rsp)
	vmulpd %ymm6, %ymm6, %ymm0
	mov 68(%rsp), %rbx
	vmulpd %ymm2, %ymm9, %ymm12
	vaddpd %ymm7, %ymm15, %ymm11
	vmaxpd %ymm4, %ymm0, %ymm11
	mov 60(%rsp), %rax
	vxorpd %ymm8, %ymm8, %ymm3
	add %rbx, 108(%rsp)
	mov 80(%rsp), %rbx
	vsubpd %ymm5, %ymm15, %ymm15
	vmaxpd %ymm13, %ymm1, %ymm11
	vaddpd %ymm8, %ymm12, %ymm6
	vmulpd %ymm4, %ymm13, %ymm15
	add $1646404055, %rdx
	mov 52(%rsp), %rbx
	vmulpd %ymm15, %ymm14, %ymm3
	vmaxpd %ymm5, %ymm2, %ymm11
	vaddpd %ymm1, %ymm7, %ymm15
	mov 52(%rsp),%rax
	mov %rbx,36(%rsp)
	mov %rdx,88(%rsp)


        #sub $1,%rcx #remove this and below comment for fixed iterations
        #cmp $0, %rcx
        jmp     .L2

         #leave
        .cfi_restore 5
        .cfi_def_cfa 4, 4
       ret

        .cfi_endproc
.LFE0:
        .ident  "GCC: (GNU) 6.4.0"
