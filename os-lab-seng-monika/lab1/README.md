# Operating Systems — Lab 01

## Task 1: Kernel and Distribution

- **Kernel version:** `7.0.0-38-generic`
- **Distribution:** Ubuntu
- **Ubuntu version:** `26.04.1 LTS`
- **Codename:** `resolute`
- **Architecture:** `x86_64`
- **Hostname:** `i4gic-os`

I used `uname -a` and `lsb_release -a` to identify the operating system.

The Linux kernel manages hardware and system resources, while Ubuntu is a Linux distribution that includes the kernel and other software.

## Task 2: Files and Directories

**Commands practiced:** `pwd`, `ls`, `mkdir`, `cd`, `touch`, `echo`, `cat`, `cp`, `mv`, and `rm`.

I created a folder named `task2_files` and two files, `lemon.txt` and `cloud.txt`.

I wrote text into both files, copied `lemon.txt` to `lemon_copy.txt`, renamed `cloud.txt` to `cloud_renamed.txt`, and deleted the copied file.

**Final files:**
- `lemon.txt`
- `cloud_renamed.txt`

## Task 3: Package Management

**Package installed:** Midnight Commander (`mc`).

I used `apt-get` to install, remove, and purge the package.

- **After installation:** `mc` was installed at `/usr/bin/mc`, and `/etc/mc` existed.
- **After removal:** The program was removed, but `/etc/mc` still existed.
- **After purge:** `/etc/mc` no longer existed.

**Difference between remove and purge:**

`apt-get remove` removes the program but keeps its system configuration files. `apt-get purge` removes the program and its system configuration files.

## Task 4: Program vs. Process

I used `which sleep`, `sleep 30 &`, and `ps` to observe a running process.

The `ps` output showed a `sleep` process with PID `598862`.

**Explanation:** A program is a file stored on disk, while a process is a running instance of a program.

The `sleep` process runs temporarily, but the program file remains available after the process finishes.

## Task 5: Multitasking

I started four `sleep` processes in the background using `sleep 300 &`.

The `ps` command showed four processes with these PIDs:

- `599308`
- `599314`
- `599320`
- `599326`

**Observation:** The operating system can manage several processes at the same time, demonstrating multitasking.

## Task 6: Virtualization

- **Hypervisor vendor:** `KVM`
- **Kernel release:** `7.0.0-38-generic`
- **Hostname:** `i4gic-os`

I used `systemd-detect-virt`, `lscpu`, `uname -r`, and `hostname` to investigate virtualization.

**Explanation:** The `lscpu` output identified KVM as the hypervisor vendor, indicating that the server runs in a virtualized environment.

## Conclusion

In this lab, I learned how to identify Linux system information, manage files and directories, install and remove software, observe processes, understand multitasking, and identify virtualization.

I completed Tasks 1, 2, 4, 5, and 6 on the course server and Task 3 on my own Ubuntu environment.
