#pmake: build trap remote os_linux cpu_386 par

tree_depth = 4

host_cpu = 386
host_os  = linux

target_cpu = x86

srv = par
which = TRAP
remote_link = 1

extra_objs = parlinux.obj

inc_dirs   = -I"$(trap_dir)/lcl/linux/h"

!include $(trap_dir)/master.mif
