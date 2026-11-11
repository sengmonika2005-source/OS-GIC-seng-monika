# Student drawing and screenshot checklist

All drawing and screenshots are left to Seng Monika by request. No image files
are supplied as placeholders. Add the seven genuine PNGs below and then add
relative image links to the main README. Use the PIDs observed in your own run.

| Filename | Capture |
|---|---|
| `task1_compile_run.png` | Ubuntu terminal: compile, run, and show actual saved output |
| `task1_process_tree.png` | Second Ubuntu terminal while the child sleeps: parent-child forest and PIDs |
| `task2_compile_run.png` | Native Windows PowerShell: compile and launch, showing parent/child PIDs and thread ID |
| `task2_taskmanager_tree.png` | Task Manager Processes view; expand relevant groups if the genuine relationship is available |
| `task2_taskmanager_details.png` | Task Manager Details view, with PID and Parent PID if supported; otherwise supplement with real CIM output and explain the missing column |
| `task3_compile_run.png` | Ubuntu terminal: compile both programs and display producer/consumer message |
| `task4_compile_run.png` | Ubuntu terminal: compile both programs and display sender/receiver message |

## Linux commands

From `/home/monika/os-gic-seng-monika-repo/os-class-activities-seng-monika/activity2`:

```bash
cd task1
gcc -Wall -Wextra -o forkchild forkchild.c
./forkchild > result_forkchild.txt 2>&1
cat result_forkchild.txt
```

During the 30-second sleep, run in another Ubuntu terminal:

```bash
ps --forest -o pid,ppid,stat,args -C forkchild
```

Draw the observed parent-child hierarchy using those live PIDs. The recorded
automated test used parent 11362 and child 11363; new runs will have new IDs.

```bash
cd ../task3_shm
gcc -Wall -Wextra -o shm-producer shm-producer.c -lrt
gcc -Wall -Wextra -o shm-consumer shm-consumer.c -lrt
./shm-producer
./shm-consumer > result-shm-ipc.txt
cat result-shm-ipc.txt
cd ../task4_mq
gcc -Wall -Wextra -o sender sender.c -lrt
gcc -Wall -Wextra -o receiver receiver.c -lrt
./sender
./receiver > result-mq-ipc.txt
cat result-mq-ipc.txt
```

## Native Windows commands

Open Windows PowerShell and enter the existing local task directory:

```powershell
Set-Location '\\wsl.localhost\Ubuntu\home\monika\os-gic-seng-monika-repo\os-class-activities-seng-monika\activity2\task2'
gcc -Wall -Wextra -o winprocess.exe winprocess.c
$paintPath = Join-Path (Get-AppxPackage -Name Microsoft.Paint).InstallLocation 'PaintApp\mspaint.exe'
.\winprocess.exe $paintPath
```

While Paint is open, use another PowerShell window for independent PPID evidence:

```powershell
Get-CimInstance Win32_Process |
    Where-Object { $_.Name -in @('winprocess.exe','mspaint.exe') } |
    Select-Object Name,ProcessId,ParentProcessId
```

Open Task Manager with Ctrl+Shift+Esc. Inspect Processes and Details. This
Windows version may not expose the teacher's requested Parent PID column or
actual parent-child grouping. Do not invent those views. Capture the real views
and the CIM table, then explain any limitation in the main README. If required,
confirm the alternative evidence with the teacher. Close Paint after observing
it to show the parent's final exit/cleanup message.

Update the README's observations and screenshot links to match the new run,
then commit and push only Activity 2. Do not claim the exact Task Manager
requirements were met if only alternative process evidence is available.
