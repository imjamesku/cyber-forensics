# Trace systemcalls in hello2.packed
# ../../pin-3.11-97998-g7ecce2dac-gcc-linux/pin -t ../../pin-3.11-97998-g7ecce2dac-gcc-linux/source/tools/MyPinTool/obj-intel64/strace.so -- ./packed/hello2.packed

# Trace systemcalls in hello2
# ../../pin-3.11-97998-g7ecce2dac-gcc-linux/pin -t ../../pin-3.11-97998-g7ecce2dac-gcc-linux/source/tools/MyPinTool/obj-intel64/strace.so -- ./hello2

# Trace systemcalls in timebomb2.packed
../../pin-3.11-97998-g7ecce2dac-gcc-linux/pin -t ../../pin-3.11-97998-g7ecce2dac-gcc-linux/source/tools/MyPinTool/obj-intel64/strace.so -- ./packed/timebomb2.packed
cp strace.out syscall-trace/timebomb2.packed-syscalls.out

# Trace systemcalls in timebomb2
../../pin-3.11-97998-g7ecce2dac-gcc-linux/pin -t ../../pin-3.11-97998-g7ecce2dac-gcc-linux/source/tools/MyPinTool/obj-intel64/strace.so -- ./timebomb2.unpacked
cp strace.out syscall-trace/timebomb2-syscalls.out


# Run timeboumb2
../../pin-3.11-97998-g7ecce2dac-gcc-linux/pin -t /home/james/git-repos/cyber-forensics/pin-3.11-97998-g7ecce2dac-gcc-linux/source/tools/hw1/obj-intel64/hw1.so -- ./timebomb2.unpacked