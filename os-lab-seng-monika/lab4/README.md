# Operating Systems — Lab 4
## Redirection and Pipelines

**Student:** gic-seng-monika
**Student ID:** E20231167
**Course:** Operating Systems
**Lab:** 4 — Redirection and Pipelines

---

## 1. Objectives

- Understand stdin, stdout, and stderr.
- Practice output and error redirection.
- Use pipelines to connect Linux commands.
- Select records using `grep`.
- Extract fields using `cut`.
- Count matching records using `wc -l`.
- Test changed inputs and missing files.

## 2. Personal Values

| Variable | Value |
|---|---|
| Status | clear |
| Other status | pass |

## 3. Core 1 — Select, Extract and Count

Commands used:

```bash
grep '^clear,' data/events.csv | tee evidence/selected.txt
grep '^clear,' data/events.csv | cut -d, -f2 | tee evidence/fields.txt
grep '^clear,' data/events.csv | wc -l > evidence/normal.txt
```

Results:
- Selected records: 3
- Extracted quantities: 9, 7, 7
- Matching record count: 3

## 4. Core 2 — Changed and Missing Inputs

| Test | Result |
|---|---|
| No matching records | 0 |
| Changed input | 4 |
| Missing file exit status | 1 |
| Standard output | Empty |
| Standard error | Missing-file diagnostic |

The missing-file test demonstrated that normal output and error messages can be redirected separately.

## 5. Redirection and Pipelines

Operators practiced:

- `>` — overwrite a file.
- `>>` — append to a file.
- `2>` — redirect standard error.
- `2>&1` — redirect standard error to standard output's current destination.
- `|` — connect standard output to another command's standard input.
- `tee` — display and save output.

## 6. Live Checkpoint

The checkpoint used `data/tiger.csv`.

- Exact matching status: `clear`
- Selected records: 2
- Record count saved in `checkpoint.txt`: 2

Evidence:
- `evidence/checkpoint-selected.txt`
- `checkpoint.txt`

## 7. Prediction and Debrief

The prediction and debrief activities were completed and submitted to the instructor.

## 8. Conclusion

This lab demonstrated how Linux redirection and pipelines manage command input, normal output, and error messages. The combination of `grep`, `cut`, and `wc -l` provided an effective method for selecting records, extracting fields, and counting matching lines.

All five Lab 4 checker milestones passed.
