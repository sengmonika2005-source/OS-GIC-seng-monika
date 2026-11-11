#!/usr/bin/env python3
"""Compile and exercise the actual Linux programs; retain commands and evidence.
Run from WSL: python3 verify_linux.py
Only student-specific IPC objects are inspected; existing objects cause an abort.
"""
import json
import os
from pathlib import Path
import re
import subprocess
import time

os.environ['COLUMNS'] = '120'
os.environ['LINES'] = '40'
ROOT = Path(__file__).resolve().parent
EVIDENCE = ROOT / "evidence"
EVIDENCE.mkdir(exist_ok=True)
SUMMARY = {"environment": {}, "checks": []}
SUMMARY["environment"]["uname"] = subprocess.check_output(["uname", "-a"], text=True).strip()
SUMMARY["environment"]["gcc"] = subprocess.check_output(["gcc", "--version"], text=True).splitlines()[0]
SUMMARY["environment"]["os_release"] = Path("/etc/os-release").read_text()
SUMMARY["environment"]["date"] = subprocess.check_output(["date", "--iso-8601=seconds"], text=True).strip()


def check(condition, description):
    assert condition, description
    SUMMARY["checks"].append(description)


def execute(command, folder, log, expected=0, output=None, env=None, timeout=10):
    log.write("$ " + command + "\n")
    log.flush()
    run = subprocess.run(["bash", "-c", command], cwd=ROOT / folder,
                         text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                         timeout=timeout, env=env)
    log.write(run.stdout)
    log.write(f"[exit status: {run.returncode}]\n\n")
    log.flush()
    if output:
        (ROOT / folder / output).write_text(run.stdout)
    check(run.returncode == expected, f"{folder}: {command} => {expected}")
    return run.stdout


shm = Path("/dev/shm/OS-e20231167")
mq = Path("/dev/mqueue/queue-e20231167")
check(not shm.exists(), "No stale student shared memory object before testing")
check(not mq.exists(), "No stale student message queue before testing")

with (EVIDENCE / "task1-linux.txt").open("w") as log:
    execute("gcc -Wall -Wextra -o forkchild forkchild.c", "task1", log)
    log.write("$ ./forkchild > result_forkchild.txt 2>&1\n")
    log.flush()
    with (ROOT / "task1/result_forkchild.txt").open("w") as output:
        process = subprocess.Popen(["./forkchild"], cwd=ROOT / "task1", stdout=output,
                                   stderr=subprocess.STDOUT)
        try:
            deadline = time.monotonic() + 10
            while time.monotonic() < deadline:
                text = (ROOT / "task1/result_forkchild.txt").read_text()
                child_match = re.search(r"Child process \(PID: (\d+), Parent PID: (\d+)\)", text)
                if child_match:
                    break
                time.sleep(0.05)
            check(child_match is not None, "Child PID appeared before the 30-second sleep completed")
            child, parent = map(int, child_match.groups())
            check(parent == process.pid, "Child's printed PPID equals the launched parent's PID")
            tree_command = f"ps --forest -o pid,ppid,stat,args -p {parent},{child}"
            tree = execute(tree_command, "task1", log)
            (ROOT / "task1/process_tree.txt").write_text("$ " + tree_command + "\n" + tree)
            rows = [line.split(None, 3) for line in tree.splitlines() if line.split() and line.split()[0].isdigit()]
            check(any(int(row[0]) == child and int(row[1]) == parent for row in rows),
                  "ps confirms the child-parent relationship while the child sleeps")
            code = process.wait(timeout=40)
        finally:
            if process.poll() is None:
                process.kill()
                process.wait()
    log.write(f"[exit status: {code}]\n\n")
    check(code == 0, "forkchild exits successfully after ls -la")
    final_output = execute("cat result_forkchild.txt", "task1", log)
    check(final_output.count("Parent process (PID:") == 1, "No duplicated pre-fork buffered output")
    check("total " in final_output and "child exited with status 0" in final_output,
          "ls -la output and successful child exit status are saved")
    SUMMARY["linux_process"] = {"parent_pid": parent, "child_pid": child, "exit_code": code}

with (EVIDENCE / "task3-linux.txt").open("w") as log:
    execute("gcc -Wall -Wextra -o shm-producer shm-producer.c -lrt", "task3_shm", log)
    execute("gcc -Wall -Wextra -o shm-consumer shm-consumer.c -lrt", "task3_shm", log)
    before = execute("./shm-consumer", "task3_shm", log, expected=1,
                     output="consumer-before-producer.txt")
    check("No such file or directory" in before, "Consumer-before-producer reports missing shared memory")
    execute("./shm-producer", "task3_shm", log, output="result-shm-producer.txt")
    check(shm.stat().st_size == 4096, "Shared memory object is exactly 4096 bytes")
    duplicate = execute("./shm-producer", "task3_shm", log, expected=1)
    check("File exists" in duplicate, "Duplicate producer refuses to overwrite an unconsumed object")
    execute("./shm-consumer > result-shm-ipc.txt", "task3_shm", log)
    received = execute("cat result-shm-ipc.txt", "task3_shm", log)
    check('message = "Hello, this is shared memory IPC!"' in received, "Consumer receives the exact teacher message")
    check(not shm.exists(), "Consumer removes the shared memory name")
    execute("./shm-consumer", "task3_shm", log, expected=1,
            output="consumer-after-cleanup.txt")

with (EVIDENCE / "task4-linux.txt").open("w") as log:
    execute("gcc -Wall -Wextra -o sender sender.c -lrt", "task4_mq", log)
    execute("gcc -Wall -Wextra -o receiver receiver.c -lrt", "task4_mq", log)
    before = execute("./receiver", "task4_mq", log, expected=1,
                     output="receiver-before-sender.txt")
    check("No such file or directory" in before, "Receiver-before-sender reports missing queue")
    execute("./sender", "task4_mq", log, output="result-mq-sender.txt")
    check(mq.exists(), "Sender leaves a named queue for the receiver")
    execute("cat /dev/mqueue/queue-e20231167", "task4_mq", log)
    execute("./receiver > result-mq-ipc.txt", "task4_mq", log)
    received = execute("cat result-mq-ipc.txt", "task4_mq", log)
    check('message = "Hello from sender! This is message queue IPC."' in received,
          "Receiver receives the exact teacher message")
    check(not mq.exists(), "Receiver removes the message queue name")
    execute("./receiver", "task4_mq", log, expected=1,
            output="receiver-after-cleanup.txt")

for folder, name, evidence_name in [
    ("task1", "command_used_forkchild.txt", "task1-linux.txt"),
    ("task3_shm", "command_used_shm.txt", "task3-linux.txt"),
    ("task4_mq", "command_used_mq.txt", "task4-linux.txt"),
]:
    transcript_path = EVIDENCE / evidence_name
    # Normalize only transcript whitespace for git diff --check; raw result files stay intact.
    transcript = "\n".join(line.rstrip() for line in transcript_path.read_text().rstrip().splitlines()) + "\n"
    transcript_path.write_text(transcript)
    commands = [line[2:] for line in transcript.splitlines() if line.startswith("$ ")]
    (ROOT / folder / name).write_text(
        "# Commands actually executed by verify_linux.py, in this task directory.\n"
        + "\n".join(commands) + "\n")

(EVIDENCE / "linux-verification.json").write_text(json.dumps(SUMMARY, indent=2) + "\n")
print(f"PASS: {len(SUMMARY['checks'])} checks; parent PID {parent}, child PID {child}.")
print("Actual compilation/execution transcripts saved in evidence/; IPC names are cleaned up.")
