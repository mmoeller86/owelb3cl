#pmake: build serv remote os_dos cpu_i86 cpu_386 nov x86

tree_depth = 4

host_cpu = i86
host_os  = dos

target_cpu = x86

srv = nov
which = SERVER
remote_link = 1

extra_objs = novdos.obj spx.obj ipx.obj
extra_flags_novdos = -zu

!include $(trap_dir)/master.mif
