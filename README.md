ANTz - High Performance Computing
====
ANTz - HPC version is aimed to push the envelope of visual processing.

An experimental variant of standard ANTz with optimizations to increase performance aimed at visual cluster such as CAVE's and other immersive enviroments.

Tasks:
- Apply to existing code-base (known) HPC modifications.
- Parrallel the OpenGL draw routines, physics and data processing.
- Parrallel multiple (cluster) systems.

---
Overview:

There are a number of known performance tweaks to be applied. These will be first hard-coded and then made into soft-user settable parameters, at which point the HPC branch can be synced (merged) with the standard (master) antz.

There are four stages to optimzing OpenGL draw routines.
- Optimize single thread GL performance (switch to using VBO objects, etc.)
- Parrellel the GL routines for a single GPU.
- Parrellel support for dual or quad GPU in a single system.
- Parrellel Visual Cluster support using [Equalizer library.](http://equalizergraphics.com)

Currently, ANTz uses direct drawing (immediate mode, glBegin()/glEnd() functions.) Approximate 2X performance gain by switing to modern VBO draw routines. This is also needed for ANTZ - ES (iphone/android ver.)

The greatest performance boost will come from parrallel GL code on a single GPU. Approximate 5X performance boost.

On multi-GPU game systems and workstations a further boost of about 2.5X (with 4 GPU's) can be achieved.

The most difficult optimization is Visual Cluster systems. This can result in additional overhead which reduces performance. However, with really realy good code (very difficult - months / years of coder time,) cluster performance can achieve better performance. Perhaps as much as a 4X boost with 8 or 16 systems.

All together, ANTz is theoretically capable of displaying a million ojbects at interactive framerates, 30fps on existing hardware.

---
wiki:

- [Performance](https://sf.net/p/openantz/wiki/Performance/)
- [Performance_Tips](https://sf.net/p/openantz/wiki/Performance_Tips/)
- [State_Machine](https://sf.net/p/openantz/wiki/State_Machine/)

