#pmake: trap lcl os_dos dsx cpu_386 x86

tree_depth = 5

host_cpu = 386
host_os  = nt

target_cpu = jvm

srv = jvm
which = TRAP

extra_objs = $(jvmx_trap_objs) jvminfo.obj acclcl.obj
extra_cppflags = -DJVM -DJVMXTRAP
extra_c_flags = -fh

!include $(trap_dir)/lcl/jvm/jvm.mif
!include $(trap_dir)/master.mif
