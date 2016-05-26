=================================================
Abstract
=================================================

=================================================
Introduction
=================================================
# What's the use of SFI
	- Secure possibly malicious code through software solution
	- Usage in applications which calls modules from unknown origin
		+ browsers
		+ computer cluster
	- Current appeal for SFI due to good performances and small TCB
FB: Motivate fixed frame size
FB: SFI not very flexible -- difficult to integrate different modules
FB: Propose a more flexible approach which protects againsts the most important attack: smashing of the return address
# Plan
	- SFI
	- our idea: Protecting the stack with fixed frame size
	- Analysis of our approach
	- Conclucsion

=================================================
Software Fault Isolation (tiré de la bibliographie)
=================================================
# Principle
	- Allows a main program using multiple modules to load them safely in its own memory making the communication faster
	- Code generator on the ASM which masks addresses before dangerous instructions
	- Code verifier which certifies that the executable have been secured by the generator before loading it to memory
	- Pros/Cons
		+ small TCB (verifier)
		+ generally faster than when we separate commnicating programs in multiple processes
		- The implementation is architecture dependent
		- main program is bigger and slower (just communicaiton is faster)
	- Implementations 
		+ NativeClient by Google

# SFI using CompCert
	- CompCert
		+ Certified compiler
	- SFI with CompCert
		+ Transformations are done in high level language (Cminor) => easily portable
		+ correctness of CompCert guarantees that the transformations are preserved up to the ASM
	- Evaluation of the approach
		+ smaller TCB (we need to trust SFI transformations)
		+ architecture independent
		- we need to compile the program ourself to have SFI properties
	
# Our approach
	- SFI have issues we indirect jump and especially ret instruction
		+ transform ret into pop + jmp => but ret is optimized in numerous processors
		(+ shadow stack)
	- we propose a solution to secure ret instruction with CompCert


=================================================
Protecting the stack with fixed frame size
=================================================
FB: Put the emphasis on the motivation 
FB: What do you want to achieve? Protect return address
FB: Why is-it difficult? Return addresses are on the stack -- programs are allowed to write on the stack...
# Description of the approach
	- Most attacks on software try to divert the flow of the program (stack overflow) and usually try to overwrite the return address of a function
	- Our approach aims to protect these return address
FB: We would like a way to be able to check at runtime that an address is a return address
FB: We need to enforce a strict discipline for the organisation of the stack
        - By setting stack frames size as constant we can know the return addresses location in memory
        - We detect every operation which is possibly harmful to return addresses and add a check in the program to prevent any illegal behaviour
	- In the case where we have illegal behaviour we make the program crash with null pointer exception

# Security prop
	- We work with CompCert so that one day we may be able to prove some security guarantees brought by the approach
	1. Constant size to stack frames, $$$ return addresses locations are all separated by a constant amount of bytes $$$
    2. Stack alignment, $$$ the beginning of the stack will have its least significant bits equals to 0 $$$
        - The transformation needs to keep the semantic of the compiled program
		- Same as before, to keep the alignment we need to forbid fiddling with ESP
		- The program main needs to be called "main" so we detect which function to call after the virtual main
    3. Return adresses protection, $$$ It's not possible to write on return addresses reserved locations except in function call routine $$$
		- Find all possibly harmful statements	
		- Only keep addresses from the stack
        - Verify that the remaining targets does not have 0 lsb

# Evaluation of the approach
	- conditions for it to work
		+ no fiddling with ESP, should not be possible with CompCert
		+ inline assembly 
		+ The transformation needs to keep the semantic of the compiled program
	- flaws still exist throught glibc flawed functions (strcpy, printf...)
		=> rewrite glibc
		=> recompile with compcert with same frame size
	- this approach is dependent of the stack layout so it's not really portable and may be more complicated with certain compilers

=================================================
# Implementation and discussion
=================================================
	- We use CompCert because we hope that one day we may prove security properties about the approach
	- The stack layout in compcert
	- We do the transformation in x86-32

# Implementation
	1. Give a constant size to stack frames 
		- We chose a power of 2, to be able to use arithmetic pointer for the mask and it's also easier for alignment of memory
		- We put a check that prevents completion of compilation in case the size chosen is too small
		- CompCert stacklayout schema
	2.  Align the stack (ASM)
		- ASM because we have to work with ESP
		- Since frame size are constant return addresses will all have the same least significant bits
		- To ease the "check/mask" phase we want the least significant bits to be always equal to 0
		- For this to be true we need the stack to be aligned
		- We chose to introduce in each program a synthetic main function which will align the stack and call the real main function
	3. Protect return addresses (Clight)
		- Clight because it's high level, close to C and got every information we need
		- First thing is to detect possible harmful instructions, these are the assignments which write on memory locations (mostly pointers deref)
			+ why did we chose clight
			+ semantics of clight
			+ memory model to see which are dangerous
			+ Sassign with Ederef or Efield on the left side
		- Add a check/mask operation before the possibly harmful instructions
			+ Check first if the memory address written to is part of the stack or the heap (if then else), if it's in the heap do the assignment else continue with the check
				* First condition used is memory above 0xff000000 is part of the stack but it is not always true
				* Second approach is to put guard zones between the heap and the stack but not implemented yet
			+ Secondly, if the address is in the stack we check the least significant bits values
				* if it's in [0, 3] then it's return addresses location => crash, else we allow the assignment
				* First approach with classical if then else
				* Second try with branchless to improve performance

# Evaluation
	- Tools we used 
		+  Add a compilation pass which prints the return address values when running the compiled program
			* Easier task given to ease the dive in the huge CompCert project
			* Useful tool to debug later on)
	- Experimental setup
	- Experimental results
		+ can introduce a delay up to 150% with the classic implementation in our tests
	- Discussion
		+ Did we cover all dangerous statements?
			=> Check function pointer for jmp after the check
		+ implementation on x86 only and the stack layout may be completely different
    	+ still not finished (have to find a good solution to differentiate between stack and heap addresses)
					=> guard area between stack and heap (but where exactly?)
			FB: Previous section. Guard does not work if stack and heap are mixed
			FB: Solution add between them unmapped memory
			FB: This is not implemented.
			+ heavy programs might have memory overflows since we take the max needed frame size of the program as the frame size constant
		=> have to make measures to compare stack usage
		+ We are confident that our transformations keep the semantics
			=> more tests and eventually a formal proof
	=> we may find better heuristics about which statement should be checked
			FB: This should be explained in the experiments section
			FB: Not heuristics : static analysis



=================================================
Conclusion
=================================================

The current results we have are average, but there are still numerous ways to improve the implementation
It would be interesting to have more tests and to try our implementation with complicated unsecure C program.


