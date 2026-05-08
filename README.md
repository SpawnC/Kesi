# Kesi

Kesi is a hobby kernel written in C++ to learn how operating systems and kernels work internally.

It is currently in very early development and is not intended for real-world use yet.

Current limitations include:

- No physical memory manager
- No heap allocator
- No virtual memory system
- No scheduler
- No userspace
- Nothing with memory

---

## Why is the project public already?

The repository is public from the beginning because I want to preserve a detailed development history, including the earliest stages of the kernel before features such as heap allocation and memory management exist.

The goal is to document the full evolution of the project rather than only publishing it once it becomes more advanced.

---

## Documentation

I know the pain of undocumented projects, so I decided to document every function declared in header files using Doxygen.

Documentation is available in a separate repository:

- [Kesi Kernel Documentation](https://github.com/spatulari/Kesi-Kernel-Documentation?utm_source=chatgpt.com)
